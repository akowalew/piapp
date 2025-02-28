static volatile u32 Aligned(16) Mbox[36];

static u32 MboxCall(u8 Channel)
{
    u32 Status;
    u32 Response;
    u32 ReadAddress;
    u32 WriteAddress = (((u64) &Mbox) & ~0xF) | (Channel & 0xF);

    while(MBOX->STATUS0 & MBOX_STATUS0_FULL_Msk)
    {
        // Do nothing
    }

    MBOX->WRITE = WriteAddress;

    do
    {
        while(MBOX->STATUS0 & MBOX_STATUS0_EMPTY_Msk)
        {
            // Do nothing
        }
    }
    while(MBOX->READ != WriteAddress);

    if(Mbox[1] == MBOX_RESPONSE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static u32 PiGetBoardSerialNumber(u64* Value)
{
    u32 Result = 0;

    Mbox[0] = 8 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_GETSERIAL;
    Mbox[3] = 8;
    Mbox[4] = 0;
    Mbox[5] = 0;
    Mbox[6] = 0;
    Mbox[7] = MBOX_TAG_LAST;
    Result = MboxCall(MBOX_CH_PROP);
    if(Result)
    {
        *Value = ((u64)Mbox[6] << 32) | Mbox[5];
    }

    return Result;
}

static b32 PiGetBoardModel(u32* Value)
{
    u32 Result = 0;

    Mbox[0] = 7 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_GETBOARDMODEL;
    Mbox[3] = 4;
    Mbox[4] = 0;
    Mbox[5] = 0;
    Mbox[6] = MBOX_TAG_LAST;
    Result = MboxCall(MBOX_CH_PROP);
    if(Result)
    {
        *Value = Mbox[5];
    }

    return Result;
}

static b32 PiGetBoardMacAddress(u64* Value)
{
    b32 Result;

    Mbox[0] = 8 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_GETBOARDMACADDRESS;
    Mbox[3] = 6;
    Mbox[4] = 0;
    Mbox[5] = 0;
    Mbox[6] = 0;
    Mbox[7] = MBOX_TAG_LAST;
    Result = MboxCall(MBOX_CH_PROP);
    if(Result)
    {
        *Value = ((u64)Mbox[6] << 32) | Mbox[5];
    }

    return Result;
}

static void PiReboot(void)
{
	*(volatile uint32_t*)WDOG_RST = PM_PASSWORD | 0x10; // Magic value + reset
    *(volatile uint32_t*)WDOG_CMD = 0; // Disable watchdog

    *(volatile uint32_t*)PM_RSTC = PM_PASSWORD | PM_RSTC_WRCFG | PM_RSTC_FULLRST;
    *(volatile uint32_t*)PM_WDOG = PM_PASSWORD | 10; // Short timeout
}
