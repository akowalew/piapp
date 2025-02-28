static void ConsolePut(char C)
{
	UART1Put(C);
}

static char ConsoleGet(void)
{
    char Result = UART1Get();

    return Result;
}

static void ConsoleSend(const void* Data, unsigned int Size)
{
	UART1Send(Data, Size);
}

static void ConsolePrintf(const char* Format, ...)
{
    char Buffer[256];

    va_list Args;
    va_start(Args, Format);
    unsigned Count = FormatStringArgs(Buffer, sizeof(Buffer), Format, Args);
    va_end(Args);

    UART1Send(Buffer, Count);
}

static b32 ConsoleInit(void)
{
	b32 Result = UART1Init();

	ConsolePrintf("\nConsole initialised with UART1\n");

	return Result;
}
