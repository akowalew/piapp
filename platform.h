#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef u32 b32;

#define Aligned(x) __attribute__((aligned(x)))
#define BRK(x) asm("brk #0");

#define Assert(x) if(!(x)) { while(1){BRK();} }

#define __get_CP(cp, op1, Rt, CRn, CRm, op2) asm volatile("MRC p" # cp ", " # op1 ", %0, c" # CRn ", c" # CRm ", " # op2 : "=r" (Rt) : : "memory" )
#define __set_CP(cp, op1, Rt, CRn, CRm, op2) asm volatile("MCR p" # cp ", " # op1 ", %0, c" # CRn ", c" # CRm ", " # op2 : : "r" (Rt) : "memory" )
#define ivac(Rt) asm volatile("dc ivac, %0" : : "r" (Rt) : "memory")
#define cvac(Rt) asm volatile("dc cvac, %0" : : "r" (Rt) : "memory")
#define civac(Rt) asm volatile("dc civac, %0" : : "r" (Rt) : "memory")
#define __get_CP64(cp, op1, Rt, CRm)         asm volatile("MRRC p" # cp ", " # op1 ", %Q0, %R0, c" # CRm  : "=r" (Rt) : : "memory" )
#define __set_CP64(cp, op1, Rt, CRm)         asm volatile("MCRR p" # cp ", " # op1 ", %Q0, %R0, c" # CRm  : : "r" (Rt) : "memory" )

static void BusyWait(unsigned int Delay)
{
	while(Delay--)
	{
		// Do nothing
	}
}
