#!/bin/sh

DFLAGS="$DFLAGS -D__FPU_PRESENT=1 "
CFLAGS="$CFLAGS -march=armv8-a+nosimd -mtune=cortex-a53 -gdwarf-2 -g3"
OFLAGS="$OFLAGS -O0 -ffreestanding -nostdlib -ffunction-sections -fdata-sections -static"
WFLAGS="$WFLAGS -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wundef -Wshadow"
XFLAGS="$XFLAGS -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-overlength-strings"
LFLAGS="$LFLAGS -Wl,-Tlink.ld -Wl,--gc-sections -Wl,--no-warn-rwx-segments -Wl,--build-id=none"

mkdir -p out
aarch64-none-elf-gcc main.c start.S -o out/kernel8.elf $DFLAGS $CFLAGS $OFLAGS $WFLAGS $XFLAGS $LFLAGS
if [ "$1" = "--full" ]; then
	aarch64-none-elf-objcopy -O binary out/kernel8.elf tftp/kernel8.img
fi
