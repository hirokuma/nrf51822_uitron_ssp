SEARCH_DIR(.)
GROUP(-lgcc -lc -lnosys)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00018000, LENGTH = 0x28000
    SRAM (rwx) : ORIGIN = 0x20002000, LENGTH = 0x02000
}

PROVIDE(hardware_init_hook = 0);
PROVIDE(software_init_hook = 0);
PROVIDE(software_term_hook = 0);
STARTUP(start.o)

SECTIONS
{
    .text :
    {
        __text = .;
        KEEP(*(.vector))
        *(.text)
        *(.text.*)
    } > FLASH
    _etext = .;
    PROVIDE(etext = .);

    .rodata :
    {
        *(.rodata)
        *(.rodata.*)
        . = ALIGN(4);
    } > FLASH

    .ARM.extab   : { *(.ARM.extab* .gnu.linkonce.armextab.*) }
    __exidx_start = .;
    .ARM.exidx   : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
    __exidx_end = .;

    __idata_start = .;
    .data : AT (__idata_start)
    {
        __data_start = .;
        *(vtable)
        *(.data)
        *(.data.*)
        __data_end = .;
    } > SRAM
    __idata_end = __idata_start + SIZEOF(.data);
    _edata = .;
    PROVIDE(edata = .);

    . = ALIGN(4);

    .bss :
    {
        __bss_start  = .;
        *(.bss)
        *(COMMON)
        *(.mtb.$RAM1)
        . = ALIGN(4);
    } > SRAM
    
    . = ALIGN(4);
    _end = .;
    __bss_end = .;
    PROVIDE(end = .);
}
