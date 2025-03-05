static void ConsolePut(char C)
{
	UART0Put(C);
}

static b32 ConsolePeek(char* C)
{
    b32 Result = UART0Peek(C);

    return Result;
}

static char ConsoleGet(void)
{
    char Result = UART0Get();

    return Result;
}

static void ConsoleSend(const void* Data, unsigned int Size)
{
	UART0Send(Data, Size);
}

static void ConsolePrintf(const char* Format, ...)
{
#if 1
    char Buffer[256];

    va_list Args;
    va_start(Args, Format);
    unsigned Count = FormatStringArgs(Buffer, sizeof(Buffer), Format, Args);
    va_end(Args);

    UART0Send(Buffer, Count);
#endif
}

static b32 ConsoleInit(void)
{
	b32 Result = UART0Init();

	ConsolePrintf("\nConsole initialised with UART0\n");

	return Result;
}
