
#include <stdint.h>
#include "terminal/terminal.h"
#include "hal/hal.h"
#include "../programs/cli/cli.h"

#include <stdint.h>


void booting_message();

void kernel_main() {
    booting_message();
    hal_init();
    terminal_clear();
    terminal_print("Hello Welcome to Diamomd OS, Press any key to contiue.");
    char check = '\0';
    while (check == '\0'){
        check = terminal_handle_input();
    }
    run_cli();

}

void print_diamond();

void booting_message(){
    int i = 0;
    while(i < 3){
        int a =0;
        terminal_clear();
        terminal_print("Booting Diamond OS");
        while (a < 300000000){
            a++;
        }
        a =0;
        terminal_clear();
        terminal_print("Booting Diamond OS.");
        while (a < 300000000){
            a++;
        }
        a =0;
        terminal_clear();
        terminal_print("Booting Diamond OS..");
        while (a < 300000000){
            a++;
        }
        a =0;
        terminal_clear();
        terminal_print("Booting Diamond OS...");
        while (a < 300000000){
            a++;
        }
        i++;
    }
    terminal_clear();
    int a =0;
    print_diamond();
    while (a < 300000000){
        a++;
    }
}

void print_diamond() {
    terminal_print("   /\\ \n");
    terminal_print("  /  \\ \n");
    terminal_print(" /    \\\n");
    terminal_print(" \\    /\n");
    terminal_print("  \\  /\n");
    terminal_print("   \\/\n");
}

