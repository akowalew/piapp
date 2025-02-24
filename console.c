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

static char ConsoleGetc(void)
{
	char Result = 0;

	do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x01));

	Result = *AUX_MU_IO;

	return Result;
}

