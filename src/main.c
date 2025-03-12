#include "platform.h"
#include "platform.c"
#include "bcm2835.h"
#include "bcm2835.c"
#include "common.h"
#include "common.c"
#include "uart1.h"
#include "uart1.c"
#include "console.h"
#include "console.c"

int main(void)
{
	int Counter = 0;
	ConsoleInit();
	while(1)
	{
		ConsolePutz("Hello world\n");
		ConsolePrintf("Siema byku: %d\n", Counter++);
	}
}
