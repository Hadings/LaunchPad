#include"serial.h"
char strBuffer[BUFFER_SIZE];
char *promptMsg = "Type something and press enter: ";
char echoMes[] = "You typed: ";
const char delim = '\r';
char * const newlineMsg = "\n\r";
int main(void) {
    init_serial();
    while(1)     {
        printString(promptMsg);
        readString(delim);
        printString(newlineMsg);
        printString(echoMes);
        printString(strBuffer);
        printString(newlineMsg);
    }
}
