CFLAGS="$CFLAGS -ffreestanding -nostdlib -mcpu=cortex-a53+nosimd --target=aarch64-elf -g -O0 -ffunction-sections -fdata-sections"
WFLAGS="$WFLAGS -Wall -Wextra -Wpedantic -Werror -Wfatal-errors -Wundef"
XFLAGS="$XFLAGS -Wno-unused-function -Wno-unused-variable -Wno-unused-parameter -Wno-unused-but-set-variable -Wno-language-extension-token"
LFLAGS="$LFLAGS --gc-sections"

mkdir -p out; cd out
clang -c ../main.c ../start.S $CFLAGS $WFLAGS $XFLAGS
ld.lld -m aarch64elf -nostdlib start.o main.o -T ../link.ld -o kernel8.elf $LFLAGS
llvm-objcopy -O binary kernel8.elf ../tftp/kernel8.img
