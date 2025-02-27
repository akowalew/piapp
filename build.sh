CFLAGS="$CFLAGS -ffreestanding -nostdlib -march=armv8-a+nosimd -mtune=cortex-a53 -gdwarf-2 -g3 -O0"

mkdir -p out; cd out
aarch64-linux-gnu-gcc -c ../main.c ../start.S $CFLAGS
aarch64-linux-gnu-ld -m aarch64elf -nostdlib start.o main.o -T ../link.ld -o kernel8.elf
aarch64-linux-gnu-objcopy -O binary kernel8.elf ../tftp/30aa58ed/kernel8.img
