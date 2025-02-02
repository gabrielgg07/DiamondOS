#include "trackpad.h"

void trackpad_init(void) {
    terminal_print("Initializing Trackpad...\n");

    // Enable second PS/2 port (if not already enabled)
    outb(0x64, 0xA8);  // Command: Enable second PS/2 port

    // Reset the trackpad
    outb(0x64, 0xD4);  // Tell PS/2 controller that next command is for the second device
    outb(0x60, 0xFF);  // Command: Reset device

    // Wait for ACK (0xFA) and self-test response (0xAA for success)
    uint8_t response = inb(0x60);
    if (response == 0xFA) {
        terminal_print("Trackpad reset ACK received!\n");
    }
    response = inb(0x60);
    if (response == 0xAA) {
        terminal_print("Trackpad self-test passed!\n");
    } else {
        terminal_print("Trackpad self-test failed or no response.\n");
    }

    // Enable data reporting (this makes the trackpad send movement events)
    outb(0x64, 0xD4);  // Next command is for the second PS/2 port
    outb(0x60, 0xF4);  // Command: Enable data reporting
    int i = 0;

    // Wait for ACK (0xFA)
    response = inb(0x60);
    if (response == 0xFA) {
        terminal_print("Trackpad data reporting enabled!\n");
    } else {
        terminal_print("Trackpad did not acknowledge data reporting.\n");
    }
}