/*
 * serial.h
 *
 *  Created on: 2017. 10. 27.
 *      Author: L
 */

#ifndef SERIAL_H_
#define SERIAL_H_




#include "TM4C123GH6PM.h"
#include <stdlib.h>
void init_serial(void);
char readChar(void);
void printChar(char c);
void printString(char * string);
char *readString(char);

#endif /* SERIAL_H_ */
