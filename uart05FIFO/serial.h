/*
 * Serail.h
 *
 *  Created on: 2017. 11. 24.
 *      Author: 19
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include"TM4C123GH6PM.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#define BUFFER_SIZE 100
extern char strBuffer[];
void init_UART0(void);
void UART0Tx(char c);
void UART0TxString(char * string);




#endif /* SERIAL_H_ */
