
#include <stdint.h>
#include "terminal/terminal.h"
#include "hal/hal.h"

#include <stdint.h>




void kernel_main() {

    hal_init();
    terminal_clear();
    terminal_print("Hello Welcome to Diamomd OS, Press any key to contiue.");

    while (1);
}

