u32 FBWidth, FBHeight, FBPitch, FBRGB;
u8* FBData;

static b32 FBInit(void)
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
	*(At++) = 768;

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
			FBWidth = Mbox[5];
			FBHeight = Mbox[6];
			FBPitch = Mbox[33];
			FBRGB = Mbox[24];
			FBData = (void*)((u64)Mbox[28]);
			Result = 1;
		}
	}

	return Result;
}

static void  __attribute__((optimize("-O3"))) FillRectangle(u32 X1, u32 Y1, u32 X2, u32 Y2, u32 Color)
{
	u8* Row = FBData + Y1 * FBPitch + X1 * 4;

	for(u32 Y = Y1; Y <= Y2; Y++)
	{
		u32* At = (u32*) Row;

		for(u32 X = X1; X <= X2; X++)
		{
			*(At++) = Color;
		}

		Row += FBPitch;
	}
}

#include "assets/homer.h"

static void FBTest(void)
{
    u32 x,y;
    unsigned char *ptr=FBData;
    char *data=(void*)homer_data, pixel[4];

    ptr += (FBHeight-homer_height)/2*FBPitch + (FBWidth-homer_width)*2;
    for(y=0;y<homer_height;y++) {
        for(x=0;x<homer_width;x++) {
            HEADER_PIXEL(data, pixel);
            // the image is in RGB. So if we have an RGB framebuffer, we can copy the pixels
            // directly, but for BGR we must swap R (pixel[0]) and B (pixel[2]) channels.
            *((unsigned int*)ptr)=FBRGB ? *((unsigned int *)&pixel) : (unsigned int)(pixel[0]<<16 | pixel[1]<<8 | pixel[2]);
            ptr+=4;
        }
        ptr+=FBPitch-homer_width*4;
    }
}
