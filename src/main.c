#include "platform.h"
#include "platform.c"
#include "bcm2835.h"
#include "bcm2835.c"
#include "common.h"
#include "common.c"
#include "mbox.h"
#include "mbox.c"
#include "gpio.h"
#include "gpio.c"
#include "uart0.h"
#include "uart0.c"
#include "uart1.h"
#include "uart1.c"
#include "console.h"
#include "console.c"
#include "led.h"
#include "led.c"
#include "sd.h"
#include "sd.c"
#include "fb.h"
#include "fb.c"

static void ConfigureGpioForAlt5JtagAndAlt4Trst(void)
{
	GpioSelectFunction(4, GPIO_FUNCTION_ALT5);
	GpioSelectFunction(5, GPIO_FUNCTION_ALT5);
	GpioSelectFunction(6, GPIO_FUNCTION_ALT5);
	GpioSelectFunction(12, GPIO_FUNCTION_ALT5);
	GpioSelectFunction(13, GPIO_FUNCTION_ALT5);
	GpioSelectFunction(22, GPIO_FUNCTION_ALT4);

	GpioControlPull(4, GPIO_PULL_OFF);
	GpioControlPull(5, GPIO_PULL_OFF);
	GpioControlPull(6, GPIO_PULL_OFF);
	GpioControlPull(12, GPIO_PULL_OFF);
	GpioControlPull(13, GPIO_PULL_OFF);
	GpioControlPull(22, GPIO_PULL_OFF);
}

static void PrintClocks(void)
{
	ConsolePrintf("--  Clock   --  Curr Rate  --   Measured  --     Maximum\n");
	ConsolePrintf("--------------------------------------------------------\n");
	for(u32 Clock = 1; Clock < 0xe; Clock++)
	{
		const char* ClockName = MboxClockNames[Clock];
		ConsolePrintf("#%x (%-5s):", Clock, ClockName);

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

int main(void)
{
	u32 Counter = 0;
	ConsoleInit();
	ConfigureGpioForAlt5JtagAndAlt4Trst();

	SdInit();
	FbInit();

	FillRectangle(0, 0, FbWidth-1, FbHeight-1, 0);
	FbSyncAndSwapBuffers();
	FillRectangle(0, 0, FbWidth-1, FbHeight-1, 0);
	FbSyncAndSwapBuffers();

	u32 Backlight = 0;
	i32 Delta = 111;
	u32 X = 0;
	u32 Y = 0;
	i32 Color = 127;
	i32 DeltaColor = 3;

	PrintClocks();

	u32 Time = 0;

	while(1)
	{
		u64 Begin = GetSystemTimer();

#if 1
		FbFill(0);
		FbFill(Color);
		Color += DeltaColor;
		if(Color > 255)
		{
			Color = 255;
			DeltaColor = -DeltaColor;
		}
		else if(Color < 0)
		{
			Color = 0;
			DeltaColor = -DeltaColor;
		}
#else
		NiceGradient(Time++);
#endif

		// ConsolePrintf("Hello world: %d\n", Counter++);
		// BusyWaitUs(1000000);

		// DispSetBacklight(Backlight);
		// Backlight += Delta;
		// if(Backlight == 999 || Backlight == 0) {
		// 	Delta = -Delta;
		// }

		u64 End = GetSystemTimer();
		u32 Diff = (u32)(End - Begin) / 1000;
		ConsolePrintf("Diff: %ums %ufps\n", Diff, (u32)(1000.f / Diff));

		FbSyncAndSwapBuffers();
	}
}
