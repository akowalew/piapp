#define GPIO_FUNCTION_INPUT  0
#define GPIO_FUNCTION_OUTPUT 1
#define GPIO_FUNCTION_ALT0   4
#define GPIO_FUNCTION_ALT1   5
#define GPIO_FUNCTION_ALT2   6
#define GPIO_FUNCTION_ALT3   7
#define GPIO_FUNCTION_ALT4   3
#define GPIO_FUNCTION_ALT5   2

#define GPIO_PULL_OFF 0
#define GPIO_PULL_DOWN 1
#define GPIO_PULL_UP 2

#define GpioSelectFunction(N, F) (&GPIO->GPFSEL0)[(N)/10] = ((&GPIO->GPFSEL0)[(N)/10] & ~(7 << (((N)%10)*3))) | ((F) << (((N)%10)*3))
#define GpioSelectFunctionInput(N) GpioFunctionSelect((N), GPIO_INPUT)
#define GpioSelectFunctionOutput(N) GpioFunctionSelect((N), GPIO_OUTPUT)
#define GpioSelectFunctionAlternate(N, M) GpioFunctionSelect((N), (M) + 2)

#define GpioSet(N) (&GPIO->GPSET0)[(N)>>5] = 1 << ((N)&31)
#define GpioClear(N) (&GPIO->GPCLR0)[(N)>>5] = 1 << ((N)&31)

static void GpioControlPull(u32 N, u32 P)
{
    GPIO->GPIO_PUP_PDN_CNTRL_REG0 = P;
    BusyWait(150);
    (&GPIO->GPIO_PUP_PDN_CNTRL_REG1)[(N)>>5] = 1 << ((N)&31);
    BusyWait(150);
    GPIO->GPIO_PUP_PDN_CNTRL_REG1 = 0;
}
