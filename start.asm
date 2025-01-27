; ==========================================================================
; start.asm -- Minimal x86 "start" for a Multiboot-compatible kernel
; ==========================================================================
; Assemble with:
;   nasm -f elf32 start.asm -o start.o
; ==========================================================================

[bits 32]

; ----------------------------------------------------------------------------
; Place the Multiboot header in its own section so it ends up early in the
; final binary. We rely on the linker script to put .multiboot at the front
; of the .text area or at least within the first 8KB of the file.
; ----------------------------------------------------------------------------
section .multiboot
align 4

; Multiboot Header:
dd 0x1BADB002                 ; 0: MB magic
dd 0x00000000                 ; 4: MB flags
dd -(0x1BADB002 + 0x00000000) ; 8: sum = -(magic + flags)

; ----------------------------------------------------------------------------
; Actual code section
; ----------------------------------------------------------------------------
section .text
global _start

; Declare symbols we'll need from C or the linker:
extern kernel_main
extern stack_top

; ----------------------------------------------------------------------------
; _start: where GRUB hands control to us
; ----------------------------------------------------------------------------
_start:
    ; Optionally, set up your stack. We'll rely on the linker-provided stack_top.

    mov esp, stack_top
    mov ebp, esp
    
    ; Now call our main kernel function in C
    call kernel_main

.early_hang:
    cli            ; disable interrupts
    hlt            ; halt the CPU
    jmp .early_hang
