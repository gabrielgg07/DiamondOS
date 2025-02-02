
#include <stdint.h>
#include "hal/hal.h"
#include "../programs/cli/cli.h"
#include "drivers/graphics/vesa.h"


#include <stdint.h>

extern uint32_t magic;
extern uint32_t mb_info;


void kernel_main() {
    hal_init();
    load_vesa_from_grub(magic, mb_info);
    // Draw a grid pattern to verify both axes.
    char c;

    while (1)
    {
        while ((c = keyboard_buffer_dequeue()) == 0);
        // Print the character on the screen
        char str[2] = {c, '\0'}; // Convert to string
        vesa_print_string(str, 0xFFFFFFFF);

    }
    while(1);

}
