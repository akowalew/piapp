static void ConsoleInit(void)
{
	unsigned int Reg;

	*AUX_ENABLE |= 1; // Enable UART1, AUX mini uart
	*AUX_MU_CNTL = 0;
	*AUX_MU_LCR = 3; // 8 bits
	*AUX_MU_MCR = 0;
	*AUX_MU_IER = 0;
	// *AUX_MU_IIR = 0xC6; // Disable interrupts
	*AUX_MU_BAUD = 270; // 115200 baud

	Reg = *GPFSEL1;
	Reg &= ~((0x7 << 12) | (0x7 << 15)); // GPIO14, GPIO15
	Reg |= (0x2 << 12) | (0x2 << 15); // Alt5
	*GPFSEL1 = Reg;

	*GPPUD = 0; // Enable pins 14 and 15
	*GPPUDCLK0 = (1 << 14) | (1 << 15);
	BusyWait(150);
	*GPPUDCLK0 = 0; // Flush GPIO setup

	*AUX_MU_CNTL = 3; // Enable TX and RX
}

static void ConsolePut(char C)
{
	do
	{
		asm volatile("nop");
	} while(!(*AUX_MU_LSR & 0x20));

	*AUX_MU_IO = C;
}

static void ConsoleSend(const void* Data, unsigned int Size)
{
	const char* At = Data;
	const char* End = At + Size;
	while(At != End)
	{
		do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x20));

		*AUX_MU_IO = *(At++);
	}
}

static unsigned
FormatStringArgs(char* Buffer, unsigned Length, const char* Format, va_list Args)
{
	char* BufferAt = Buffer;

	while(1)
	{
		char C1 = *(Format++);
		switch(C1)
		{
			case 0:
			{
				goto finish;
			} break;

			case '%':
			{
				char C2 = *(Format++);
				switch(C2)
				{
					case 0:
					{
						goto finish;
					} break;

					case 'd':
					{
						int Value = va_arg(Args, int);
						int Negative = 0;
						if(Value < 0)
						{
							Negative = 1;
							Value = -Value;
						}

						char Tmp[32];
						char* TmpAt = Tmp;
						*(TmpAt++) = 0;
						do
						{
							unsigned Num = Value / 10;
							unsigned Rem = Value - Num * 10;
							*(TmpAt++) = Rem + '0';
							Value = Num;
						} while(Value);

						if(Negative)
						{
							*(BufferAt++) = '-';
						}

						char C;
						while((C = *(--TmpAt)) != 0)
						{
							*(BufferAt++) = C;
						}
					} break;

					case '%':
					{
						*(BufferAt++) = C2;
					} break;

					default:
					{
						// TODO: Unknown modifier
					} break;
				}
			} break;

			default:
			{
				*(BufferAt++) = C1;
			} break;
		}
	}

finish:
	return (BufferAt - Buffer);
}


static void ConsolePrint(const char* String)
{
	char C;
	const char* At = String;
	while((C = *(At++)) != 0)
	{
		do
		{
			asm volatile("nop");
		} while(!(*AUX_MU_LSR & 0x20));

		*AUX_MU_IO = C;
	}
}

static void ConsolePrintf(const char* Format, ...)
{
	char Buffer[256];

	va_list Args;
	va_start(Args, Format);
	unsigned Count = FormatStringArgs(Buffer, sizeof(Buffer), Format, Args);
	va_end(Args);

	ConsoleSend(Buffer, Count);
}

static char ConsoleGetc(void)
{
	char Result = 0;

	do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x01));

	Result = *AUX_MU_IO;

	return Result;
}

