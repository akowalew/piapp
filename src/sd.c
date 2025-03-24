
static u32 SdVersion, SdCcs;

static b32 SdSetClock(u32 Clock)
{
	u32 Counter;

	Counter = 100000;
	while(EMMC->STATUS & (EMMC_STATUS_CMD_INHIBIT_Msk | EMMC_STATUS_DAT_INHIBIT_Msk))
	{
		Counter--;
		if(Counter == 0)
		{
			// TODO: Logging
			return 0;
		}

		BusyWaitMs(1);
	}

	EMMC->CONTROL1 &= ~EMMC_CONTROL1_CLK_EN_Msk;

	u32 Shift = 32;
	u32 X = Clock - 1;
	if(!X) { Shift = 0; }
	else if(!(X & 0xffff0000u)) { X <<= 16; Shift -= 16; }
	else if(!(X & 0xff000000u)) { X <<= 8;  Shift -= 8; }
	else if(!(X & 0xf0000000u)) { X <<= 4;  Shift -= 4; }
	else if(!(X & 0xc0000000u)) { X <<= 2;  Shift -= 2; }
	else if(!(X & 0x80000000u)) { X <<= 1;  Shift -= 1; }
	else if(Shift > 0) { Shift--; }
	else if(Shift > 7) { Shift = 7; }

	u32 Divisor = (SdVersion > 1) ? Clock : (u32)(1 << Shift);
	if(Divisor <= 2)
	{
		Divisor = 2;
		Shift = 0;
	}

	u32 H = (SdVersion > 1) ? ((Divisor & 0x300) >> 2) : 0;
	Divisor = ((Divisor & 0x0FF) << 8) | H;

	EMMC->CONTROL1 = (EMMC->CONTROL1 & 0xFFFF003F) | Divisor;
	BusyWaitMs(10);

	EMMC->CONTROL1 |= EMMC_CONTROL1_CLK_EN_Msk;
	BusyWaitMs(10);

	Counter = 10000;
	while(!(EMMC->CONTROL1 & EMMC_CONTROL1_CLK_STABLE_Msk))
	{
		Counter--;
		if(Counter == 0)
		{
			// TODO: Logging
			return 0;
		}
	}

	return 1;
}

#define SdSetFrequency(F) SdSetClock(41666666/(F))

#define INT_ERROR_MASK      0x017E8000

static b32 SdPollStatus(u8 Mask)
{
	for(u32 Counter = 500000; Counter > 0; Counter--)
	{
		u32 Match = (EMMC->STATUS & Mask);
		u32 Error = (EMMC->INTERRUPT & INT_ERROR_MASK);
		if(!Match || Error)
		{
			if(Error)
			{
				// TODO: Logging
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}

	// TODO: Logging
	return 0;
}

static b32 SdPollInterrupt(u32 Mask)
{
	for(u32 Counter = 1000000; Counter > 0; Counter--)
	{
		u32 R = EMMC->INTERRUPT;
		if(R & EMMC_INTERRUPT_CTO_ERR_Msk)
		{
			// TODO: Logging
			EMMC->INTERRUPT = R;
			return 0;
		}
		else if(R & EMMC_INTERRUPT_DTO_ERR_Msk)
		{
			// TODO: Logging
			EMMC->INTERRUPT = R;
			return 0;
		}
		else if(R & INT_ERROR_MASK)
		{
			// TODO: Logging
			EMMC->INTERRUPT = R;
			return 0;
		}
		else if(R & Mask)
		{
			EMMC->INTERRUPT = Mask;
			return 1;
		}
	}

	// TODO: Logging
	return 0;
}

static b32 SdCmd(u32 Cmd, u32 Arg, u32* Rsp)
{
	if(!SdPollStatus(EMMC_STATUS_CMD_INHIBIT_Msk))
	{
		// TODO: Logging
		return 0;
	}

	EMMC->INTERRUPT = EMMC->INTERRUPT;
	EMMC->ARG1 = Arg;
	EMMC->CMDTM = Cmd;

	if(!SdPollInterrupt(EMMC_INTERRUPT_CMD_DONE_Msk))
	{
		// TODO: Logging
		return 0;
	}

	*Rsp = EMMC->RESP0;

	return 1;
}

static b32 SdGoIdle(u32 Arg, u32* Rsp)
{
	return SdCmd(SD_CMD_GO_IDLE, Arg, Rsp);
}

static b32 SdSendInterfaceCondition(u32 Arg, u32* Rsp)
{
	if(!SdPollStatus(EMMC_STATUS_CMD_INHIBIT_Msk))
	{
		// TODO: Logging
		return 0;
	}

	EMMC->INTERRUPT = EMMC->INTERRUPT;
	EMMC->ARG1 = Arg;
	EMMC->CMDTM = SD_CMD_SEND_IF_COND;

	BusyWaitMs(100);

	if(!SdPollInterrupt(EMMC_INTERRUPT_CMD_DONE_Msk))
	{
		// TODO: Logging
		return 0;
	}

	*Rsp = EMMC->RESP0;

	if(*Rsp == Arg)
	{
		return 1;
	}
	else
	{
		// TODO: Logging
		return 0;
	}
}

static b32 SdSendOpCond(u32 Arg, u32* Rsp)
{
	if(!SdPollStatus(EMMC_STATUS_CMD_INHIBIT_Msk))
	{
		// TODO: Logging
		return 0;
	}

	EMMC->INTERRUPT = EMMC->INTERRUPT;
	EMMC->ARG1 = Arg;
	EMMC->CMDTM = SD_CMD_SEND_OP_COND;
	BusyWaitMs(1000);

	if(!SdPollInterrupt(EMMC_INTERRUPT_CMD_DONE_Msk))
	{
		// TODO: Logging
		return 0;
	}

	*Rsp = EMMC->RESP0;

	return 1;
}

static b32 SdAllSendCid(u32 Arg, u32* Rsp)
{
	b32 Result = SdCmd(SD_CMD_ALL_SEND_CID, Arg, Rsp);
	if(Result)
	{
		*Rsp |= EMMC->RESP3 | EMMC->RESP2 | EMMC->RESP1;
	}

	return Result;
}

static b32 SdInit(void)
{
	GpioSelectFunction(SD_GPIO_CD, GPIO_FUNCTION_INPUT);
	GpioSelectFunction(SD_GPIO_CLK, GPIO_FUNCTION_ALT3);
	GpioSelectFunction(SD_GPIO_CMD, GPIO_FUNCTION_ALT3);
	GpioSelectFunction(SD_GPIO_DAT0, GPIO_FUNCTION_ALT3);
	GpioSelectFunction(SD_GPIO_DAT1, GPIO_FUNCTION_ALT3);
	GpioSelectFunction(SD_GPIO_DAT2, GPIO_FUNCTION_ALT3);
	GpioSelectFunction(SD_GPIO_DAT3, GPIO_FUNCTION_ALT3);

	GpioControlPull(SD_GPIO_CD, GPIO_PULL_UP);
	GpioControlPull(SD_GPIO_CLK, GPIO_PULL_UP);
	GpioControlPull(SD_GPIO_CMD, GPIO_PULL_UP);
	GpioControlPull(SD_GPIO_DAT0, GPIO_PULL_UP);
	GpioControlPull(SD_GPIO_DAT1, GPIO_PULL_UP);
	GpioControlPull(SD_GPIO_DAT2, GPIO_PULL_UP);
	GpioControlPull(SD_GPIO_DAT3, GPIO_PULL_UP);

	(&GPIO->GPHEN0)[SD_GPIO_CD/32] |= ((SD_GPIO_CD-32) << 15);

	SdVersion = (EMMC->SLOTISR_VER & EMMC_SLOTISR_VER_SDVERSION_Msk) >> EMMC_SLOTISR_VER_SDVERSION_Pos;

	EMMC->CONTROL0 = 0;
	EMMC->CONTROL1 |= EMMC_CONTROL1_SRST_HC_Msk;

	u32 Counter = 10000;
	while(EMMC->CONTROL1 & EMMC_CONTROL1_SRST_HC_Msk)
	{
		Counter--;
		if(!Counter)
		{
			// TODO: Logging
			return 0;
		}

		BusyWaitMs(10);
	}

	EMMC->CONTROL1 |=
		EMMC_CONTROL1_CLK_INTLEN_Msk |
		(0xE << EMMC_CONTROL1_DATA_TOUNIT_Pos);

	BusyWaitMs(10);

	if(!SdSetFrequency(400000))
	{
		// TODO: Logging
		return 0;
	}

	EMMC->IRPT_EN = 0xFFFFFFFF;
	EMMC->IRPT_MASK = 0xFFFFFFFF;

	u32 Rsp;

	Rsp = 0;
	if(!SdCmd(SD_CMD0, 0x0, &Rsp))
	{
		// TODO: Logging
		return 0;
	}

	Rsp = 0;
	if(!SdCmd(SD_CMD8, 0x1AA, &Rsp))
	{
		// TODO: Logging
		return 0;
	}

	if(Rsp != 0x1AA)
	{
		// TODO: Not an SDHC card
		return 0;
	}

	Counter = 8;
	while(1)
	{
		Rsp = 0;
		if(!SdCmd(SD_CMD55, 0, &Rsp))
		{
			// TODO: Logging
			return 0;
		}

		Rsp = 0;
		if(!SdCmd(SD_ACMD41, 0x51ff8000, &Rsp))
		{
			// TODO: Logging
			return 0;
		}

		u32 Mask = EMMC->RESP0;
		Mask = Mask & 0x80000000;
		if(Mask)
		{
			break;
		}

		Counter--;
		if(!Counter)
		{
			// TODO: Logging
			return 0;
		}
	}

	Rsp = 0;
	if(!SdCmd(SD_CMD2, 0, &Rsp))
	{
		// TODO: Logging
		return 0;
	}

	u32 CID0 = EMMC->RESP0;
	u32 CID1 = EMMC->RESP1;
	u32 CID2 = EMMC->RESP2;
	u32 CID3 = EMMC->RESP3;

	return 1;
}
