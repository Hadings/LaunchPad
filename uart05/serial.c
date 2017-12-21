#include "serial.h"
void init_UART0()
{
    SYSCTL->RCGCUART |= (1 << 0);
    SYSCTL->RCGCGPIO |= (1 << 0);
    GPIOA->AFSEL = (1 << 1) | (1 << 0);
    GPIOA->PCTL = (1 << 0) | (1 << 4);
    GPIOA->DEN = (1 << 0) | (1 << 1);
    UART0->CTL &= ~(1 << 0);
    UART0->IBRD = 104;
    UART0->FBRD = 11;
    UART0->LCRH = (0x3 << 5) | (1 << 4); // 8-bit, no parity, 1-stop bit, FIFO enable
    UART0->CC = 0x0;
    UART0->IM |= 0x0010; /* enable RX interrupt */
    UART0->CTL = (1 << 0) | (1 << 8) | (1 << 9);
    /* enable interrupt in NVIC and set priority to 6 */
    NVIC->IP[5] = 3 << 5; /* set interrupt priority to 4 */
    NVIC->ISER[0] |= 0x00000020; /* enable IRQ6 for UART0 */
}
void init_UART5()
{
    SYSCTL->RCGCUART |= 0x20; /* provide clock to UART5 */
    SYSCTL->RCGCGPIO |= 0x10; /* Enable clock to PORTE */
    /* UART5 initialization */
    UART5->CTL = 0;
    UART5->IBRD = 104;
    UART5->FBRD = 11;
    UART5->CC = 0;
    UART5->LCRH = 0x60;
    UART5->CTL = 0x301; /* enable UART0, TXE, RXE */
    GPIOE->DEN = 0x30;
    GPIOE->AMSEL = 0;
    GPIOE->AFSEL = 0x30;
    GPIOE->PCTL = 0x00110000; /* configure PE5, PE4 for UART5 */
}
void UART5Tx(char c)
{
    while ((UART5->FR & 0x20) != 0); /* wait until Tx buffer not full */
    UART5->DR = c; /* before giving it another byte */
}
void UART0_Handler(void)
{
    volatile int readback;
    char c;
    if (UART0->MIS & 0x0010)
    {
        c = UART0->DR; /* read the received data */
        enqueue(c);

        UART0->ICR = 0x0010; /* clear Rx interrupt flag */
        readback = UART0->ICR; /* a read to force clearing of interrupt flag */
    }
    else
    {
        /* should not get here. But if it does, */
        UART0->ICR = UART0->MIS; /* clear all interrupt flags */
        readback = UART0->ICR; /* a read to force clearing of interrupt flag */
    }
}
