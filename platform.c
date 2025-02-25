#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

static void BusyWait(unsigned int Delay)
{
	while(Delay--)
	{
		// Do nothing
	}
}
