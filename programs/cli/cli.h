#ifndef CLI_H
#define CLI_H

#include "../../kernel/terminal/terminal.h"
#include "../../kernel/drivers/keyboard/keyboard.h"
#include "../../kernel/hal/hal.h"



void run_cli();
void process_command();
void add_com();
void int_to_string(char *buffer, int num);
int isdigit(char c);
int my_strncmp(const char *s1, const char *s2, int n);
void divide_com();
void help_com();

#endif