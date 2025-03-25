u32 FbWidth, FbHeight, FbPitch, FbRGB, FbSize;
u8* FbBuffer, *FbData;
b32 FbSwap;

static b32 FbInit(void)
{
	b32 Result = 0;

	volatile u32* At = Mbox;

	*(At++) = 35 * 4;
	*(At++) = MBOX_REQUEST;

	*(At++) = MBOX_TAG_FRAMEBUFFER_SET_PHYSICAL_WIDTH_HEIGHT;
	*(At++) = 8;
	*(At++) = 8;
	*(At++) = 1024;
	*(At++) = 768;

	*(At++) = MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_WIDTH_HEIGHT;
	*(At++) = 8;
	*(At++) = 8;
	*(At++) = 1024;
	*(At++) = 768 * 2;

	*(At++) = MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_OFFSET;
	*(At++) = 8;
	*(At++) = 8;
	*(At++) = 0;
	*(At++) = 0;

	*(At++) = MBOX_TAG_FRAMEBUFFER_SET_DEPTH;
	*(At++) = 4;
	*(At++) = 4;
	*(At++) = 32;

	*(At++) = MBOX_TAG_FRAMEBUFFER_SET_PIXEL_ORDER;
	*(At++) = 4;
	*(At++) = 4;
	*(At++) = 1;

	*(At++) = MBOX_TAG_FRAMEBUFFER_ALLOCATE;
	*(At++) = 8;
	*(At++) = 8;
	*(At++) = 4096;
	*(At++) = 0;

	*(At++) = MBOX_TAG_FRAMEBUFFER_GET_PITCH;
	*(At++) = 4;
	*(At++) = 4;
	*(At++) = 0;

	*(At++) = MBOX_TAG_LAST;

	if(MboxCall(MBOX_CH_PROP))
	{
		if(Mbox[20] == 32 && Mbox[28])
		{
			Mbox[28] &= 0x3FFFFFFF;
			FbWidth = Mbox[5];
			FbHeight = Mbox[6];
			FbPitch = Mbox[33];
			FbRGB = Mbox[24];
			FbBuffer = (void*)((unsigned)Mbox[28]);
			FbSize = FbHeight * FbPitch;
			FbData = FbBuffer + FbSize;
			FbSwap = 0;
			Result = 1;
		}
	}

	return Result;
}

static b32 FbSetVirtualOffset(u32 X, u32 Y)
{
	b32 Result = 0;

	Mbox[0] = 8 * 4;
	Mbox[1] = MBOX_REQUEST;
	Mbox[2] = MBOX_TAG_FRAMEBUFFER_SET_VIRTUAL_OFFSET;
	Mbox[3] = 2 * 4;
	Mbox[4] = 0;
	Mbox[5] = X;
	Mbox[6] = Y;
	Mbox[7] = MBOX_TAG_LAST;
	if(MboxCall(MBOX_CH_PROP))
	{
		Result = 1;
	}

	return Result;
}

static void FbWaitForVerticalSync(void)
{
    Mbox[0] = 7 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_FRAMEBUFFER_SET_VSYNC;
    Mbox[3] = 1 * 4;
    Mbox[4] = 0;
    Mbox[5] = 0;
    Mbox[6] = MBOX_TAG_LAST;
    MboxCallForever(MBOX_CH_PROP);
}

static void FbSyncAndSwapBuffers(void)
{
	FbSetVirtualOffset(0, FbSwap ? 0 : FbHeight);
	FbWaitForVerticalSync();
	FbData = FbSwap ? &FbBuffer[FbSize] : &FbBuffer[0];
	FbSwap = !FbSwap;
}

static void  __attribute__((optimize("-O4"))) FillRectangle(u32 X1, u32 Y1, u32 X2, u32 Y2, u32 Color)
{
	u8* Row = FbData + Y1 * FbPitch + X1 * 4;

	for(u32 Y = Y1; Y <= Y2; Y++)
	{
		u32* restrict At = (u32*) Row;
		u32* restrict End = ((u32*) Row) + (X2 - X1);
		while(At != End)
		{
			*(At++) = Color;
		}

		Row += FbPitch;
	}
}

#if 0
static void __attribute__((optimize("-O2"))) NiceGradient(u32 Time)
{
	u8* Row = FbData;

	u8 P1[] = {0x00, 0x00, 0xFF};
	u8 P2[] = {0x00, 0xFF, 0x00};
	u8 P3[] = {0xFF, 0x00, 0x00};
	u8 P4[] = {0xFF, 0xFF, 0x00};

	f32 InvHeight = 1.f / FbHeight;
	f32 InvWidth = 1.f / FbWidth;

	f32 DP12[] =
	{
		(P2[0]-P1[0]) * InvHeight,
		(P2[1]-P1[1]) * InvHeight,
		(P2[2]-P1[2]) * InvHeight,
	};

	f32 DP34[] =
	{
		(P4[0]-P3[0]) * InvHeight,
		(P4[1]-P3[1]) * InvHeight,
		(P4[2]-P3[2]) * InvHeight,
	};

	f32 K[] =
	{
		DP34[0]-DP12[0],
		DP34[1]-DP12[1],
		DP34[2]-DP12[2],
	};

	f32 L[] =
	{
		P3[0] - P1[0],
		P3[1] - P1[1],
		P3[2] - P1[2],
	};

	for(u32 Y = 0; Y < FbHeight; Y++)
	{
		u32* At = (u32*) Row;

		f32 Q1[] =
		{
			Y*DP12[0] + P1[0],
			Y*DP12[1] + P1[1],
			Y*DP12[2] + P1[2],
		};

		f32 DQ[] =
		{
			(Y*K[0] + L[0]) * InvWidth,
			(Y*K[1] + L[1]) * InvWidth,
			(Y*K[2] + L[2]) * InvWidth,
		};

		for(u32 X = 0; X < FbWidth; X++)
		{
			u8 V[] =
			{
				(u8)(X*DQ[0] + Q1[0]),
				(u8)(X*DQ[1] + Q1[1]),
				(u8)(X*DQ[2] + Q1[2]),
			};

			*(At++) = (V[0] << 16) | (V[1] << 8) | V[2];
		}

		Row += FbPitch;
	}
}
#else
static void __attribute__((optimize("-O2"))) NiceGradient(u32 Time)
{
	u8* Row = FbData;

	u32 Width = FbWidth/2;
	for(u32 Y = 0; Y < FbHeight; Y++)
	{
		u64* Col = (u64*) Row;

		for(u32 X = 0; X < Width; X++)
		{
			*(Col++) = (0x00FF00FFULL << 32) | 0x00FF00FF;
		}

		Row += FbPitch;
	}
}
#endif

static void __attribute__((optimize("-O2"))) FbFill(u32 Color)
{
	u8* Row = FbData;

	u32 Width = FbWidth/2/3;
	u64 Value = (((u64) Color) << 32) | Color;

	for(u32 Y = 0; Y < FbHeight; Y++)
	{
		u64* Col = (u64*) Row;
		for(u32 X = 0; X < Width; X++)
		{
			*(Col++) = Value;
			*(Col++) = Value;
			*(Col++) = Value;
		}

		Row += FbPitch;
	}
}
