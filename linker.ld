ENTRY(_start)

SECTIONS {
    /* Start at 1MB */
    . = 1M;

    /* Ensure Multiboot2 header is at the very start */
    .multiboot_header ALIGN(8) : {
        KEEP(*(.multiboot_header))
    }

    /* Code Section */
    .text ALIGN(4K) : {
        *(.text*)
    }   

    /* Read-only data */
    .rodata ALIGN(4K) : {
        *(.rodata*)    /* Read-only data */
    }

    /* Initialized data */
    .data ALIGN(4K) : {
        *(.data*)      /* Initialized data */
    }

    /* Uninitialized data (zeroed out) */
    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss*)
    }

    /* Stack (16 KB) */
    .stack ALIGN(16) : {
        . = . + 64K;
        PROVIDE(stack_top = .);
    }

}
