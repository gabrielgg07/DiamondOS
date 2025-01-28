#ifndef HAL_H
#define HAL_H

#include "../interrupts/interrupt_init.h"
#include "../gdt/gdt_init.h"
#include "../terminal/terminal.h"

void hal_init();

#endif