#include "hal.h"

void hal_init(){
    init_gdt();
    idt_init();
    init_cursor();
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