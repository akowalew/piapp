static void RNGInit(void)
{
	*RNG_STATUS = 0x40000;
	*RNG_INT_MASK |= 1;
	*RNG_CTRL |= 1;
}

static u32 RNGRead(void)
{
	u32 Result = 0;

	while(!(*RNG_STATUS >> 24))
	{
		// Do nothing
	}

	Result = *RNG_DATA;

	return Result;
}
