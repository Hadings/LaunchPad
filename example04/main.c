#include "TM4C123GH6PM.h"
#include "queue.h"
#include "serial.h"
void delayMs(int n);

int main(void)
{
 SYSCTL->RCGCGPIO |= 0x20; /* enable clock to GPIOF at clock gating control register */
 GPIOF->DIR = 0x0E; /* enable the GPIO pins for the LED (PF3, 2 1) as output */
 GPIOF->DEN = 0x0E; /* enable the GPIO pins for digital function */
 GPIOF->DATA = 0x02; /* turn on red LED only and leave it on */


 init_UART0();
 init_UART5();
 __enable_irq(); /* global enable IRQs */
 int i;
 while (1)
 {
     if (!isEmpty())
     {
         char ch = dequeue();
         if(ch == 'R'){
             for (i = 0; i < 3; i++)
                 {
                    GPIOF->DATA |= 0x08;
                     delayMs(500);
                     GPIOF->DATA &= ~0x08;
                     delayMs(500);
                }

         }

         GPIOF->DATA = 0;
         UART5Tx(ch);
     }
 }

}
void delayMs(int n)
{
 int i;
 for(i = 0 ; i < n; i++) {}
}
