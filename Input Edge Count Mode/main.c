/*
 * 5modes for each timer block은 총 5가지로 나뉜다.
 * 1. one-shot / Periodic mode
 * 2. Real-time clock mode
 * 3. Input edge-time mode
 * 4. Input edge-count mode
 * 5. Pulse width modulation(PWM)
 *
 *이 중에서 본 source에 해당하는 것은 <Input edge-count mode>
 *교제 5장 P35
 */

/* Square wave signal should be fed to PB6 pin. Make sure it is 3.3 to 5V peak-to-peak. Initialize Timer0A in edge-time mode to capture rising edges. The input pin of Timer0A is PB6. */
#include "TM4C123GH6PM.h"
#include "serial.h"

#include <stdio.h>

void delayMs(int n);


/* The input pin of Timer2A is PF4.*/
void Timer2A_countCapture_init(void){
    SYSCTL->RCGCTIMER |= 4;     /* enable clock to Timer Block 2 */
    SYSCTL->RCGCGPIO |= 0x20;   /* enable clock to PORTF */
    GPIOF->DIR &= ~0x10;        /* make PF4 an input pin */
    GPIOF->DEN |= 0x18;         /* make PF4 a digital pin */
    GPIOF->PUR = 0x10;
    GPIOF->AFSEL |= 0x10;        /* enable alternate function on PF4 */
    GPIOF->PCTL &= ~0x000F0000;  /* configure PF4 as T2CCP0 pin */
    GPIOF->PCTL |= 0x00070000;
    TIMER2->CTL &= ~1;          /* disable TIMER2A during setup */
    TIMER2->CFG = 4;            /* configure as 16-bit timer mode */
    TIMER2->TAMR = 0x13;        /* up-count, edge-count, capture mode */
    TIMER2->TAMATCHR = 0xFFFF;  /* set the count limit */
    TIMER2->TAPMR = 0xFF;   /* to 0xFFFFFF with prescaler */
    TIMER2->CTL &= ~0xC;        /* capture the rising edge */
    TIMER2->CTL |= 1;           /* enable timer2A */
}

int Timer2A_countCapture(void){
    return TIMER2->TAR;
}

int main (void){

    char str[20];
    init_serial();
    printString("Ready\n\r");
    delayMs(500);
    Timer2A_countCapture_init();
    for (int n=0; n<10; n++) {
        delayMs(1000);
        int r = Timer2A_countCapture();
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

