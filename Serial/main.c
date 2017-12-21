#include "TM4C123GH6PM.h"
#include "serial.h"
#define MAX 100
int main(void)
{
    init_serial();
    while (1)
    {
        printString("Type something and press enter: ");
        char *string = readString('\r');
        printString("\n\r");
        printString("You typed: ");
        printString(string);
        printString("\n\r");
        free(string);
    }
}
