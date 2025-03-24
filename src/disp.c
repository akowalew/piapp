static void DispSetBacklight(u32 Value)
{
	PWM0->DAT1 = Value;
}

static void DispResetOn(void)
{
	GpioClear(DISP_GPIO_RESET);
}

static void DispResetOff(void)
{
	GpioSet(DISP_GPIO_RESET);
}

static void DispDataMode(void)
{
	GpioSet(DISP_GPIO_LCD_D_CX);
}

static void DispCommandMode(void)
{
	GpioClear(DISP_GPIO_LCD_D_CX);
}

static void DispPut(u8 Data)
{
	while(!(SPI0->CS & SPI0_CS_TXD_Msk))
	{
		// Do nothing
	}

	SPI0->FIFO = Data;

	while(!(SPI0->CS & SPI0_CS_RXD_Msk))
	{
		// Do nothing
	}

	Data = SPI0->FIFO;
}

static void DispSelectChip(void)
{
	SPI0->CS = SPI0_CS_TA_Msk;
}

static void DispDeselectChip(void)
{
	SPI0->CS = 0;
}

static void DispWaitForFinish(void)
{
	// BusyWaitUs(10000);
	while(!(SPI0->CS & SPI0_CS_DONE_Msk))
	{
		// Do nothing
	}
}

static void DispSend0(u8 Command)
{
	DispCommandMode();
	DispPut(Command);
}

static void DispSend1(u8 Command, u8 Data1)
{
	DispCommandMode();
	DispPut(Command);
	DispDataMode();
	DispPut(Data1);
}

static void DispSend2(u8 Command, u8 Data1, u8 Data2)
{
	DispCommandMode();
	DispPut(Command);
	DispDataMode();
	DispPut(Data1);
	DispPut(Data2);
}

static void DispSend3(u8 Command, u8 Data1, u8 Data2, u8 Data3)
{
	DispCommandMode();
	DispPut(Command);
	DispDataMode();
	DispPut(Data1);
	DispPut(Data2);
	DispPut(Data3);
}

static void DispSend4(u8 Command, u8 Data1, u8 Data2, u8 Data3, u8 Data4)
{
	DispCommandMode();
	DispPut(Command);
	DispDataMode();
	DispPut(Data1);
	DispPut(Data2);
	DispPut(Data3);
	DispPut(Data4);
}

static void DispSend6(u8 Command, u8 Data1, u8 Data2, u8 Data3, u8 Data4, u8 Data5, u8 Data6)
{
	DispCommandMode();
	DispPut(Command);
	DispDataMode();
	DispPut(Data1);
	DispPut(Data2);
	DispPut(Data3);
	DispPut(Data4);
	DispPut(Data5);
	DispPut(Data6);
}

static void DispSendN(u8 Command, u8* Data, u32 Length)
{
	DispCommandMode();
	DispPut(Command);
	DispDataMode();
	for(u32 Idx = 0; Idx < Length; Idx++)
	{
		DispPut(Data[Idx]);
	}
}

#define DISP_TRANSFER(Cmd, ...) do { u8 _Array[] = {__VA_ARGS__}; DispSendN(Cmd, _Array, ArrayCount(_Array)); }while(0)

static b32 DispInit(void)
{
	b32 Result = 0;

	GpioSelectFunction(DISP_GPIO_TP_CS, GPIO_FUNCTION_ALT0);
	GpioSelectFunction(DISP_GPIO_LCD_CS, GPIO_FUNCTION_ALT0);
	GpioSelectFunction(DISP_GPIO_LCD_MISO, GPIO_FUNCTION_ALT0);
	GpioSelectFunction(DISP_GPIO_LCD_MOSI, GPIO_FUNCTION_ALT0);
	GpioSelectFunction(DISP_GPIO_LCD_SCLK, GPIO_FUNCTION_ALT0);
	GpioSelectFunction(DISP_GPIO_LCD_D_CX, GPIO_FUNCTION_OUTPUT);
	GpioSelectFunction(DISP_GPIO_RESET, GPIO_FUNCTION_OUTPUT);
	GpioSelectFunction(DISP_GPIO_TP_IRQ, GPIO_FUNCTION_INPUT);

#if 0
	// GpioSelectFunction(DISP_GPIO_BACKLIGHT, GPIO_FUNCTION_OUTPUT);
#else
	GpioSelectFunction(DISP_GPIO_BACKLIGHT, GPIO_FUNCTION_ALT5);
#endif

	GpioControlPull(DISP_GPIO_TP_CS, GPIO_PULL_OFF);
	GpioControlPull(DISP_GPIO_LCD_CS, GPIO_PULL_OFF);
	GpioControlPull(DISP_GPIO_LCD_MISO, GPIO_PULL_OFF);
	GpioControlPull(DISP_GPIO_LCD_MOSI, GPIO_PULL_OFF);
	GpioControlPull(DISP_GPIO_LCD_SCLK, GPIO_PULL_OFF);
	GpioControlPull(DISP_GPIO_LCD_D_CX, GPIO_PULL_OFF);
	GpioControlPull(DISP_GPIO_RESET, GPIO_PULL_OFF);
	GpioControlPull(DISP_GPIO_TP_IRQ, GPIO_PULL_UP);
	GpioControlPull(DISP_GPIO_BACKLIGHT, GPIO_PULL_OFF);

	AUX->ENABLES |= 2;
	SPI0->CLK = 34;

#if 0
	GpioSet(DISP_GPIO_BACKLIGHT);
	Result = 1;
#else
	if(MboxSetClockRate(MBOX_CLOCK_PWM, 100000000, 0))
	{
		PWM0->CTL = PWM0_CTL_PWEN1_Msk|PWM0_CTL_RPTL1_Msk;
		PWM0->RNG1 = 1000 - 1;
		PWM0->DAT1 = 500 - 1;
		Result = 1;
	}
#endif

	DispResetOff();
	BusyWaitUs(120000);
	DispResetOn();
	BusyWaitUs(120000);
	DispResetOff();
	BusyWaitUs(120000);

	DispSelectChip();

	DispSend0(DISP_CMD_SOFT_RESET);
	BusyWaitUs(120000);
	DispSend1(DISP_CMD_INTERFACE_MODE_CONTROL, 0x00);
	DispSend0(DISP_CMD_SLEEP_OUT);
	BusyWaitUs(120000);

	DISP_TRANSFER(DISP_CMD_SET_PIXEL_FORMAT, 0x55);
	DISP_TRANSFER(DISP_CMD_MEMORY_ACCESS_CONTROL, 0X48);
	DISP_TRANSFER(DISP_CMD_FRAME_RATE_CONTROL, 0xB0, 0x11);
	DISP_TRANSFER(DISP_CMD_INTERFACE_MODE_CONTROL, 0X00);
	DISP_TRANSFER(0xb1, 0Xa0);
	DISP_TRANSFER(0xb4, 0x02);
	DISP_TRANSFER(0xb6, 0x02, 0x02, 0x3b);
	DISP_TRANSFER(0xe9, 0x00);
	// DISP_TRANSFER(0xf7, 0xa9, 0x51, 0x2c, 0x82);
	DISP_TRANSFER(DISP_CMD_POWER_CONTROL1, 0X17, 0X15);
	DISP_TRANSFER(DISP_CMD_POWER_CONTROL2, 0XC1);
	DISP_TRANSFER(DISP_CMD_POWER_CONTROL3, 0X00, 0x12, 0x80);
 	DISP_TRANSFER(DISP_CMD_VCOM_CONTROL1, 0x00);
 	DISP_TRANSFER(0x36, 0x00);
	DispSend0(DISP_CMD_SLEEP_OUT);
	BusyWaitUs(120000);
	DISP_TRANSFER(DISP_CMD_NORMAL_DISPLAY_MODE_ON);
	DISP_TRANSFER(DISP_CMD_DISPLAY_ON);
	DISP_TRANSFER(0x2a, 0, 100, 0, 200);
	DISP_TRANSFER(0x2b, 0, 100, 0, 200);

	u8 Data[] = {
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
		0x0f, 0x0f, 0x0f, 0x0f, 0x0f, 0x0f,
	};
	DispSendN(DISP_CMD_MEMORY_WRITE, Data, ArrayCount(Data));

	DispWaitForFinish();
	DispDeselectChip();

	return Result;
}
