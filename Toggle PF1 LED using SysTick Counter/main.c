/* Toggle PF1 LED every 1 second using the SysTick Counter */
/* The system clock is running at 16 MHz. 1sec / 62.5ns = 16,000,000 for RELOAD
 *  register. The program then polls the flag and toggles the red LED PORTF1
 *   every time COUNT flag is set.
 *   The COUNT flag is cleared when the STCTRL register is read. */
#include "TM4C123GH6PM.h"
int main (void){
    /* enable clock to GPIOF at clock gating control register */
    SYSCTL->RCGCGPIO |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2 1) as output */
    GPIOF->DIR = 0x0E;
    /* enable the GPIO pins for digital function */
    GPIOF->DEN = 0x0E;
    /* Configure SysTick */
    SysTick->LOAD = 16000000 - 1;
    /* reload with number of clocks per second */
    SysTick->CTRL = 5;
    /* enable it, no interrupt, use system clock */
    while (1)    {
        if (SysTick->CTRL & 0x10000) /* if COUNT (D16 of CTRL reg.) flag is set */
            GPIOF->DATA ^= 2;    /* toggle PORTF1 red LED */
    }
}
