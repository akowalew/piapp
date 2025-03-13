ENTRY(_start)

SECTIONS
{
    . = 0x8000;  /* Load at address 0x8000 (default for RPi) */

    .text :
    {
        *(.boot .boot.*)
        *(.text .text.* .gnu.linkonce.t*)
    }

    .rodata :
    {
        *(.rodata .rodata* .gnu.linkonce.r*)
    }

    .got : {
        *(.got .got.*)      /* Global Offset Table */
    }

    _sidata = .;  /* Start of .data in ROM */

    .data :
    {
        _sdata = .;
        *(.data .data.*)
        _edata = .;
    }

    .bss :
    {
        _sbss = .;
        *(COMMON)
        *(.bss .bss.*)
        _ebss = .;
    }

    . = ALIGN(8);
    .stack :
    {
        . = . + 0x1000;  /* 4KB stack */
        stack_top = .;
    }

    /DISCARD/ :
    {
        *(.comment)
    }
}
