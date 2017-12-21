/*program sets up the SysTick to set the COUNT flag at 2 Hz.
 *The system clock is running at 16 MHz.
 *0.5sec / 62.5ns = 8,000,000 for RELOAD register since 1 / 0.5sec = 2Hz
 *The program then polls the flag and counts up a software counter every time COUNT flag is set.
 *The  counter value is written to the tri-color LEDs.
 *The COUNT flag (D16 of CTRL reg.)is cleared when the STCTRL register is read. */
#include "TM4C123GH6PM.h"
int main (void){
    int myCount  = 0;/* enable clock to GPIOF at clock gating control register */
    SYSCTL->RCGCGPIO |= 0x20;/* enable the GPIO pins for the LED (PF3, PF2, and PF1) as output */
    GPIOF->DIR = 0x0E;/* enable the GPIO pins for digital function */
    GPIOF->DEN = 0x0E;/* Configure SysTick */
    SysTick->LOAD = 8000000-1;   /* reload with number of clocks per half second */
    SysTick->CTRL = 5;    /* enable it, no interrupt, use system clock */
    while (1)    {
        if (SysTick->CTRL & 0x10000)   /* if COUNT flag (D16 of CTRL reg.) is set */{
            myCount++;
            GPIOF->DATA = myCount;    /* write the count to LEDs */
        }
    }
}
