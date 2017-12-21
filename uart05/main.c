#include "TM4C123GH6PM.h"
#include "queue.h"
#include "serial.h"
#include<stdio.h>

int main(void)
{
    SYSCTL->RCGCGPIO |= 0x20; /* enable clock to GPIOF */
     GPIOF->DIR = 0x08; /* set PORTF3 pin as output (LED) pin */
     /* and PORTF4 as input, SW1 is on PORTF4 */
     GPIOF->DEN = 0x18; /* set PORTF pins 4-3 as digital pins */
     GPIOF->PUR = 0x10; /* enable pull up for pin 4 */

    init_UART0();
    init_UART5();
    __enable_irq(); /* global enable IRQs */
    while (1)
    {
        if (!isEmpty())
        {
            char ch = dequeue();
            if(ch == 'R')
                GPIOF->DATA = 2;
            UART5Tx(ch);
        }
    }
}
