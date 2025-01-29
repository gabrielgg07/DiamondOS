#include "keyboard.h"

static bool pressed = false;

char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
int keyboard_buffer_head = 0;
int keyboard_buffer_tail = 0;


__attribute__((interrupt))
void keyboard_interrupt_handler(interrupt_frame_t* frame) {
    uint8_t scancode = inb(0x60);

    // Send End of Interrupt (EOI) to PIC
    outb(0x20, 0x20);

    // Handle scancode (convert to character or store raw scancode)
    if (!(scancode & 0x80)) {  // Key down event (ignore key up)
        const char *character = scancode_to_string[scancode];
        if (character) {
            keyboard_buffer_enqueue(character);  // Store character in a buffer
        }
    }
}

void keyboard_buffer_enqueue(const char *character) {
    keyboard_buffer[keyboard_buffer_head] = *character;
    keyboard_buffer_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
}

char keyboard_buffer_dequeue() {
    if (keyboard_buffer_head == keyboard_buffer_tail) {
        return 0;  // Buffer is empty
    }
    char c = keyboard_buffer[keyboard_buffer_tail];
    keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}
