OUTPUT_ARCH(aarch64)
OUTPUT_FORMAT("elf64-littleaarch64", "elf64-bigaarch64", "elf64-littleaarch64")

MEMORY
{
    ram_code (rwx) : ORIGIN = 0x80000, LENGTH = 64K
    ram_data (rwx) : ORIGIN = 0x80000 + 64K, LENGTH = 64K
}

SECTIONS
{
	.text :
	{
		KEEP(*(.text.boot))
		*(.text .text.* .gnu.linkonce.t*)
	} > ram_code

	.rodata :
	{
		*(.rodata .rodata* .gnu.linkonce.r*)
	} > ram_code

	PROVIDE(_data = .);

	.bss (NOLOAD) :
	{
		. = ALIGN(16);
		__bss_start = .;
		*(.bss .bss.*)
		*(COMMON)
		__bss_end = .;
	} > ram_data

	_end = .;

	/DISCARD/ :
	{
		*(.comment)
	}
}

__bss_size = (__bss_end - __bss_start) >> 3;
