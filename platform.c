#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

static void BusyWait(unsigned int Delay)
{
	while(Delay--)
	{
		// Do nothing
	}
}
