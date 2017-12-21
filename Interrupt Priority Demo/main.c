#include "TM4C123GH6PM.h"

void Timer1_init(void);
void Timer2_init(void);
void delayMs(int n);

int main (void)
{
    /* enable clock to GPIOF at clock gating control register */
    SYSCTL->RCGCGPIO |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2 1) as output */
    GPIOF->DIR = 0x0e;
    /* enable the GPIO pins for digital function */
    GPIOF->DEN = 0x0e;
    Timer1_init();
    Timer2_init();
    __enable_irq();
    while(1)
    {
    }
}

void TIMER1A_Handler(void)
{
    volatile int readback;
    GPIOF->DATA |= 2; /* turn on red LED */
    delayMs(500);
    GPIOF->DATA &= ~2; /* turn off red LED */
    TIMER1->ICR = 0x1;
    readback = TIMER1->ICR; /* a read to force clearing of interrupt flag */
}
void TIMER2A_Handler(void)
{
    volatile int readback;
    TIMER2->ICR = 0x1;
    GPIOF->DATA |= 8; /* turn on green LED */
    delayMs(20);
    GPIOF->DATA &= ~8; /* turn off green LED */
    readback = TIMER2->ICR;/* a read to force clearing of interrupt flag */
}

void Timer1_init(void)
{
    SYSCTL->RCGCTIMER |= 2; /* enable clock to Timer Module 1 */
    TIMER1->CTL = 0; /* disable Timer1 before initialization */
    TIMER1->CFG = 0x04; /* 16-bit option */
    TIMER1->TAMR = 0x02; /* periodic mode and up-counter */
    TIMER1->TAPR = 250; /* 16000000 Hz / 250 = 64000 Hz */
    TIMER1->TAILR = 64000; /* 64000 Hz / 64000 = 1 Hz */
    TIMER1->ICR = 0x1; /* clear the Timer1A timeout flag */
    TIMER1->IMR |= 0x01; /* enable Timer1A timeout interrupt */
    TIMER1->CTL |= 0x01; /* enable Timer1A after initialization */
    NVIC->IP[21] = 4 << 5; /* set interrupt priority to 4 */
    NVIC->ISER[0] |= 0x00200000; /* enable IRQ21 (D21 of ISER[0]) */
}

void Timer2_init(void)
{
    SYSCTL->RCGCTIMER |= 4; /* enable clock to Timer Module 2 */
    TIMER2->CTL = 0; /* disable Timer2 before initialization */
    TIMER2->CFG = 0x04; /* 16-bit option */
    TIMER2->TAMR = 0x02; /* periodic mode and up-counter */
    TIMER2->TAPR = 250; /* 16000000 Hz / 250 = 64000 Hz */
    TIMER2->TAILR = 6400; /* 64000 Hz / 6400 = 10 Hz */
    TIMER2->ICR = 0x1; /* clear the Timer1A timeout flag */
    TIMER2->IMR |= 0x01; /* enable Timer2A timeout interrupt */
    TIMER2->CTL |= 0x01; /* enable Timer2A after initialization */
    NVIC->IP[23] = 5 << 5; /* set timer2A interrupt priority to 5 */
    NVIC->ISER[0] |= 0x00800000; /* enable IRQ23 (D23 of ISER[0]) */
}
/* delay n milliseconds (16 MHz CPU clock) */
void delayMs(int n)
{
    int32_t i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
            {} /* do nothing for 1 ms */
}

