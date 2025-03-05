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
	ConsoleInit();

	u32 BoardModel = 0;
	u64 SerialNumber = 0;
	u64 BoardMacAddress = 0;

	MboxGetBoardSerialNumber(&SerialNumber);
	MboxGetBoardModel(&BoardModel);
	MboxGetBoardMacAddress(&BoardMacAddress);

	BusyWait(100000);

	ConsolePrintf("Serial number is: 0x%X\n", SerialNumber);
	ConsolePrintf("Board model is: 0x%x\n", BoardModel);
	ConsolePrintf("Board mac address is: 0x%x\n", BoardMacAddress);

	ConsolePrintf("FB initialized %dx%d [%d pitch] [rgb=%d]\n", FBWidth, FBHeight, FBPitch, FBRGB);

	RNGInit();
	FBInit();
	BusyWait(100000);

	FillRectangle(0, 0, FBWidth-1, FBHeight-1, 0x00111111);

	if(!MboxSetClockRate(MBOX_CLOCK_ARM, 1400000000, 0))
	{
		ConsolePrintf("Failed to set ARM clock rate\n");
	}

	if(!MboxSetClockRate(MBOX_CLOCK_CORE, 500000000, 0))
	{
		ConsolePrintf("Failed to set CORE clock rate\n");
	}
	BusyWait(100000);

	char C;
	u32 X = 200;
	u32 Y = 200;
	int Size = 200;
	u32 State = 0;
	for(int Num = 0; ; Num++)
	{
		u64 Time1 = ReadCpuCounter();
		u64 Freq1 = ReadCpuFrequency();

		if(!ConsolePeek(&C))
		{
			C = 'd';
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

		FillRectangle(X, Y, X+Size, Y+Size, 0x00111111);
		if(C == 'w') Y -= 10;
		if(C == 's') Y += 10;
		if(C == 'a') X -= 10;
		if(C == 'd') X += 10;
		if(C == '+') Size += 10;
		if(C == '-') Size -= 10;
		FillRectangle(X, Y, X+Size, Y+Size, 0x00FF00FF);

		u64 Freq2 = ReadCpuFrequency();
		u64 Time2 = ReadCpuCounter();
		if(Freq2 == Freq1)
		{
			u64 Total = ((Time2 - Time1) * 1000) / Freq1;
			ConsolePrintf("Total cycles: %ums (Freq: %u)\n", Total, Freq1);
		}
	}
}
