#!/bin/sh

DFLAGS="$DFLAGS -D__FPU_PRESENT=1 "
CFLAGS="$CFLAGS -march=armv8-a+simd -ffast-math -fstrict-aliasing -mtune=cortex-a53"
GFLAGS="$GFLAGS -gdwarf-2 -g3"
OFLAGS="$OFLAGS -O0 -ffreestanding -nostartfiles -nostdlib -ffunction-sections -fdata-sections -static"
WFLAGS="$WFLAGS -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wundef -Wshadow"
XFLAGS="$XFLAGS -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-overlength-strings"
LFLAGS="$LFLAGS -Wl,-Tlink.ld -Wl,-Map=out/kernel8.map -Wl,--gc-sections -Wl,--no-warn-rwx-segments -Wl,--build-id=none"
IFLAGS="$IFLAGS -I."

mkdir -p out;
aarch64-linux-gnu-gcc-9 src/main.c src/start.S -o out/kernel8.elf $DFLAGS $CFLAGS $OFLAGS $WFLAGS $XFLAGS $IFLAGS $LFLAGS $GFLAGS
if [ "$1" = "--full" ]; then
	aarch64-linux-gnu-objcopy -O binary out/kernel8.elf tftp/30aa58ed/kernel8.img
fi
