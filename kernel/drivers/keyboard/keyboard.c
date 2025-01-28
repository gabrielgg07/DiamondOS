#include "keyboard.h"

static bool pressed = false;




__attribute__((interrupt))
void keyboard_interrupt_handler(interrupt_frame_t* frame) {
    // Read the scancode from the keyboard controller
    uint8_t scancode = inb(0x60);
    const char *character = scancode_to_string[scancode];


    // Send End of Interrupt (EOI) to the PIC
    outb(0x20, 0x20);

    // Check if it's a break code (key up event)
    if (scancode & 0x80) {
            // Key up event (MSB is set)
        if (pressed) {
            pressed = false;  // Update state to "not pressed"
            uint8_t keycode = scancode & 0x7F;  // Remove the "key up" bit
            
        }
    } else {
        // Key down event (MSB is clear)
        if (!pressed) {
            pressed = true;  // Update state to "pressed"
            if (character == "Enter"){
                terminal_print("\n");
            }else if (character =="Space"){
                terminal_print(" ");
            }else if (character == "Backspace") {
                terminal_backspace();
            }else{
                terminal_print(character);
            }
        }
    }
}
