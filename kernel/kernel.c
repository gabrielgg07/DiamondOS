
#include <stdint.h>
#include "terminal/terminal.h"
#include "port_io/port_io.h"
#include "interrupts/interrupt_init.h"
#include "gdt/gdt_init.h"

#include <stdint.h>

void itoa(uint32_t value, char* buffer, int base) {
    char digits[] = "0123456789ABCDEF";
    char temp[16];  // Temporary buffer for conversion
    int i = 0, j = 0;

    if (value == 0) {
        buffer[j++] = '0';
    } else {
        while (value > 0) {
            temp[i++] = digits[value % base];
            value /= base;
        }
    }

    // Reverse the string
    while (i > 0) {
        buffer[j++] = temp[--i];
    }

    buffer[j] = '\0';  // Null-terminate the string
}

void check_segments() {
    uint16_t cs, ds, es, fs, gs, ss;
    char buffer[16];

    asm volatile ("mov %%cs, %0" : "=r"(cs));
    asm volatile ("mov %%ds, %0" : "=r"(ds));
    asm volatile ("mov %%es, %0" : "=r"(es));
    asm volatile ("mov %%fs, %0" : "=r"(fs));
    asm volatile ("mov %%gs, %0" : "=r"(gs));
    asm volatile ("mov %%ss, %0" : "=r"(ss));

    terminal_print("CS: 0x");
    itoa(cs, buffer, 16);
    terminal_print(buffer);
    terminal_print("\n");

    terminal_print("DS: 0x");
    itoa(ds, buffer, 16);
    terminal_print(buffer);
    terminal_print("\n");

    terminal_print("ES: 0x");
    itoa(es, buffer, 16);
    terminal_print(buffer);
    terminal_print("\n");

    terminal_print("FS: 0x");
    itoa(fs, buffer, 16);
    terminal_print(buffer);
    terminal_print("\n");

    terminal_print("GS: 0x");
    itoa(gs, buffer, 16);
    terminal_print(buffer);
    terminal_print("\n");

    terminal_print("SS: 0x");
    itoa(ss, buffer, 16);
    terminal_print(buffer);
    terminal_print("\n");
}





void kernel_main() {

    init_gdt();
    
    idt_init();
    terminal_clear();
    terminal_print("Hello Welcome to Diamomd OS, Press any key to contiue.");

    while (1);
}

