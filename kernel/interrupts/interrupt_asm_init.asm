section .text

; Macro to generate ISRs without error codes
%macro isr_no_err_stub 1
isr_stub_%+%1:
    call exception_handler
    iret
%endmacro

; Macro to generate ISRs with error codes
%macro isr_err_stub 1
isr_stub_%+%1:
    call exception_handler
    add esp, 4       ; Remove the error code from the stack
    iret
%endmacro

; Extern declaration to link with C handler
extern exception_handler

; Define ISRs for exceptions without an error code
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8   ; Double Fault (with error code)
isr_no_err_stub 9
isr_err_stub    10  ; Invalid TSS
isr_err_stub    11  ; Segment Not Present
isr_err_stub    12  ; Stack-Segment Fault
isr_err_stub    13  ; General Protection Fault
isr_err_stub    14  ; Page Fault
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17  ; Alignment Check
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30  ; Security Exception
isr_no_err_stub 31

; Define an ISR table to store addresses
section .data
global isr_stub_table
isr_stub_table:
%assign i 0 
%rep 32 
    dd isr_stub_%+i  ; Use "dq" for 64-bit
%assign i i+1 
%endrep
