#define MMIO_BASE       0x3F000000

#define RNG_CTRL        ((volatile unsigned int*)(MMIO_BASE+0x00104000))
#define RNG_STATUS      ((volatile unsigned int*)(MMIO_BASE+0x00104004))
#define RNG_DATA        ((volatile unsigned int*)(MMIO_BASE+0x00104008))
#define RNG_INT_MASK    ((volatile unsigned int*)(MMIO_BASE+0x00104010))

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

#define MBOX_TAG_GETBOARDMODEL      0x10001
#define MBOX_TAG_GETBOARDREVISION   0x10002
#define MBOX_TAG_GETBOARDMACADDRESS 0x10003
#define MBOX_TAG_GETSERIAL          0x10004

#define MBOX_TAG_GETCLOCKSTATE          0x00030001
#define MBOX_TAG_GETCLOCKRATE           0x00030002
#define MBOX_TAG_SETCLOCKSTATE          0x00038001
#define MBOX_TAG_SETCLOCKRATE           0x00038002

#define MBOX_TAG_LAST           0

#define MBOX_CLOCK_reserved     0x000000000
#define MBOX_CLOCK_EMMC         0x000000001
#define MBOX_CLOCK_UART         0x000000002
#define MBOX_CLOCK_ARM          0x000000003
#define MBOX_CLOCK_CORE         0x000000004
#define MBOX_CLOCK_V3D          0x000000005
#define MBOX_CLOCK_H264         0x000000006
#define MBOX_CLOCK_ISP          0x000000007
#define MBOX_CLOCK_SDRAM        0x000000008
#define MBOX_CLOCK_PIXEL        0x000000009
#define MBOX_CLOCK_PWM          0x00000000a
#define MBOX_CLOCK_HEVC         0x00000000b
#define MBOX_CLOCK_EMMC2        0x00000000c
#define MBOX_CLOCK_M2MC         0x00000000d
#define MBOX_CLOCK_PIXEL_BVB    0x00000000e

#define WDOG_OFFSET     0x00100000
#define WDOG_RST        (MMIO_BASE + WDOG_OFFSET + 0x1C)
#define WDOG_CMD        (MMIO_BASE + WDOG_OFFSET + 0x24)

#define PM_OFFSET       0x00100000
#define PM_RSTC         (MMIO_BASE + PM_OFFSET + 0x1C)
#define PM_WDOG         (MMIO_BASE + PM_OFFSET + 0x24)
#define PM_PASSWORD     0x5A000000
#define PM_RSTC_WRCFG   0x00000020
#define PM_RSTC_FULLRST 0x00000010
