#ifndef CURSOR_H
#define CURSOR_H

#include <stdint.h>
#include "../port_io/port_io.h"

// Cursor control functions
void update_cursor(int row, int col);
void get_cursor_position(int *row, int *col);
void init_cursor();
void reset_cursor();

#endif // CURSOR_H
