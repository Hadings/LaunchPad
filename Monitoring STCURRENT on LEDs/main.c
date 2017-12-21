/*
 * This program let the SysTick counter run freely and dumps the counter values to the tri-color LEDs continuously.
 * The counter value is shifted 20 places to the right so that the changes of LEDs will be slow enough to be visible.
 *이 프로그램은 SysTick 카운터가 자유롭게 작동하고 카운터 값을 삼색 LED에 지속적으로 덤프시킵니다.
 카운터 값은 LED의 변화가 보이기에 충분히 느릴 수 있도록 20 번 오른쪽으로 이동됩니다.
 * */

#include "TM4C123GH6PM.h"
int main (void){
    int x;
    /* enable clock to GPIOF at clock gating control register */
    SYSCTL->RCGCGPIO |= 0x20;
    /* enable the GPIO pins for the LED (PF3, 2 1) as output */
    GPIOF->DIR = 0x0E;
    /* enable the GPIO pins for digital function */
    GPIOF->DEN = 0x0E;

    /* Configure SysTick */
    SysTick->LOAD = 0xFFFFFF;   /* reload reg. with max value */
    SysTick->CTRL = 5;          /* enable it, no interrupt, use system clock */
    while (1)    {
        x = SysTick->VAL;   /* read current value of down counter */
        x = x >> 20;        /* shift right to slow down the rate */
        GPIOF->DATA = x;    /* dump it to the LEDs */
    }
}
