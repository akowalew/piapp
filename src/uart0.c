static b32 UART0Init(void)
{
	b32 Result = 0;

    Mbox[0] = 9 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_SET_CLOCK_RATE;
    Mbox[3] = 3 * 4;
    Mbox[4] = 8;
    Mbox[5] = MBOX_CLOCK_UART; // UART Clock
    Mbox[6] = 4000000; // MHz
    Mbox[7] = 0; // Clear turbo
    Mbox[8] = MBOX_TAG_LAST;
    if(MboxCall(MBOX_CH_PROP))
    {
        unsigned int Reg;

	    Reg = *GPFSEL1;
	    Reg &= ~((0x7 << 12) | (0x7 << 15)); // GPIO14, GPIO15
	    Reg |= (0x4 << 12) | (0x4 << 15); // Alt5
	    *GPFSEL1 = Reg;
	    *GPPUD = 0; // Enable pins 14 and 15
	    *GPPUDCLK0 = (1 << 14) | (1 << 15);
	    BusyWait(150);
	    *GPPUDCLK0 = 0; // Flush GPIO setup

        UART0->ICR = 0x7FF; // Clear interrupts
        UART0->IBRD = 2; // 115200
        UART0->FBRD = 0xB;
        UART0->LCR_H = 0x7 << 4; // 8n1, enable FIFOs
        UART0->CR = 0x301; // Enable Tx, Rx, UART

    	Result = 1;
    }

    return Result;
}

static void UART0Put(char C)
{
    while(UART0->FR & UART0_FR_TXFF_Msk)
    {
        // Do nothing
    }

    UART0->DR = C;
}

static b32 UART0Peek(char* C)
{
    b32 Result = 0;

    if(!(UART0->FR & UART0_FR_RXFE_Msk))
    {
        *C = UART0->DR;

        Result = 1;
    }

    return Result;
}

static char UART0Get(void)
{
    char Result = 0;

    while(UART0->FR & UART0_FR_RXFE_Msk)
    {
        // Do nothing
    }

    Result = UART0->DR;

    return Result;
}

static void UART0Send(const void* Data, unsigned int Size)
{
    const char* At = Data;
    const char* End = At + Size;
    while(At != End)
    {
        while(UART0->FR & UART0_FR_TXFF_Msk)
        {
            // Do nothing
        }

        UART0->DR = *(At++);
    }
}
