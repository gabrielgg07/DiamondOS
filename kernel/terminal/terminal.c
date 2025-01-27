#include "terminal.h"

#include "../port_io/port_io.h"

char *vidmem = (char*) 0xb8000;
unsigned int screen_size = 80 * 25;

static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

void get_cursor_position() {
    uint16_t pos = 0;
    
    // Get high byte
    outb(0x03D4, 0x0E);
    pos = inb(0x03D5) << 8;
    
    // Get low byte
    outb(0x03D4, 0x0F);
    pos |= inb(0x03D5);
    
    cursor_x = pos % VGA_WIDTH;
    cursor_y = pos / VGA_WIDTH;
}

void update_cursor () {
    if (cursor_x > 79 && cursor_y > 24) {
        cursor_x = 0;
        cursor_y = 0;
    }
    else if (cursor_x > 79){
        cursor_x = 0;
        cursor_y++;
    }
    else {
        cursor_x++;
    }
}

void terminal_clear(){

    for (unsigned int i = 0; i < screen_size; i++) {
        vidmem[i * 2] = ' ';
        vidmem[i * 2 + 1] = 0x07;
    }
}
void terminal_put_char(char c){
        int index = cursor_y * VGA_WIDTH + cursor_x;
        vidmem[index * 2] = c;
        vidmem[index * 2 + 1] = 0x07;
        update_cursor();

}
void terminal_print(const char *str){
    int i = 0;
    while (str[i] != '\0'){
        terminal_put_char(str[i]);
        i++;
    }
}

void terminal_color(){
        int index = cursor_y * VGA_WIDTH + cursor_x;
        //vidmem[index * 2] = c;
        vidmem[index * 2 + 1] = 0x97;
        update_cursor();
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

void reset_cursor(){
    cursor_x = 0;
    cursor_y = 0;
}
