/*
 * �ǽ����� Launchpad�� 1�� ����ġ�� ���� Ƚ���� ī��Ʈ�Ͽ� serial�� �����ϴ� ���α׷��� �ۼ��϶�.
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

