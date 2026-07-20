ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 0xffffffff80000000;

    .text ALIGN(0x1000) :
    {
        *(.text*)
    } :text

    .rodata ALIGN(0x1000) :
    {
        *(.rodata*)
        *(.eh_frame*)
    } :rodata

    .limine_requests ALIGN(16) :
    {
        KEEP(*(.limine_requests_start))
        KEEP(*(.limine_requests))
        KEEP(*(.limine_requests_end))
    } :rodata


    .data ALIGN(0x1000) :
    {
        *(.data*)
    } :data


    .bss ALIGN(0x1000) :
    {
        *(COMMON)
        *(.bss*)
    } :data
}