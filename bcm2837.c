#define MMIO_BASE       0x3F000000

/* GPIO registers */
#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))
#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))
#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034))
#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038))
#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040))
#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044))
#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064))
#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068))
#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))

#define AUX_ENABLE      ((volatile unsigned int*)(MMIO_BASE+0x00215004))
#define AUX_MU_IO       ((volatile unsigned int*)(MMIO_BASE+0x00215040))
#define AUX_MU_IER      ((volatile unsigned int*)(MMIO_BASE+0x00215044))
#define AUX_MU_IIR      ((volatile unsigned int*)(MMIO_BASE+0x00215048))
#define AUX_MU_LCR      ((volatile unsigned int*)(MMIO_BASE+0x0021504C))
#define AUX_MU_MCR      ((volatile unsigned int*)(MMIO_BASE+0x00215050))
#define AUX_MU_LSR      ((volatile unsigned int*)(MMIO_BASE+0x00215054))
#define AUX_MU_MSR      ((volatile unsigned int*)(MMIO_BASE+0x00215058))
#define AUX_MU_SCRATCH  ((volatile unsigned int*)(MMIO_BASE+0x0021505C))
#define AUX_MU_CNTL     ((volatile unsigned int*)(MMIO_BASE+0x00215060))
#define AUX_MU_STAT     ((volatile unsigned int*)(MMIO_BASE+0x00215064))
#define AUX_MU_BAUD     ((volatile unsigned int*)(MMIO_BASE+0x00215068))

static void ConsoleInit(void)
{
    unsigned int Reg;

    *AUX_ENABLE |= 1; // Enable UART1, AUX mini uart
    *AUX_MU_CNTL = 0;
    *AUX_MU_LCR = 3; // 8 bits
    *AUX_MU_MCR = 0;
    *AUX_MU_IER = 0;
    // *AUX_MU_IIR = 0xC6; // Disable interrupts
    *AUX_MU_BAUD = 270; // 115200 baud

    Reg = *GPFSEL1;
    Reg &= ~((0x7 << 12) | (0x7 << 15)); // GPIO14, GPIO15
    Reg |= (0x2 << 12) | (0x2 << 15); // Alt5
    *GPFSEL1 = Reg;

    *GPPUD = 0; // Enable pins 14 and 15
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    BusyWait(150);
    *GPPUDCLK0 = 0; // Flush GPIO setup

    *AUX_MU_CNTL = 3; // Enable TX and RX
}

static void ConsolePut(char C)
{
    do
    {
        asm volatile("nop");
    } while(!(*AUX_MU_LSR & 0x20));

    *AUX_MU_IO = C;
}

static void ConsoleSend(const void* Data, unsigned int Size)
{
    const char* At = Data;
    const char* End = At + Size;
    while(At != End)
    {
        do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x20));

        *AUX_MU_IO = *(At++);
    }
}

static void ConsolePrint(const char* String)
{
    char C;
    const char* At = String;
    while((C = *(At++)) != 0)
    {
        do
        {
            asm volatile("nop");
        } while(!(*AUX_MU_LSR & 0x20));

        *AUX_MU_IO = C;
    }
}

static void ConsolePrintf(const char* Format, ...)
{
    char Buffer[256];

    va_list Args;
    va_start(Args, Format);
    unsigned Count = FormatStringArgs(Buffer, sizeof(Buffer), Format, Args);
    va_end(Args);

    ConsoleSend(Buffer, Count);
}

static char ConsoleGetc(void)
{
    char Result = 0;

    do{asm volatile("nop");}while(!(*AUX_MU_LSR&0x01));

    Result = *AUX_MU_IO;

    return Result;
}

#define VIDEOCORE_MBOX  (MMIO_BASE+0x0000B880)
#define MBOX_READ       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x0))
#define MBOX_POLL       ((volatile unsigned int*)(VIDEOCORE_MBOX+0x10))
#define MBOX_SENDER     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x14))
#define MBOX_STATUS     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x18))
#define MBOX_CONFIG     ((volatile unsigned int*)(VIDEOCORE_MBOX+0x1C))
#define MBOX_WRITE      ((volatile unsigned int*)(VIDEOCORE_MBOX+0x20))
#define MBOX_RESPONSE   0x80000000
#define MBOX_FULL       0x80000000
#define MBOX_EMPTY      0x40000000

#define MBOX_REQUEST    0

#define MBOX_CH_POWER   0
#define MBOX_CH_FB      1
#define MBOX_CH_VUART   2
#define MBOX_CH_VCHIQ   3
#define MBOX_CH_LEDS    4
#define MBOX_CH_BTNS    5
#define MBOX_CH_TOUCH   6
#define MBOX_CH_COUNT   7
#define MBOX_CH_PROP    8

#define MBOX_TAG_GETSERIAL      0x10004
#define MBOX_TAG_LAST           0

volatile unsigned int __attribute__((aligned(16))) Mbox[36];

static u32 MboxCall(u8 Channel)
{
    u32 Status;
    u32 Response;
    u32 ReadAddress;
    u32 WriteAddress = (((u64) &Mbox) & ~0xF) | (Channel & 0xF);

    do
    {
        asm volatile("nop");
        Status = *MBOX_STATUS;
    } while(Status & MBOX_FULL);

    asm volatile ("dc civac, %0" :: "r" (Mbox) : "memory");
    asm volatile ("dsb sy");
    asm volatile ("isb");

    *MBOX_WRITE = WriteAddress;


    do
    {
        do
        {
            asm volatile("nop");
            Status = *MBOX_STATUS;
        } while(Status & MBOX_EMPTY);

        ReadAddress = *MBOX_READ;
    }
    while((ReadAddress & 0xF) != Channel);
    ConsolePrintf("Response = 0x%x\r\n", Mbox[1]);
    ConsolePrintf("Response = 0x%x\r\n", Mbox[1]);
    ConsolePrintf("Response = 0x%x\r\n", Mbox[1]);
    if(Mbox[1] == MBOX_RESPONSE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

struct msg {
    uint32_t size;    // Total size in bytes
    uint32_t req_resp; // Request indicator (0x00000000)
    uint32_t tag_id;  // Tag identifier
    uint32_t buf_size; // Buffer size in bytes
    uint32_t req_code; // Request code (0x00000000)
    uint32_t value;    // Value to set/get
    uint32_t end_tag;  // End tag (0x00000000)
} __attribute__((aligned(16)));

static u32 PiReadSerialNumber(u64* Value)
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

#define WDOG_OFFSET     0x00100000
#define WDOG_RST        (MMIO_BASE + WDOG_OFFSET + 0x1C)
#define WDOG_CMD        (MMIO_BASE + WDOG_OFFSET + 0x24)

#define PM_OFFSET       0x00100000
#define PM_RSTC         (MMIO_BASE + PM_OFFSET + 0x1C)
#define PM_WDOG         (MMIO_BASE + PM_OFFSET + 0x24)
#define PM_PASSWORD     0x5A000000
#define PM_RSTC_WRCFG   0x00000020
#define PM_RSTC_FULLRST 0x00000010

static void PiReboot(void)
{
    // Disable watchdog timer
	*(volatile uint32_t*)WDOG_RST = PM_PASSWORD | 0x10; // Magic value + reset
    *(volatile uint32_t*)WDOG_CMD = 0; // Disable watchdog

    // Trigger a full reset
    *(volatile uint32_t*)PM_RSTC = PM_PASSWORD | PM_RSTC_WRCFG | PM_RSTC_FULLRST;
    *(volatile uint32_t*)PM_WDOG = PM_PASSWORD | 10; // Short timeout
    asm volatile("dsb 0xf");
    asm volatile("isb 0xf");
}

