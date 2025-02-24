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

#define WDT       ((volatile unsigned int*)(MMIO_BASE+0x600))
#define WDTTMO       ((volatile unsigned int*)(MMIO_BASE+0x604))
#define WDTCTL       ((volatile unsigned int*)(MMIO_BASE+0x608))

/* Auxilary mini UART registers */
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

#define WDOG_OFFSET     0x00100000
#define PM_OFFSET       0x00100000

#define WDOG_RST        (MMIO_BASE + WDOG_OFFSET + 0x1C)
#define WDOG_CMD        (MMIO_BASE + WDOG_OFFSET + 0x24)
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
