#ifndef SERIAL_H
#define SERIAL_H
#include "TM4C123GH6PM.h"
void init_UART0(void);
void init_UART5(void);
void UART5Tx(char c);
#endif
