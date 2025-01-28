#include "cursor.h"

// VGA constants
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

// Ports for VGA cursor
#define VGA_COMMAND_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

void update_cursor(int row, int col) {
    uint16_t position = row * VGA_WIDTH + col;

    // Send the high byte of the cursor position
    outb(VGA_COMMAND_PORT, 14);          // Select the high cursor byte
    outb(VGA_DATA_PORT, (position >> 8) & 0xFF);

    // Send the low byte of the cursor position
    outb(VGA_COMMAND_PORT, 15);          // Select the low cursor byte
    outb(VGA_DATA_PORT, position & 0xFF);
}

void init_cursor(){
    update_cursor(0,0);
}

void get_cursor_position(int *row, int *col) {
    uint16_t position = 0;

    // Read the high byte of the cursor position
    outb(VGA_COMMAND_PORT, 14);
    position |= inb(VGA_DATA_PORT) << 8;

    // Read the low byte of the cursor position
    outb(VGA_COMMAND_PORT, 15);
    position |= inb(VGA_DATA_PORT);

    *row = position / VGA_WIDTH;
    *col = position % VGA_WIDTH;
}
