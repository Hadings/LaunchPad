/*
spin lock semaphore를 사용하라 .
쓰레드가 1개이기 때문에 wait 시킬 필요가 없다.

*/

#include "TM4C123GH6PM.h"
#include <stdint.h>
#include<stdio.h>
#include "os.h"
#include "serial.h"
#include "queue.h"

#define TIMESLICE TIME_2MS // thread switch time in system time units


int32_t count = 0;
int32_t mutex = 1;

char buffer[20];
char* itoa1(int32_t i, char* b);

void counting_op(int32_t thread_id) { // 상호 배제적으로 구현
    OS_Wait(&mutex);
    count++;
    Item t;
    t.thread_id = thread_id;
    t.count = count;
    OS_FIFO_Put(t);
    OS_Signal(&mutex);
}

/* delay n milliseconds (16 MHz CPU clock) */
void doSomethingElse(int n) {
    int i, j;
    for(i = 0 ; i < n; i++)
        for(j = 0; j < 3180; j++)
        {}  /* do nothing for 1 ms */
}
int32_t rend1 = 0;
int32_t rend2 = 0;
int32_t rend3 = 0;


void Task1(void){
    while(1) {
        counting_op(1);
        doSomethingElse(10);

    }
}
void Task2(void){
    while(1) {
        counting_op(2);
        doSomethingElse(20);

    }
}

//Task 1,2 가 프로듀서  Task3 이 컨슈머
void Task3(void){
    while(1) {
        transmitMessage();
        doSomethingElse(30);

    }
}

void transmitMessage() { // 메시지 전달
    Item t = OS_FIFO_Get();
    UART0TxString("Thread ");
    itoa1(t.thread_id, buffer);
    UART0TxString(buffer);
    UART0TxString(": ");
    itoa1(t.count, buffer);
    UART0TxString(buffer);
    UART0TxString("\n\r");
}

int main(void){

 init_UART0();
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

char* itoa1(int32_t i, char* b){
    char const digit[] = "0123456789";
    char* p = b;
    if(i<0){
        *p++ = '-';
        i *= -1;
    }
    int shifter = i;
    do{ //Move to where representation ends
        ++p;
        shifter = shifter/10;
    } while(shifter);
    *p = '\0';
    do{ //Move back, inserting digits as u go
        *--p = digit[i%10];
        i = i/10;
    } while(i);
    return b;
}
