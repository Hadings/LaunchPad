/* Sending "YES" to UART0 on TI ARM Launchpad (TM4C123GH6PM) */
/* UART0 is on USB/Debug */
#include <stdint.h>
#include "TM4C123GH6PM.h"
void UART0Tx(char c);
void delayMs(int n);
int main(void){
    SYSCTL->RCGCUART |= 1;  /* ¨ç provide clock to UART0 */
    SYSCTL->RCGCGPIO |= 1;  /* ¨è enable clock to PORTA */
    /* UART0 initialization */
    UART0->CTL = 0;         /* ¨é disable UART0 */
    UART0->IBRD = 104;      /* ¨ê 16MHz/16=1MHz, 1MHz/104=9600 baud rate */
    UART0->FBRD = 11;       /* ¨ê fraction part, see Example 4-4 */
    UART0->CC = 0;          /* ¨ë use system clock */
    UART0->LCRH = 0x60;     /* ¨ì 8-bit, no parity, 1-stop bit, no FIFO */
    UART0->CTL = 0x301;     /* ¨í,¨î enable UART0, TXE, RXE */

    /* UART0 TX0 and RX0 use PA0 and PA1. Set them up. */
    GPIOA->DEN = 0x03;      /* ¨ï Make PA0 and PA1 as digital */
    GPIOA->AFSEL = 0x03;    /* ¨ð Use PA0,PA1 alternate function */
    GPIOA->PCTL = 0x11;     /* ¨ñ configure PA0 and PA1 for UART */
    delayMs(1);             /* wait for output line to stabilize */
    for(;;)    {
        UART0Tx('Y');
        UART0Tx('E');
        UART0Tx('S');
        UART0Tx(' ');
    }
}

/* This function waits until the transmit buffer is available then */
/* writes the character in the transmit buffer. It does not wait */
/* for transmission to complete. */
void UART0Tx(char c){
    while((UART0->FR & 0x20) != 0); /* ¨ò wait until Tx buffer not full */
    UART0->DR = c;                  /* ¨ó before giving it another byte */
}

void delayMs(int n){
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
        {}  /* do nothing for 1 ms */
}
