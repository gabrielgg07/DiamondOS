// kernel/terminal/terminal.h

#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include "cursor.h"
#include "../drivers/keyboard/keyboard.h"
#include <stdbool.h>




#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VIDEO_MEMORY (uint16_t*) 0xB8000


// Function declarations
void terminal_clear();
void terminal_put_char(char c);
void terminal_print(const char *str);
void terminal_backspace();
char terminal_handle_input();
void terminal_arrow(char c);


void terminal_print_hex(uint32_t num); 

#endif
