#include "exception_handler.h"


__attribute__((noreturn))
void exception_handler(void) {
    __asm__ volatile ("cli; hlt"); // Completely hangs the computer
}