# piapp

Bare metal experiments on Raspberry Pi 3B+

## Preparation

0. Install some recent Linux distro (I'm using Xubuntu 24.04.2 LTS).
1. Grab an MicroSD card (8GB is fine, but basically any size will be good since things are extremely small)
2. Format it to be completely empty, delete all volumes, create MS-DOS partition table
3. Create one partition to fill whole MicroSD card and format it to FAT32
4. Download x86_64 Linux hosted cross tolchains for AArch64 bare-metal target version 14.2.1: https://developer.arm.com/-/media/Files/downloads/gnu/14.2.rel1/binrel/arm-gnu-toolchain-14.2.rel1-x86_64-aarch64-none-elf.tar.xz and install it, add it to PATH
5. Verify toolchain by calling: `aarch64-none-elf-gcc -v`, which should print in the end the version 14.2.1

## JTAG

Connect J-Link (in my case PLUS Compact) into Raspberry PI JTAG pins configured by default into ALT4 mode:

```
- JLINK #1  VCC  - RPI 3V3 Power (#1, #17)
- JLINK #3  TRST - RPI GPIO 22 (TRST ALT4, #15)
- JLINK #5  TDI  - RPI GPIO 26 (TDI ALT4, #37)
- JLINK #7  TMS  - RPI GPIO 27 (TMS ALT4, #13)
- JLINK #9  TCLK - RPI GPIO 25 (TCK ALT4, #22)
- JLINK #11 RTCK - RPI GPIO 23 (RTCK ALT4, #16)
- JLINK #13 TDO  - RPI GPIO 24 (TDO ALT4, #18)
- JLINK #16 GND  - RPI Ground (#6, #39, #9, #14, #20, #30, #34, #25)
```

As you see JTAG `RESET` pin is not connected because RPi doesn't have one.

## Building

Once the toolchain is ready open the terminal, go to this directory and simply launch:

```sh
./build.sh
```

It should successfully build `out/kernel8.elf` and then objcopy it into `tftp/kernel8.img`.

_If, for some reason, you cannot build it, I left `out/kernel8.elf` and `tftp/kernel8.img` files in the repository._

## Running

Open MicroSD card in a file explorer and copy into it contents of `tftp` folder. It should be something similar to that:

```
-rw-r--r-- 1 adamk adamk   52476 Feb 27 11:48 bootcode.bin
-rw-rw-r-- 1 adamk adamk      19 Feb 27 11:48 config.txt
-rw-r--r-- 1 adamk adamk    7327 Feb 27 11:48 fixup.dat
-rwxrwxr-x 1 adamk adamk     176 Mar 31 13:55 kernel8.img
-rw-rw-r-- 1 adamk adamk 2983488 Mar 31 13:55 start.elf
```

Safely remove SD card and plug it into Raspberry Pi. Power cycle the board.

Optionally you can connect also TTL-UART-3V3 cable to see debug console.

It is helpful especially when debugging the `bootcode.bin` as it prints into UART. Connect it into GPIO14 (UART TX) and GPIO15 (UART RX)

When such cable is connected the `bootcode.bin` should print about reading our stuff:

```
Raspberry Pi Bootcode
Read File: config.txt, 19                   <---- This is our config.txt
Read File: start.elf, 2983488 (bytes)       <---- This is our start.elf
Read File: fixup.dat, 7327 (bytes)          <---- This is our fixup.dat
MESS:00:00:02.024934:0: brfs: File read: /mfs/sd/config.txt
MESS:00:00:02.028995:0: brfs: File read: 19 bytes
MESS:00:00:02.091521:0: brfs: File read: /mfs/sd/config.txt
MESS:00:00:02.304127:0: gpioman: gpioman_get_pin_num: pin DISPLAY_DSI_PORT not defined
MESS:00:00:02.311156:0: gpioman: gpioman_get_pin_num: pin DISPLAY_DSI_PORT not defined
MESS:00:00:02.319777:0: *** Restart logging
MESS:00:00:02.322267:0: brfs: File read: 19 bytes
MESS:00:00:02.352819:0: gpioman: gpioman_get_pin_num: pin EMMC_ENABLE not defined
MESS:00:00:02.369789:0: HDMI0: hdmi_pixel_encoding: 162000000
MESS:00:00:02.384913:0: dtb_file 'bcm2710-rpi-3-b-plus.dtb'
MESS:00:00:02.388875:0: dtb_file 'bcm2710-rpi-3-b.dtb'
MESS:00:00:02.393730:0: dtb_file 'bcm2837-rpi-3-b-plus.dtb'
MESS:00:00:02.399030:0: dterror: Failed to load Device Tree file '?'
MESS:00:00:02.405104:0: Failed to open command line file 'cmdline.txt'
MESS:00:00:02.413575:0: brfs: File read: /mfs/sd/kernel8.img              <-------------- This is our bare-metal kernel8.img
MESS:00:00:02.417540:0: Loaded 'kernel8.img' to 0x200000 size 0xb0
MESS:00:00:02.423457:0: Kernel relocated to 0x80000
MESS:00:00:02.429814:0: uart: Set PL011 baud rate to 103448.300000 Hz
MESS:00:00:02.436106:0: uart: Baud rate change done...
...
```

Possibly there will be no further console logs, since the app is simple and only spins in the loop!

## Debugging

I am using Ozone for debugging. Version 3.34 since newer versions have some bug inside which causes dramatic slowdown when debugging.

1. Simply open Ozone, New Project Wizard, select Cortex A53 as a device, select Cortex-A54 (AArch64) as a register set, peripherals you can omit.
2. Then select Target Interface JTAG, leave default speed at 4MHz, Host Interface USB.
3. Select `kernel8.elf` file which is created in `out` directory after build.
4. Initial PC "Do not set", Initial Stack Pointer "Do not set", choose `jlinkscript` from this directory as a "J-Link Script File" (that's very important!)
5. Start debugging and have fun
6. Each time you call `build.sh` the Ozone will detect changes in `kernel8.elf` and reload the program in the RaspberryPi automatically.

_If, for some reason, you can't create such a project in Ozone, I've left it here in repository under `piapp.jdebug`._
