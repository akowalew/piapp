#include "platform.h"
#include "platform.c"
#include "common.h"
#include "common.c"
#include "bcm2837.h"
#include "bcm2837.c"
#include "mbox.h"
#include "mbox.c"
#include "gpio.h"
#include "gpio.c"
#include "uart1.h"
#include "uart1.c"
#include "uart0.c"
#include "uart0.h"
#include "rng.h"
#include "rng.c"
#include "pm.h"
#include "pm.c"
#include "console.h"
#include "console.c"
#include "fb.h"
#include "fb.c"

int main(void)
{
	ConsoleInit();

	u64 SerialNumber = 0;
	if(MboxGetBoardSerialNumber(&SerialNumber))
	{
		ConsolePrintf("Serial number is: 0x%X\n", SerialNumber);
	}
	else
	{
		ConsolePrintf("Failed to retrieve serial number\r\n");
	}

	u32 BoardModel = 0;
	if(MboxGetBoardModel(&BoardModel))
	{
		ConsolePrintf("Board model is: 0x%x\n", BoardModel);
	}
	else
	{
		ConsolePrintf("Failed to retrieve board model\n");
	}

	u64 BoardMacAddress = 0;
	if(MboxGetBoardMacAddress(&BoardMacAddress))
	{
		ConsolePrintf("Board mac address is: 0x%x\n", BoardMacAddress);
	}
	else
	{
		ConsolePrintf("Failed to retrieve board mac address\n");
	}

	if(RNGInit())
	{
		ConsolePrintf("RNG initialized\n");
	}
	else
	{
		ConsolePrintf("RNG NOT initialized\n");
	}

	if(FBInit())
	{
		ConsolePrintf("FB initialized %dx%d [%d pitch] [rgb=%d]\n", FBWidth, FBHeight, FBPitch, FBRGB);
	}
	else
	{
		ConsolePrintf("FB NOT initialized\n");
	}

	// FBTest();
	FillRectangle(0, 0, FBWidth-1, FBHeight-1, 0x00111111);

	u32 X = 200;
	u32 Y = 200;
	for(int Num = 0; ; Num++)
	{
		u32 Random = RNGRead();
		u32 Temperature = 0;
		Assert(MboxGetTemperature(&Temperature));
		ConsolePrintf("[%d] [%u] [%u^C] Type something: ", Num, Random, Temperature);
		char C = ConsoleGet();
		ConsolePrintf("%c\r\n", C);
		FillRectangle(X, Y, X+50, Y+50, 0x00111111);
		if(C == 'w') Y -= 10;
		if(C == 's') Y += 10;
		if(C == 'a') X -= 10;
		if(C == 'd') X += 10;
		FillRectangle(X, Y, X+50, Y+50, 0x00FF00FF);
		if(C == 'r')
		{
			while(1)
			{
				ConsolePrintf("Rebooting...\r\n");
				BusyWait(100000);
				Reboot();
			}
		}
	}
}
