ENTRY(_start)

SECTIONS
{
    . = 0x8000;  /* Load at address 0x8000 (default for RPi) */

    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    _sidata = .;  /* Start of .data in ROM */

    .data :
    {
        _sdata = .;
        *(.data)
        _edata = .;
    }

    .bss :
    {
        _sbss = .;
        *(COMMON)
        *(.bss)
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
