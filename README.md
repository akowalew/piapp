# piapp

Bare metal experiments on Raspberry Pi 3B+

## Building

```sh
./build.sh
```

## Running

Prepare empty SD card with FAT32 partition and copy contents of `tftp` folder into it.

## Debugging

I have used JLink PLUS Compact along with Ozone debugger.

Connect JLink into JTAG pins of RPI (all in ALT4 mode).

Simply launch `piapp.jdebug` and should work.
