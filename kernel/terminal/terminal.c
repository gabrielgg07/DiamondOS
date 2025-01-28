#include "terminal.h"

#include "../port_io/port_io.h"

char *vidmem = (char*) 0xb8000;
unsigned int screen_size = 80 * 25;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;


void scroll() {
    // Each row in VGA text mode is (VGA_WIDTH * 2) bytes
    // because each character has 2 bytes: one for ASCII, one for the attribute.
    int bytes_per_row = VGA_WIDTH * 2;

    // 1) Move rows [1..VGA_HEIGHT-1] up to rows [0..VGA_HEIGHT-2]
    for (int row = 1; row < VGA_HEIGHT; row++) {
        for (int col = 0; col < bytes_per_row; col++) {
            // Calculate the source index (row) and destination index (row-1)
            int src_index  = (row * bytes_per_row) + col;
            int dest_index = ((row - 1) * bytes_per_row) + col;

            vidmem[dest_index] = vidmem[src_index];
        }
    }

    // 2) Clear the last row (row = VGA_HEIGHT - 1)
    int last_row_start = (VGA_HEIGHT - 1) * bytes_per_row;
    for (int col = 0; col < bytes_per_row; col += 2) {
        vidmem[last_row_start + col]     = ' ';   // Space character
        vidmem[last_row_start + col + 1] = 0x07;  // Default VGA attribute (white-on-black)
    }
}


void terminal_clear(){
    
    for (unsigned int i = 0; i < screen_size; i++) {

        vidmem[i * 2] = ' ';
        vidmem[i * 2 + 1] = 0x07;
    }
}
void terminal_put_char(char c){
        
    int cursor_y;
    int cursor_x;
    get_cursor_position(&cursor_y,&cursor_x);

    if (cursor_y >= VGA_HEIGHT) {
        scroll();
        //cursor_x = 0;
        cursor_y = VGA_HEIGHT - 1;
    }
    int index = cursor_y * VGA_WIDTH + cursor_x;


    if (c == '\n') {
        // Handle new line: move to the next row, reset column
        cursor_x = 0;
        cursor_y++;
    } else {
        // Write character to video memory
        vidmem[index * 2] = c;
        vidmem[index * 2 + 1] = 0x07; // Attribute byte (white text on black background)
        cursor_x++; // Move to the next column
    }

    // Handle line wrapping
    if (cursor_x >= VGA_WIDTH) {
        cursor_x = 0;
        cursor_y++;
    }


    // Update hardware cursor
    update_cursor(cursor_y, cursor_x);
}
void terminal_print(const char *str){
    int i = 0;
    while (str[i] != '\0'){
        terminal_put_char(str[i]);
        i++;
    }
}



void terminal_print_hex(uint32_t num) {
    char hex_chars[] = "0123456789ABCDEF";
    char buffer[9];  // 8 hex digits + null terminator
    buffer[8] = '\0';  // Null-terminate the string

    for (int i = 7; i >= 0; i--) {
        buffer[i] = hex_chars[num & 0xF];  // Extract last 4 bits (nibble)
        num >>= 4;  // Shift right by 4 bits to process next digit
    }

    terminal_print("0x");
    terminal_print(buffer);
}

void terminal_backspace() {
    int cursor_y;
    int cursor_x;
    get_cursor_position(&cursor_y, &cursor_x);

    // Move the cursor back
    if (cursor_x > 0) {
        cursor_x--;
    } else if (cursor_y > 0) {
        // If at the start of a line, move to the end of the previous line
        cursor_y--;
        cursor_x = VGA_WIDTH - 1;
    } else {
        // Cursor is already at the top-left; nothing to backspace
        return;
    }

    // Clear the character at the new position
    int index = (cursor_y * VGA_WIDTH + cursor_x) * 2; // Calculate video memory index
    vidmem[index] = ' ';        // Clear with a space
    vidmem[index + 1] = 0x07;   // Default attribute (white on black)

    // Update the cursor
    update_cursor(cursor_y, cursor_x);
}

