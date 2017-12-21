/*
 * 실습과제 Launchpad의 1번 스위치를 누른 횟수를 카운트하여 serial로 전송하는 프로그램을 작성하라.
 */

/* Square wave signal should be fed to PB6 pin. Make sure it is 3.3 to 5V peak-to-peak. Initialize Timer0A in edge-time mode to capture rising edges. The input pin of Timer0A is PB6. */
#include "TM4C123GH6PM.h"
#include <stdio.h>
#include "serial.h"
#include "timer.h"
#include <stdlib.h>


int main(void)
{
    char str[20];
    init_serial();

    printString("Ready\n\r");
    printString("\n\r");

    Timer2A_countCapture_init();

    int r = 0,pre =0;
    while (1)
    {
        r = Timer2A_countCapture();
        if (r != pre)
        {
            sprintf(str, "%d,\n\r", r);
            printString(str);
            pre = r;
        }
    }
    printString("End.\n\r");
}

