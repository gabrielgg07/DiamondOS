#include "hal.h"

void hal_init(){
    init_gdt();
    idt_init();
    init_cursor();
}