ENTRY(_start)

SECTIONS {
    /* Start at 1MB */
    . = 1M;

    /* Code Section */
    .text ALIGN(4K) : {
        *(.multiboot)  /* Multiboot header */
        *(.text*)      /* Code */
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
