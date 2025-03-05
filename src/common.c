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
				u32 Width = 0;
				b32 AlignLeft = 0;
				char PadChar = ' ';
				b32 InProgress = 1;

				do
				{
					char C2 = *(Format++);
					switch(C2)
					{
						case 0:
						{
							goto finish;
						} break;

						case '-':
						{
							AlignLeft = 1;
						} break;

						case '0':
						{
							if(!Width)
							{
								PadChar = '0';
							}
						} // fallthrough
						case '1':
						case '2':
						case '3':
						case '4':
						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
						{
							Width = (Width * 10) + (C2 - '0');
						} break;

						case 's':
						{
							const char* String = va_arg(Args, const char*);
							const char* StringAt = String;

							char C;
							while((C = *StringAt) != 0)
							{
								*(BufferAt++) = C;
								StringAt++;
							}

							u32 Written = (StringAt - String);
							if(AlignLeft)
							{
								if(Width > Written)
								{
									u32 Elapsed = Width - Written;
									while(Elapsed--)
									{
										*(BufferAt++) = ' ';
									}
								}
							}

							InProgress = 0;
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

							InProgress = 0;
						} break;

						case 'u':
						{
							u32 Value = va_arg(Args, u32);

							char Tmp[32];
							char* TmpAt = Tmp;
							*TmpAt = 0;

							do
							{
								u32 Num = Value / 10;
								u32 Rem = Value - Num * 10;
								*(++TmpAt) = Rem + '0';
								Value = Num;
							} while(Value);

							u32 Written = TmpAt - Tmp;
							u32 Elapsed = Width - Written;
							if(Width > Written)
							{
								if(!AlignLeft)
								{
									while(Elapsed--)
									{
										*(BufferAt++) = PadChar;
									}
								}
							}

							char C;
							while((C = *(TmpAt--)) != 0)
							{
								*(BufferAt++) = C;
							}

							if(Width > Written)
							{
								if(AlignLeft)
								{
									while(Elapsed--)
									{
										*(BufferAt++) = PadChar;
									}
								}
							}

							InProgress = 0;
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

							InProgress = 0;
						} break;

						case 'c':
						{
							int Value = va_arg(Args, int);
							*(BufferAt++) = (char) Value;
							InProgress = 0;
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
				}
				while(InProgress);
			} break;

			default:
			{
				*(BufferAt++) = C1;
			} break;
		}
	}

finish:
	*(BufferAt++) = 0;
	return (BufferAt - Buffer);
}
