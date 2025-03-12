static void ConsolePut(char C)
{
	UART1Put(C);
}

static b32 ConsolePeek(char* C)
{
    b32 Result = UART1Peek(C);

    return Result;
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
#if 1
    char Buffer[256];

    va_list Args;
    va_start(Args, Format);
    unsigned Count = FormatStringArgs(Buffer, sizeof(Buffer), Format, Args);
    va_end(Args);

    UART1Send(Buffer, Count);
#endif
}

#define ConsolePutz(String) ConsoleSend(String, sizeof(String)-1)

static b32 ConsoleInit(void)
{
	b32 Result = 0;

    if(UART1Init())
    {
        Result = 1;

        ConsolePutz("\nConsole UART1 initialized\n");
    }
    else
    {
    	ConsolePutz("\nConsole UART1 failed to init?\n");
    }

	return Result;
}
