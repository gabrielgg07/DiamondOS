#ifndef HAL_H
#define HAL_H

#include "../interrupts/interrupt_init.h"
#include "../gdt/gdt_init.h"
#include "../terminal/terminal.h"
#include "../drivers/trackpad/trackpad.h"
#include "../drivers/graphics/vesa.h"

extern uint32_t magic;   // Defined in start.asm
extern uint32_t mb_info; // Defined in start.asm

void hal_init();
void power_off();
void enable_cursor(uint8_t cursor_start, uint8_t cursor_end);
void disable_cursor();

#endif