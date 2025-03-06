#include "platform.h"
#include "platform.c"
#include "bcm2837.h"
#include "bcm2837.c"
#include "common.h"
#include "common.c"
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
	BusyWait(100000);
	if(!ConsoleInit())
	{
		while(1);
	}

	ConsolePrintf("Hello world\n");

	u32 BoardModel = 0;
	u64 SerialNumber = 0;
	u64 BoardMacAddress = 0;

	MboxGetBoardSerialNumber(&SerialNumber);
	MboxGetBoardModel(&BoardModel);
	MboxGetBoardMacAddress(&BoardMacAddress);

	ConsolePrintf("Serial number is: 0x%X\n", SerialNumber);
	ConsolePrintf("Board model is: 0x%x\n", BoardModel);
	ConsolePrintf("Board mac address is: 0x%x\n", BoardMacAddress);

	RNGInit();

	FBInit();
	ConsolePrintf("FB initialized %dx%d [%d pitch] [rgb=%d]\n", FBWidth, FBHeight, FBPitch, FBRGB);

	if(!MboxSetClockRate(MBOX_CLOCK_ARM, 1400000000, 0))
	{
		ConsolePrintf("Failed to set ARM clock rate\n");
	}

	if(!MboxSetClockRate(MBOX_CLOCK_CORE, 500000000, 0))
	{
		ConsolePrintf("Failed to set CORE clock rate\n");
	}

	u32 VirtualOffset = 0;
	FBSetVirtualOffset(0, VirtualOffset);
	FillRectangle(0, 0, FBWidth-1, FBHeight-1, 0x00000000);
	FBData = FBBuffer + (VirtualOffset ? 0 : FBHeight) * FBPitch;
	FillRectangle(0, 0, FBWidth-1, FBHeight-1, 0x00000000);

	char C = 0;
	u32 Color = 255;
	i32 Delta = -1;
	u32 X = 200;
	u32 Y = 200;
	int Size = 512;
	u32 State = 0;
	u64 Freq = ReadCpuFrequency();
	for(int Num = 0; ; Num++)
	{
		u64 Time1 = ReadCpuCounter();

		if(!ConsolePeek(&C))
		{
			C = 0;
		}

		if(C == 'r')
		{
			while(1)
			{
				ConsolePrintf("Rebooting...\r\n");
				BusyWait(100000);
				Reboot();
			}
		}

		if(C == 'n')
		{
			u32 Random = RNGRead();
			ConsolePrintf("Random: %u\n", Random);
		}

		if(C == 'T')
		{
			u32 Temperature = 0;
			Assert(MboxGetTemperature(&Temperature));
			ConsolePrintf("Temperature: %u^C\n", Temperature / 1000);
		}

		if(C == 'c')
		{
			ConsolePrintf("--  Clock   --  Curr Rate  --   Measured  --     Maximum\n");
			ConsolePrintf("--------------------------------------------------------\n");
			for(u32 Clock = 1; Clock < 0xe; Clock++)
			{
				ConsolePrintf("#%x (%-5s):", Clock, MboxClockNames[Clock]);

				u32 ClockRate = 0;
				u32 MaxClockRate = 0;
				u32 ClockRateMeasured = 0;
				MboxGetClockRate(Clock, &ClockRate);
				MboxGetMaxClockRate(Clock, &MaxClockRate);
				MboxGetClockRateMeasured(Clock, &ClockRateMeasured);

				ConsolePrintf(" -- %4u.%-03uMHz", ClockRate / 1000000, (ClockRate - (ClockRate / 1000000) * 1000000) / 1000);
				ConsolePrintf(" -- %4u.%-03uMHz", ClockRateMeasured / 1000000, (ClockRateMeasured - (ClockRateMeasured / 1000000) * 1000000) / 1000);
				ConsolePrintf(" -- %4u.%-03uMHz", MaxClockRate / 1000000, (MaxClockRate - (MaxClockRate / 1000000) * 1000000) / 1000);
				ConsolePrintf("\n");
			}
		}

		if(C == 'l')
		{
			MboxSetGpioState(MBOX_GPIO_STATUS_LED, State);
			MboxSetGpioState(MBOX_GPIO_POWER_LED, State);
			State = State ? 1 : 0;
		}

		u32 Box = 128;
		FillRectangle(0, 0, FBWidth-1, FBHeight-1, 0x00222222);
		FillRectangle(X, Y, X+Box-1, Y+Box-1, Color);
		Color += Delta;
		if(Color == 0)
		{
			Delta = 1;
		}
		else if(Color == 255)
		{
			Delta = -1;
		}

		if(C == 'w') Y -= 4;
		if(C == 's') Y += 4;
		if(C == 'a') X -= 4;
		if(C == 'd') X += 4;
		if(C == '+') Size += 16;
		if(C == '-') Size -= 16;

		u64 Time2 = ReadCpuCounter();
		if(!FBSetVirtualOffset(0, VirtualOffset ? 0 : FBHeight))
		{
			ConsolePrintf("Failed to set virtual offset\n");
		}

		u64 Time3 = ReadCpuCounter();
		MboxWaitForVerticalSync();
		FBData = FBBuffer + (VirtualOffset ? FBHeight : 0) * FBPitch;
		VirtualOffset = VirtualOffset ? 0 : FBHeight;

		u64 Time4 = ReadCpuCounter();
		u64 TimeLoop = ((Time4 - Time1) * 1000) / Freq;
		u64 TimeSwap = ((Time3 - Time1) * 1000) / Freq;
		u64 TimeRender = ((Time2 - Time1) * 1000) / Freq;
		u64 FPS = (1000 / TimeLoop);
		ConsolePrintf("Render %ums Swap %ums Loop %ums FPS %u\n", TimeRender, TimeSwap, TimeLoop, FPS);
	}
}
