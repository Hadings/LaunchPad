/*
 * RTOS�� �����
 * ���⼭��  Systick�� Ȱ���Ͽ� ���� ���̴�.
 * ���� �����ؼ� �� �� ����.
 * Realtime OS��� �� �� �ִ�.
 * ���⼭�� �ش������� ������ OS�� �����Ѵ�.
 * ��Ƽ �������� �ϴ� �ý��� ����
 * ���μ��� vs ������
 * ���μ��� : �������� ���α׷� /
 * ������ : CPU ����, ���� �� �������� �ʴ´�.  => CPU ���� +  ������ �����尡 ������ �ְ� �������� �� �����ȴ�.

 GPIOD -> GPIOF �� ��ȯ�Ѵ�.
 SYSCTL->RCGCGPIO & 0x10 => SYSCTL->RCGCGPIO & 0x20 �� ��ȯ�Ѵ�.

 GPIOF�� ��Ʈ�� �ϱ����� ��ȯ��.
*/

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include "os.h"
#define TIMESLICE TIME_2MS // thread switch time in system time units
uint32_t Count1; // number of times thread1 loops
uint32_t Count2; // number of times thread2 loops
uint32_t Count3; // number of times thread3 loops

void Task1(void){
    Count1 = 0;
    for(;;){
        Count1++;
        GPIOF->DATA ^= 0x02; // toggle PD1
    }
}
void Task2(void){
    Count2 = 0;
    for(;;){
        Count2++;
        GPIOF->DATA ^= 0x04; // toggle PD2
    }
}
void Task3(void){
    Count3 = 0;
    for(;;){
        Count3++;
    GPIOF->DATA ^= 0x08; // toggle PD3
    }
}

int main(void){
    OS_Init(); // initialize, disable interrupts, 50 MHz
    SYSCTL->RCGCGPIO |= 0x20; // activate clock for Port D
    while((SYSCTL->RCGCGPIO & 0x20) == 0){}; // allow time to stabilize
    GPIOF->DIR |= 0x0E;
    GPIOF->AFSEL &= ~0x0E; // disable alt funct on PD3-1
    GPIOF->DEN |= 0x0E; // enable digital I/O on PD3-1
    GPIOF->PCTL &= 0xFFFF000F; // configure PD3-1 as GPIO
    GPIOF->AMSEL &= ~0x0E; // disable analog functionality on PD3-1
    OS_AddThreads(&Task1, &Task2, &Task3);
    OS_Launch(TIMESLICE); // doesn't return, interrupts enabled in here
    return 0; // this never executes
}