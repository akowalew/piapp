#include "platform.c"

#include "common.c"
#include "bcm2837.c"
#include "console.c"

int main(void)
{
	int Num = 0;
	ConsoleInit();
	ConsolePrint("Hello world, type something:\r\n");
	while(1)
	{
		ConsolePrintf("[%d] Type something: ", Num);
		char C = ConsoleGetc();
		ConsolePut(C);
		ConsolePut('\r');
		ConsolePut('\n');
		if(C == 'r')
		{
			while(1)
			{
				ConsolePrint("Rebooting...\r\n");
				BusyWait(100000);
				PiReboot();
			}
		}

		Num++;
	}
}
