static void LedInit(void)
{
	GpioSelectFunction(LED_GPIO, GPIO_FUNCTION_OUTPUT);
}

static void LedOn(void)
{
	GpioSet(LED_GPIO);
}

static void LedOff(void)
{
	GpioClear(LED_GPIO);
}
