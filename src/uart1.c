static b32 UART1Init(void)
{
    b32 Result = 0;

    unsigned int Reg;
    Reg = *GPFSEL1;
    Reg &= ~((0x7 << 12) | (0x7 << 15)); // GPIO14, GPIO15
    Reg |= (0x2 << 12) | (0x2 << 15); // Alt5
    *GPFSEL1 = Reg;
    *GPPUD = 0; // Enable pins 14 and 15
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    BusyWait(150);
    *GPPUDCLK0 = 0; // Flush GPIO setup

    AUX->ENABLES |= AUX_ENABLES_UART_1_Msk;

    UART1->CNTL = 0;
    UART1->LCR = 3; // 8 bits
    UART1->MCR = 0;
    UART1->IER = 0;
    UART1->IIR = 0xC6; // Disable interrupts
    UART1->BAUD = 270; // 115200 baud
    UART1->CNTL = UART1_CNTL_RX_ENABLE_Msk | UART1_CNTL_TX_ENABLE_Msk; // Enable TX and RX

    Result = 1;

    return Result;
}

static b32 UART1Peek(char* C)
{
    b32 Result = 0;

    if(UART1->LSR & UART1_LSR_DATA_READY_Msk)
    {
        *C = UART1->IO;

        Result = 1;
    }

    return Result;
}

static void UART1Put(char C)
{
    while(!(UART1->LSR & UART1_LSR_TX_EMPTY_Msk))
    {
        // Do nothing
    }

    UART1->IO = C;
}

static void UART1Send(const void* Data, unsigned int Size)
{
    const char* At = Data;
    const char* End = At + Size;
    while(At != End)
    {
        while(!(UART1->LSR & UART1_LSR_TX_EMPTY_Msk))
        {
            // Do nothing
        }

        UART1->IO = *(At++);
    }
}

static char UART1Get(void)
{
    char Result = 0;

    while(!(UART1->LSR & UART1_LSR_DATA_READY_Msk))
    {
        // Do nothing
    }

    Result = UART1->IO;

    return Result;
}
