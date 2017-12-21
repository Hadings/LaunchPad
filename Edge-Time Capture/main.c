/*
 * 5modes for each timer block은 총 5가지로 나뉜다.
 * 1. one-shot / Periodic mode
 * 2. Real-time clock mode
 * 3. Input edge-time mode
 * 4. Input edge-count mode
 * 5. Pulse width modulation(PWM)
 *
 *이 중에서 본 source에 해당하는 것은 <Input edge-time mode>
 *교제 5장 P35
 */

/* Square wave signal should be fed to PB6 pin. Make sure it is 3.3 to 5V peak-to-peak. Initialize Timer0A in edge-time mode to capture rising edges. The input pin of Timer0A is PB6. */
#include "TM4C123GH6PM.h"
#include "serial.h"
#include <stdio.h>

void delayMs(int n);
void Timer0Capture_init(void){
    SYSCTL->RCGCTIMER |= 1;     /* enable clock to Timer Block 0 */
    SYSCTL->RCGCGPIO |= 2;      /* enable clock to PORTB */
    GPIOB->DIR &= ~0x40;        /* make PB6 an input pin */
    GPIOB->DEN |= 0x40;         /* make PB6 as digital pin */
    GPIOB->AFSEL |= 0x40;       /* use PB6 alternate function */
    GPIOB->PCTL &= ~0x0F000000;  /* configure PB6 for T0CCP0 */
    GPIOB->PCTL |= 0x07000000;
    TIMER0->CTL &= ~1;          /* disable timer0A during setup */
    TIMER0->CFG = 4;            /* 16-bit timer mode */
    TIMER0->TAMR = 0x17;        /* up-count, edge-time, capture mode */
    TIMER0->CTL &= ~0x0C;        /* capture the rising edge */
    TIMER0->CTL |= 1;           /* enable timer0A */
}

/* This function captures two consecutive rising edges of a periodic signal from Timer Block 0 Timer A and returns the time difference (the period of the signal). */
int Timer0A_periodCapture(void){
    int lastEdge, thisEdge;

    /* capture the first rising edge */
    TIMER0->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER0->RIS & 4) == 0) ;    /* wait till captured */
    lastEdge = TIMER0->TAR;     /* save the timestamp */

    /* capture the second rising edge */
    TIMER0->ICR = 4;            /* clear timer0A capture flag */
    while((TIMER0->RIS & 4) == 0) ;    /* wait till captured */
    thisEdge = TIMER0->TAR;     /* save the timestamp */
    return (thisEdge - lastEdge) & 0x00FFFFFF; /* return the time difference */
}

int main (void){
    char str[20];
    init_serial();
    printString("Ready\n\r");
    delayMs(500);
    Timer0Capture_init();
    for (int n=0; n<10; n++) {
        delayMs(1000);
        int r = Timer0A_periodCapture();
        sprintf(str, "%d\n\r", r);
        printString(str);
    }
    printString("End.\n\r");
}

void delayMs(int n)
{
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
        {} /* do nothing for 1 ms */
}

