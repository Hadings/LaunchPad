/* This program incorporates the use of prescaler to create a longer delay.
 * Also the use of timer has been changed to TimerA of timer Block 1. */
#include "TM4C123GH6PM.h"
void timer1A_delaySec(int ttime);

int main (void){
    /* enable clock to GPIOF at clock gating control register */
    SYSCTL->RCGCGPIO |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2 1) as output */
    GPIOF->DIR = 0x0E;
    /* enable the GPIO pins for digital function */
    GPIOF->DEN = 0x0E;
    while(1)    {
        GPIOF->DATA = 4;        /* turn on blue LED */
        timer1A_delaySec(1);     /* TimerA 1 sec delay */
        GPIOF->DATA = 0;        /* turn off blue LED */
        timer1A_delaySec(1);    /*TimerA 1 sec delay*/
    }
}

/* multiple of second delays using periodic mode and prescaler*/
void timer1A_delaySec(int ttime){
    int i;
    SYSCTL->RCGCTIMER |= 2;     /* enable clock to Timer Block 1 */
    TIMER1->CTL = 0;            /* disable Timer before initialization */
    TIMER1->CFG = 0x04;         /* 16-bit option */
    TIMER1->TAMR = 0x02;        /* periodic mode and down-counter */
    TIMER1->TAILR = 64000 - 1;  /* TimerA interval load value reg */
    TIMER1->TAPR = 250 - 1;     /* TimerA Prescaler 16MHz/250=64000Hz */
    TIMER1->ICR = 0x1;          /* clear the TimerA timeout flag */
    TIMER1->CTL |= 0x01;        /* enable Timer A after initialization */

    for(i = 0; i < ttime; i++)    {
        while ((TIMER1->RIS & 0x1) == 0);      /* wait for TimerA timeout flag */
        TIMER1->ICR = 0x1;      /* clear the TimerA timeout flag */
    }
}

