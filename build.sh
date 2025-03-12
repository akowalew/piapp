#!/bin/sh

DFLAGS="$DFLAGS -D__FPU_PRESENT=1"
CFLAGS="$CFLAGS -marm -march=armv6 -mtune=arm1176jzf-s -mfpu=vfp -mfloat-abi=hard"
GFLAGS="$GFLAGS -gdwarf-2 -g3"
OFLAGS="$OFLAGS -O0 -ffreestanding -nostartfiles -nostdlib -ffunction-sections -fdata-sections -static"
WFLAGS="$WFLAGS -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wundef -Wshadow"
XFLAGS="$XFLAGS -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-overlength-strings"
LFLAGS="$LFLAGS -Wl,-Tbcm2835_link.ld -Wl,-Map=out/kernel.map -Wl,--gc-sections -Wl,--no-warn-rwx-segments -Wl,--build-id=none"
IFLAGS="$IFLAGS -I."

set -ex
mkdir -p out;
arm-none-eabi-gcc src/main.c src/bcm2835_start.S -o out/kernel.elf $DFLAGS $CFLAGS $OFLAGS $WFLAGS $XFLAGS $IFLAGS $LFLAGS $GFLAGS
if [ "$1" = "--full" ]; then
	arm-none-eabi-objcopy -O binary out/kernel.elf tftp/kernel.img
fi
