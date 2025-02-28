static b32 RNGInit(void)
{
	b32 Result = 1;

	*RNG_STATUS = 0x40000;
	*RNG_INT_MASK |= 1;
	*RNG_CTRL |= 1;

	return Result;
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
