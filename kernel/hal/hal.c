#include "hal.h"

void hal_init(){
    init_gdt();
    //trackpad_init();
    idt_init();
    enable_cursor(13, 14);
}

void enable_cursor(uint8_t cursor_start, uint8_t cursor_end) {
    init_cursor();
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start); // Keep upper 2 bits, modify start

    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end); // Keep upper 3 bits, modify e
}

void disable_cursor() {
    outb(0x3D4, 0x0A); 
    outb(0x3D5, 0x20); // Bit 5 (0x20) disables the cursor
}

void power_off(){
    int a = 0;
    terminal_clear();
    terminal_print("Powering Down\n");
    while (a < 300000000){
        a++;
    }
    a = 0;
    terminal_clear();
    terminal_print("Powering Down.\n");
    while (a < 300000000){
        a++;
    }
    a = 0;
    terminal_clear();
    terminal_print("Powering Down..\n");
    while (a < 300000000){
        a++;
    }
    a = 0;
    terminal_clear();
    terminal_print("Powering Down...\n");
    while (a < 300000000){
        a++;
    }
    outb(0x64, 0xFE); // Send reset command to the keyboard controller
}