CFLAGS="$CFLAGS -ffreestanding -nostdlib -march=armv8-a+nosimd -mtune=cortex-a53 -gdwarf-2 -g3 -O0 -ffunction-sections -fdata-sections"
WFLAGS="$WFLAGS -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wundef"
XFLAGS="$XFLAGS -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-language-extension-token"
LFLAGS="$LFLAGS --gc-sections"

mkdir -p out; cd out
aarch64-linux-gnu-gcc-14 -c ../main.c ../start.S $CFLAGS $WFLAGS $XFLAGS
ld.lld -m aarch64elf -nostdlib start.o main.o -T ../link.ld -o kernel8.elf $LFLAGS
llvm-objcopy -O binary kernel8.elf ../tftp/30aa58ed/kernel8.img
