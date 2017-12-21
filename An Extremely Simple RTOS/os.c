/*
 * os.c
 *
 *  Created on: 2017. 11. 17.
 *      Author: 19
 */


#include "TM4C123GH6PM.h"
#include <stdint.h>
#include "os.h"
#define NVIC_SYS_PRI3_R (*((volatile uint32_t *)0xE000ED20))
// function definitions in osasm.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void); // Enable interrupts
int32_t StartCritical(void);
void EndCritical(int32_t primask);
void StartOS(void);

#define NUMTHREADS 3 // maximum number of threads
#define STACKSIZE 100 // number of 32-bit words in stack
struct tcb{
 int32_t *sp; // pointer to stack
 struct tcb *next; // link to the tcb of the next thread
};
typedef struct tcb tcbType;
tcbType tcbs[NUMTHREADS];
tcbType *RunPt;
int32_t Stacks[NUMTHREADS][STACKSIZE];

void OS_Init(void){
 DisableInterrupts();
 SysTick->CTRL = 0; // disable SysTick during setup
 SysTick->VAL = 0;
}

void SetInitialStack(int i){
 tcbs[i].sp = &Stacks[i][STACKSIZE-16]; // thread stack pointer
 Stacks[i][STACKSIZE-1] = 0x01000000; // thumb bit, PSR
 Stacks[i][STACKSIZE-3] = 0x14141414; // R14
 Stacks[i][STACKSIZE-4] = 0x12121212; // R12
 Stacks[i][STACKSIZE-5] = 0x03030303; // R3
 Stacks[i][STACKSIZE-6] = 0x02020202; // R2
 Stacks[i][STACKSIZE-7] = 0x01010101; // R1
 Stacks[i][STACKSIZE-8] = 0x00000000; // R0
 Stacks[i][STACKSIZE-9] = 0x11111111; // R11
 Stacks[i][STACKSIZE-10] = 0x10101010; // R10
 Stacks[i][STACKSIZE-11] = 0x09090909; // R9
 Stacks[i][STACKSIZE-12] = 0x08080808; // R8
 Stacks[i][STACKSIZE-13] = 0x07070707; // R7
 Stacks[i][STACKSIZE-14] = 0x06060606; // R6
 Stacks[i][STACKSIZE-15] = 0x05050505; // R5
 Stacks[i][STACKSIZE-16] = 0x04040404; // R4
}


int OS_AddThreads(void(*task0)(void),void(*task1)(void),void(*task2)(void)){
 int32_t status;
 status = StartCritical();
 tcbs[0].next = &tcbs[1]; // 0 points to 1
 tcbs[1].next = &tcbs[2]; // 1 points to 2
 tcbs[2].next = &tcbs[0]; // 2 points to 0
 SetInitialStack(0);
 Stacks[0][STACKSIZE-2] = (int32_t)(task0); // PC
 SetInitialStack(1);
 Stacks[1][STACKSIZE-2] = (int32_t)(task1); // PC
 SetInitialStack(2);
 Stacks[2][STACKSIZE-2] = (int32_t)(task2); // PC
 RunPt = &tcbs[0]; // thread 0 will run first
 EndCritical(status);
 return 1;
}

void OS_Launch(uint32_t theTimeSlice){
 SysTick->LOAD = theTimeSlice - 1; // reload value
 SysTick->CTRL = 3; // enable, core clock and interrupt arm
 StartOS(); // start on the first task
}
