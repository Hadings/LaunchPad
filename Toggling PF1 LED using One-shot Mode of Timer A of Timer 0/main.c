/*Demonstrates the use of TimerA of Timer0 block to do a delay of the multiple of milliseconds. Because 16-bit mode is used, it will only work up to 4 ms. */
#include "TM4C123GH6PM.h"
void timer0A_delayMs(int ttime);
void delayMs(int n);
int main (void){
    /* enable clock to GPIOF at clock gating control register */
    SYSCTL->RCGCGPIO |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2 1) as output */
    GPIOF->DIR = 0x0E;
    /* enable the GPIO pins for digital function */
    GPIOF->DEN = 0x0E;
    while(1)    {
        GPIOF->DATA = 2;        /* turn on red LED */
        timer0A_delayMs(2);     /* Timer A msec delay */
        GPIOF->DATA = 0;        /* turn off red LED */
        delayMs(3);             /* use old delay function */
    }
}

/* millisecond delay using one-shot mode */
void timer0A_delayMs(int ttime){
    SYSCTL->RCGCTIMER |= 1;     /* enable clock to Timer Block 0 */
    TIMER0->CTL = 0;            /* disable Timer before initialization */ /*GTPMCTL의 D0, D8비트를 이용하여 Timer A,B를 enable/disable함 */
    TIMER0->CFG = 0x04;         /* 16-bit option */
    TIMER0->TAMR = 0x01;        /* one-shot mode and down-counter */
    TIMER0->TAILR = 16000*ttime-1;  /* Timer A interval load value register*/
    TIMER0->ICR = 0x1;          /* clear the TimerA timeout flag*/
    TIMER0->CTL |= 0x01;        /* enable Timer A after initialization*/
    while ((TIMER0->RIS & 0x1)==0);  /* wait for TimerA timeout flag to set*/
}

/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n){
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
        {}
/* do nothing for 1 ms */
}
