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
section .multiboot_header
global multiboot_header

align 8
multiboot_header:
    dd 0xe85250d6                ; Multiboot2 Magic Number
    dd 0                         ; Architecture (0 = 32-bit)
    dd multiboot_header_end - multiboot_header  ; Header length
    dd -(0xe85250d6 + 0 + (multiboot_header_end - multiboot_header)) ; Checksum

    ; ---------------------------------------------------------------------------
    ; Framebuffer Request Tag
    ; This tag requests that GRUB set up a linear framebuffer mode.
    ; Tag Type: 5
    ; Tag Size: 24 bytes (20 bytes of data + 4 bytes padding for 8-byte alignment)
    ; Fields:
    ;   - Desired framebuffer width   (in pixels)
    ;   - Desired framebuffer height  (in pixels)
    ;   - Desired bits per pixel      (color depth)
    ; You can adjust these values as needed.
    ; ---------------------------------------------------------------------------
    align 8
    multiboot_header_tag_framebuffer:
        dd 5                        ; Tag type: Framebuffer request (5)
        dd 24                       ; Tag size (must be a multiple of 8)
        dd 1024                     ; Requested width
        dd 768                      ; Requested height
        dd 32                       ; Requested bpp
        dd 0                        ; Reserved / padding

multiboot_header_end:


; ---------------------------------------------------------------------------
; Some Data for the VESA Framebuffer
; ---------------------------------------------------------------------------
section .bss
align 4
global magic
global mb_info  

magic:    resb 4
mb_info:  resb 4

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
    mov [magic], eax    ; Store Multiboot2 magic number
    mov [mb_info], ebx  ; Store Multiboot2 info structure address
    
    ; Now call our main kernel function in C
    call kernel_main

.early_hang:
    cli            ; disable interrupts
    hlt            ; halt the CPU
    jmp .early_hang
