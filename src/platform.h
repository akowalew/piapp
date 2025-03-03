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
#define ArrayCount(x) sizeof(x)/sizeof(x[0])
