#include "platform.h"

#include "bcm2837_lpa.h"
#include "common.c"
#include "bcm2837.h"
#include "bcm2837.c"
#include "uart1.h"
#include "uart1.c"
#include "uart0.c"
#include "uart0.h"
#include "rng.h"
#include "rng.c"
#include "console.c"

int main(void)
{
	ConsoleInit();

	u64 SerialNumber = 0;
	if(PiGetBoardSerialNumber(&SerialNumber))
	{
		ConsolePrintf("Serial number is: 0x%X\n", SerialNumber);
	}
	else
	{
		ConsolePrintf("Failed to retrieve serial number\r\n");
	}

	u32 BoardModel = 0;
	if(PiGetBoardModel(&BoardModel))
	{
		ConsolePrintf("Board model is: 0x%x\n", BoardModel);
	}
	else
	{
		ConsolePrintf("Failed to retrieve board model\n");
	}

	u64 BoardMacAddress = 0;
	if(PiGetBoardMacAddress(&BoardMacAddress))
	{
		ConsolePrintf("Board mac address is: 0x%x\n", BoardMacAddress);
	}
	else
	{
		ConsolePrintf("Failed to retrieve board mac address\n");
	}

	RNGInit();

	for(int Num = 0; ; Num++)
	{
		u32 Random = RNGRead();
		ConsolePrintf("[%d] [%u] Type something: ", Num, Random);
		char C = ConsoleGet();
		ConsolePrintf("%c\r\n", C);
		if(C == 'r')
		{
			while(1)
			{
				ConsolePrintf("Rebooting...\r\n");
				BusyWait(100000);
				PiReboot();
			}
		}
	}
}
