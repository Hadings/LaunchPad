/* Toggle red LED on PF1 continuously. Upon pressing either SW1 or SW2, the green
 LED of PF3 should toggle for three times.*/
#include "TM4C123GH6PM.h"
void delayMs(int n);
int main(void)
{
    SYSCTL->RCGCGPIO |= 0x20; /* enable clock to PORTF */
    /* PORTF0 has special function, need to unlock to modify */
    GPIOF->LOCK = 0x4C4F434B; /* unlock commit register */
    GPIOF->CR = 0x01; /* make PORTF0 configurable */
    GPIOF->LOCK = 0; /* lock commit register */
    /* configure PORTF for switch input and LED output */
    GPIOF->DIR &= ~0x11; /* make PORTF0 and 4 input for switch */
    GPIOF->DIR |= 0x0E; /* make PORTF3, 2, 1 output for LEDs */
    GPIOF->DEN |= 0x1F; /* make PORTF4-0 digital pins */
    GPIOF->PUR |= 0x11; /* enable pull up for PORTF0 and 4 */
    /* configure PORTF4, 0 for falling edge trigger interrupt */
    GPIOF->IS &= ~0x11; /* make bit 4, 0 edge sensitive */

    GPIOF->IBE |= 0x11; /* trigger is controlled by IEV */

    GPIOF->IEV &= ~0x11; /* falling edge trigger */
    GPIOF->ICR |= 0x11; /* clear any prior interrupt */
    GPIOF->IM |= 0x11; /* unmask interrupt */
    /* enable interrupt in NVIC and set priority to 3 */
    NVIC->IP[30] = 3 << 5; /* set interrupt priority to 3 */
    NVIC->ISER[0] |= 0x40000000; /* enable IRQ30 (D30 of ISER[0]) */
    __enable_irq(); /* global enable IRQs */
    /* toggle the red LED (PF1) continuously */
    while (1)
    {

    }
}
/* SW1 is connected to PF4 pin, SW2 is connected to PF0. */
/* Both of them trigger PORTF interrupt */

/*
 * GPIOMIS (인터럽트 핀의 분별)
 *
 */
volatile int fla=0;
void GPIOF_Handler(void)
{
    int i;
    volatile int readback;

    while(GPIOF->MIS != 0){
        if(GPIOF->MIS & 0x10 ){
            /* GPIOF4 pin interrupt */
             /* toggle green LED (PF3) three times */
            if(fla %2 != 0){
                GPIOF->DATA |= 0x02;
            }
            if(fla %2 == 0){
                GPIOF->DATA &= ~0x02;
            }
            GPIOF->ICR |= 0x10; /* clear the interrupt flag */
            readback = GPIOF->ICR;
            fla++;
            /* a read to force clearing of interrupt flag */
        }
         else
         {
         /* We should never get here. */
         /* But if we do, clear all pending interrupts. */
             GPIOF->ICR = GPIOF->MIS;
             readback = GPIOF->ICR; /* a read to force clearing of interrupt flag */
         }
    }
}


/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int i, j;
    for (i = 0; i < n; i++)
        for (j = 0; j < 3180; j++)
        {
        } /* do nothing for 1 ms */
}
