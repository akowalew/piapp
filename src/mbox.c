static volatile u32 Aligned(16) Mbox[36];

static b32 MboxInit(void)
{
    b32 Result = 1;

    return Result;
}

static b32 MboxCall(u8 Channel)
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

static b32 MboxGetBoardSerialNumber(u64* Value)
{
    u32 Result = 0;

    Mbox[0] = 8 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_GET_BOARD_SERIAL;
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

static b32 MboxGetBoardModel(u32* Value)
{
    u32 Result = 0;

    Mbox[0] = 7 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_GET_BOARD_MODEL;
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

static b32 MboxGetBoardMacAddress(u64* Value)
{
    b32 Result;

    Mbox[0] = 8 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_GET_BOARD_MAC_ADDRESS;
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

static b32 MboxGetTemperature(u32* Value)
{
    b32 Result;

    Mbox[0] = 8 * 4;
    Mbox[1] = MBOX_REQUEST;
    Mbox[2] = MBOX_TAG_GET_TEMPERATURE;
    Mbox[3] = 2 * 4;
    Mbox[4] = 0;
    Mbox[5] = 0;
    Mbox[6] = 0;
    Mbox[7] = MBOX_TAG_LAST;
    if(MboxCall(MBOX_CH_PROP))
    {
        *Value = Mbox[6];

        Result = 1;
    }

    return Result;
}
