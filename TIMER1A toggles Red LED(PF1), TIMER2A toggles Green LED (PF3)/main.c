#include "TM4C123GH6PM.h"
int main (void)
{
 /* enable clock to GPIOF at clock gating control register */
 SYSCTL->RCGCGPIO |= 0x20;
 /* enable the GPIO pins for the LED (PF3, 2 1) as output */
 GPIOF->DIR = 0x0e;
 /* enable the GPIO pins for digital function */
 GPIOF->DEN = 0x0e;
 timer1A_init(); /* setup timer1A interrupt */
 timer2A_init(); /* setup timer2A interrupt */
 __enable_irq(); /* global enable IRQs */
 while(1)
 {
 GPIOF->DATA ^= 4; /* toggle blue LED */
 delayMs(500); /* wait for half second */
 }
}

/* Setup Timer1A to create 1 Hz interrupt */
void timer1A_init(void)
{
 SYSCTL->RCGCTIMER |= 2; /* enable clock to Timer Block 1 */
 TIMER1->CTL = 0; /* disable Timer1 before initialization */
 TIMER1->CFG = 0x04; /* 16-bit option */
 TIMER1->TAMR = 0x02; /* periodic mode and down-counter */
 TIMER1->TAPR = 250; /* 16000000 Hz / 250 = 64000 Hz */
 TIMER1->TAILR = 64000; /* 64000 Hz / 64000 = 1 Hz */
 TIMER1->ICR = 0x1; /* clear the Timer1A timeout flag */
 TIMER1->IMR |= 0x01; /* enable Timer1A timeout interrupt */
 TIMER1->CTL |= 0x01; /* enable Timer1A after initialization */
 NVIC->ISER[0] |= 0x00200000; /* enable IRQ21 (D21 of ISER[0]) */
}

/* Setup Timer2A to create 10 Hz interrupt */
void timer2A_init(void)
{
 SYSCTL->RCGCTIMER |= 4; /* enable clock to Timer Block 2 */
 TIMER2->CTL = 0; /* disable Timer2 before initialization */
 TIMER2->CFG = 0x04; /* 16-bit option */
 TIMER2->TAMR = 0x02; /* periodic mode and down-counter */
 TIMER2->TAPR = 250; /* 16000000 Hz / 250 = 64000 Hz */
 TIMER2->TAILR = 6400; /* 64000 Hz / 6400 = 10 Hz */
 TIMER2->ICR = 0x1; /* clear the Timer2A timeout flag */
 TIMER2->IMR |= 0x01; /* enable Timer2A timeout interrupt */
 TIMER2->CTL |= 0x01; /* enable Timer2A after initialization */
 NVIC->ISER[0] |= 0x00800000; /* enable IRQ23 (D23 of ISER[0]) */
}

void TIMER1A_Handler(void)
{
 volatile int readback;
 if (TIMER1->MIS & 0x1) /* Timer1A timeout flag */
 {
 GPIOF->DATA ^= 2; /* toggle red LED */
 TIMER1->ICR = 0x1; /* clear the Timer1A timeout flag */
 readback = TIMER1->ICR; /* to force clearing of interrupt flag */
 }
 else
 { /* should not get here, but if we do */
 TIMER1->ICR = TIMER1->MIS; /* clear all flags */
 readback = TIMER1->ICR; /* to force clearing of interrupt flag */
 }
}

void TIMER2A_Handler(void)
{
 volatile int readback;
 if (TIMER2->MIS & 0x1) /* Timer2A timeout flag */
 {
 GPIOF->DATA ^= 8; /* toggle green LED */
 TIMER2->ICR = 0x1; /* clear the Timer2A timeout flag */
 readback = TIMER2->ICR; /* to force clearing of interrupt flag */
 }
 else
 { /* should not get here, but if we do */
 TIMER2->ICR = TIMER2->MIS; /* clear all flags */
 readback = TIMER2->ICR; /* to force clearing of interrupt flag */
 }
}
/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
 int i, j;
 for(i = 0 ; i < n; i++)
 for(j = 0; j < 3180; j++)
 {} /* do nothing for 1 ms */
}
