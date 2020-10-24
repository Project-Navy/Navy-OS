ENTRY(_start)
 
SECTIONS
{
	. = 1M;
        __start = .;

    .stivale2hdr ALIGN(4K) :
    {
        *(.stivale2hdr)
    }

	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.text)
	}
 
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}
 
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}
 
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

        __end = .;
 
}
