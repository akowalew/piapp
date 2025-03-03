static void BusyWait(unsigned int Delay)
{
	while(Delay--)
	{
		// Do nothing
	}
}

static void BusyWaitMs(unsigned int Millis)
{
	u64 Time;
	u64 Start = GetSystemTimer();
	while((Time = GetSystemTimer()) - Start < Millis)
	{
		// Do nothing
	}
}

static void BusyWaitMsCpu(unsigned int Millis)
{
	register u64 Freq, Start, Time;
	asm volatile("mrs %0, cntfrq_el0" : "=r"(Freq));
	asm volatile("mrs %0, cntpct_el0" : "=r"(Start));

	u64 Counts = ((Freq / 1000) * Millis) / 1000;

	do
	{
		asm volatile("mrs %0, cntpct_el0" : "=r"(Time));
	}
	while((Time - Start) < Counts);
}

static unsigned
FormatStringArgs(char* Buffer, unsigned Length, const char* Format, va_list Args)
{
	char* BufferAt = Buffer;

	while(1)
	{
		char C1 = *(Format++);
		switch(C1)
		{
			case 0:
			{
				goto finish;
			} break;

			case '%':
			{
				char C2 = *(Format++);
				switch(C2)
				{
					case 0:
					{
						goto finish;
					} break;

					case 'd':
					{
						i32 Value = va_arg(Args, i32);
						i32 Negative = 0;
						if(Value < 0)
						{
							Negative = 1;
							Value = -Value;
						}

						char Tmp[32];
						char* TmpAt = Tmp;
						*(TmpAt++) = 0;
						do
						{
							u32 Num = Value / 10;
							u32 Rem = Value - Num * 10;
							*(TmpAt++) = Rem + '0';
							Value = Num;
						} while(Value);

						if(Negative)
						{
							*(BufferAt++) = '-';
						}

						char C;
						while((C = *(--TmpAt)) != 0)
						{
							*(BufferAt++) = C;
						}
					} break;

					case 'u':
					{
						u32 Value = va_arg(Args, u32);

						char Tmp[32];
						char* TmpAt = Tmp;
						*(TmpAt++) = 0;
						do
						{
							u32 Num = Value / 10;
							u32 Rem = Value - Num * 10;
							*(TmpAt++) = Rem + '0';
							Value = Num;
						} while(Value);

						char C;
						while((C = *(--TmpAt)) != 0)
						{
							*(BufferAt++) = C;
						}
					} break;

					case 'x':
					case 'X':
					{
						char Case = (C2 == 'x') ? 'a' : 'A';

						u64 Pos;
						u64 Value = va_arg(Args, u64);
						for(Pos = 64 - 4; Pos > 0; Pos -= 4)
						{
							u64 Mask = (0xFUL << Pos);
							if(Value & Mask)
							{
								break;
							}
						}

						while(1)
						{
							u64 Part = (Value >> Pos) & 0xF;
							*(BufferAt++) = (Part >= 10) ? (Part - 10 + Case) : (Part + '0');
							if(Pos == 0)
							{
								break;
							}

							Pos -= 4;
						}
					} break;

					case 'c':
					{
						int Value = va_arg(Args, int);
						*(BufferAt++) = (char) Value;
					} break;

					case '%':
					{
						*(BufferAt++) = C2;
					} break;

					default:
					{
						// TODO: Unknown modifier
					} break;
				}
			} break;

			default:
			{
				*(BufferAt++) = C1;
			} break;
		}
	}

finish:
	return (BufferAt - Buffer);
}
