static u64 GetSystemTimer(void)
{
	u64 Result;

	u32 Hi, Lo;

	do
	{
		Hi = SYSTMR->CHI;
		Lo = SYSTMR->CLO;
	}
	while(Hi != SYSTMR->CHI);

	Result = ((u64)Hi << 32) | Lo;

	return Result;
}
