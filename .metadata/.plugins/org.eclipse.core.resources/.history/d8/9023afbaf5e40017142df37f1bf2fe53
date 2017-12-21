#include "tm4c123gh6pm.h"
#include "queue.h"
#include "serial.h"
int main(void)
{
    init_UART0();
    init_UART5();
    __enable_irq(); /* global enable IRQs */
    while (1)
    {
        if (!isEmpty())
        {
            char ch = dequeue();
            UART5Tx(ch);
        }
    }
}
