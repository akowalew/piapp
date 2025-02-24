#include "platform.c"

#include "bcm2837.c"
#include "console.c"

int main(void)
{
	ConsoleInit();
	ConsolePrint("Hello world, type something:\r\n");

	while(1)
	{
		char C = ConsoleGetc();
		ConsolePrint("You have typed: ");
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
	}
}
