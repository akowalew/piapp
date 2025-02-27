#include "platform.c"

#include "common.c"
#include "bcm2837.c"
#include "console.c"

int main(void)
{
	int Num = 0;

	ConsoleInit();
	ConsolePrint("Hello world\r\n");

	u64 SerialNumber = 0;
	if(PiReadSerialNumber(&SerialNumber))
	{
		ConsolePrintf("Serial number is: 0x%X\n", SerialNumber);
	}
	else
	{
		ConsolePrintf("Failed to retrieve serial number\r\n");
	}

	while(1)
	{
		ConsolePrintf("[%d] Type something: ", Num);
		char C = ConsoleGetc();
		ConsolePrintf("%c\r\n", C);
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
