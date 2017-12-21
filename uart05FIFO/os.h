#ifndef __OS_H
#define __OS_H 1
#define TIME_1MS 50000
#define TIME_2MS 2*TIME_1MS
void OS_Init(void);
int OS_AddThreads(void(*task0)(void),
 void(*task1)(void),
 void(*task2)(void));
void OS_Launch(uint32_t theTimeSlice);
void OS_InitSemaphore(int32_t *s, int32_t initVal);

#endif
