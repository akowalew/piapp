DFLAGS="$DFLAGS -D__FPU_PRESENT=1 "
CFLAGS="$CFLAGS -march=armv8-a+nosimd -mtune=cortex-a53 -gdwarf-2 -g3"
OFLAGS="$OFLAGS -O0 -ffreestanding -nostdlib -ffunction-sections -fdata-sections"
WFLAGS="$WFLAGS -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wundef -Wshadow"
XFLAGS="$XFLAGS -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable"
LFLAGS="$LFLAGS --gc-sections"

set -e
mkdir -p out; cd out
aarch64-linux-gnu-gcc -c ../src/main.c ../src/start.S $DFLAGS $CFLAGS $OFLAGS $WFLAGS $XFLAGS
aarch64-linux-gnu-ld -m aarch64elf -nostdlib start.o main.o -T ../link.ld -o kernel8.elf $LFLAGS
aarch64-linux-gnu-objcopy -O binary kernel8.elf ../tftp/30aa58ed/kernel8.img
