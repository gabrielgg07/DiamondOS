#include "cli.h"

#define MAX_COMMAND_LENGTH 256  // Define a reasonable max size

char command[MAX_COMMAND_LENGTH]; // Static buffer
int command_length = 0; // Tracks number of characters added

// Custom strcmp function
int my_strncmp(const char *s1, const char *s2, int n) {
    while (n > 0 && *s1 && *s2) { // Compare up to n characters
        if (*s1 != *s2) {
            return *s1 - *s2; // Return difference if mismatch
        }
        s1++;
        s2++;
        n--;
    }

    return (n == 0) ? 0 : *s1 - *s2; // Return 0 if first n characters match
}

void run_cli() {
    char entered;
    terminal_clear();
    command_length = 0;

    while (1) {
        terminal_print("C:\\Users\\Gabeg>");

        entered = '\0';
        command_length = 0; // Reset command length at the start of each loop

        while (1) {
            entered = terminal_handle_input(); // Read character
            //terminal_put_char(entered);
            if (entered == '\n') {
                break; // Stop when Enter is pressed
            }
            if (entered == '\b'){
                if (command_length > 0) { // Ensure we don't go below zero
                    command_length--; // Move cursor back
                    command[command_length] = '\0'; // Remove last character from the buffer
                    terminal_backspace(); // Handle the visual backspace
                }
            } 
            else if (command_length < MAX_COMMAND_LENGTH - 1 && entered != '\0') { // Prevent overflow
                command[command_length] = entered;
                command_length++;
            }
        }

        command[command_length] = '\0'; // Null-terminate properly

        process_command(); // Process the entered command
    }
}

void process_command() {
    
    if (my_strncmp(command, "ADD ", 4) == 0) {
        add_com();
    }
    else if (my_strncmp(command, "HELP", 4) == 0) {
        help_com();
    }
    else if (!my_strncmp(command, "DIVIDE ", 7)){
        divide_com();
    }
    else if (!my_strncmp(command, "CAT ", 4)){
        terminal_print("Herro from the filesystem\n");
    }
    else if (!my_strncmp(command, "POWER-OFF", 9)){
        disable_cursor();
        power_off();
    }
    else if (!my_strncmp(command, "ECHO ", 5)){
        char *print = command + 5;
        terminal_print(print);
        terminal_print("\n");
    }
    else{
        terminal_print("Invalid Command: Use HELP for more info ");
        terminal_print("\n");
    }
}

void help_com(){
    terminal_print("Supported Commands: ADD <Num> <Num> \n");
    terminal_print("                    DIVIDE <NUM> <NUM> \n");
    terminal_print("                    POWER_OFF, Turns off PC \n");
    terminal_print("                    ECHO \"String\" \n");
    terminal_print("                    DIVIDE <NUM> <NUM> \n");
}


void add_com() {
    char *ptr = command + 3; // Skip "ADD" (3 chars)

    // Step 1: Skip leading spaces
    while (*ptr == ' ') {
        ptr++;
    }

    // Step 2: Extract the first number manually
    int num1 = 0;
    if (!isdigit(*ptr)) { // If first character isn't a digit, invalid input
        terminal_print("Invalid ADD format. Expected: ADD <num1> <num2>\n");
        return;
    }
    while (isdigit(*ptr)) { // Convert ASCII '0'-'9' to an integer
        num1 = num1 * 10 + (*ptr - '0');
        ptr++;
    }

    // Step 3: Skip spaces between numbers
    while (*ptr == ' ') {
        ptr++;
    }

    // Step 4: Extract the second number manually
    int num2 = 0;
    if (!isdigit(*ptr)) { // If no second number, invalid input
        terminal_print("Invalid ADD format. Expected: ADD <num1> <num2>\n");
        return;
    }
    while (isdigit(*ptr)) { // Convert ASCII '0'-'9' to an integer
        num2 = num2 * 10 + (*ptr - '0');
        ptr++;
    }

    // Step 5: Print the extracted numbers for debugging
    terminal_print("Result: ");
    
    char buffer[50];
    int result = num1 + num2;
    int_to_string(buffer, result); // Convert int to string (custom function)
    terminal_print(buffer);

    terminal_print("\n");
}

// Helper function to convert int to string (since sprintf() is unavailable)
void int_to_string(char *buffer, int num) {
    int i = 0;
    int isNegative = 0;

    // Handle negative numbers (if necessary)
    if (num < 0) {
        isNegative = 1;
        num = -num;
    }

    // Extract digits in reverse order
    do {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    } while (num > 0);

    // Add negative sign if needed
    if (isNegative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0'; // Null terminate

    // Reverse the string to correct order
    for (int j = 0, k = i - 1; j < k; j++, k--) {
        char temp = buffer[j];
        buffer[j] = buffer[k];
        buffer[k] = temp;
    }
}

int isdigit(char c) {
    return (c >= '0' && c <= '9'); // Returns 1 if it's a digit, 0 otherwise
}




void divide_com() {
    char *ptr = command + 6; // Skip "ADD" (3 chars)

    // Step 1: Skip leading spaces
    while (*ptr == ' ') {
        ptr++;
    }

    // Step 2: Extract the first number manually
    int num1 = 0;
    if (!isdigit(*ptr)) { // If first character isn't a digit, invalid input
        terminal_print("Invalid DIVIDE format. Expected: DIVIDE <num1> <num2>\n");
        return;
    }
    while (isdigit(*ptr)) { // Convert ASCII '0'-'9' to an integer
        num1 = num1 * 10 + (*ptr - '0');
        ptr++;
    }

    // Step 3: Skip spaces between numbers
    while (*ptr == ' ') {
        ptr++;
    }

    // Step 4: Extract the second number manually
    int num2 = 0;
    if (!isdigit(*ptr)) { // If no second number, invalid input
        terminal_print("Invalid DIVIDE format. Expected: DIVIDE <num1> <num2>\n");
        return;
    }
    while (isdigit(*ptr)) { // Convert ASCII '0'-'9' to an integer
        num2 = num2 * 10 + (*ptr - '0');
        ptr++;
    }

    // Step 5: Print the extracted numbers for debugging
    terminal_print("Result: ");
    
    char buffer[50];
    int result = num1 / num2;
    int_to_string(buffer, result); // Convert int to string (custom function)
    terminal_print(buffer);

    terminal_print("\n");
}
