static void Reboot(void)
{
	*(volatile uint32_t*)PM_RSTC = PM_PASSWORD | 0x10; // Magic value + reset
    *(volatile uint32_t*)PM_WDOG = 0; // Disable watchdog

    *(volatile uint32_t*)PM_RSTC = PM_PASSWORD | PM_RSTC_WRCFG | PM_RSTC_FULLRST;
    *(volatile uint32_t*)PM_WDOG = PM_PASSWORD | 10; // Short timeout
}
