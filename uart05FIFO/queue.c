#include"queue.h"
#define CAPACITY 10
uint32_t rear;
uint32_t front;
Item Fifo[CAPACITY];
int32_t size;      // 0 means FIFO empty, FIFOSIZE means full ť���ִ� �������� ����
uint32_t lostData;    // number of lost pieces of data

void OS_FIFO_Init(void){
    rear = front = 0; // Empty
    OS_InitSemaphore(&size, 0); // CurrentSize = 0
    lostData = 0;
}

int OS_FIFO_Put(Item data) { // enqueue ->
    if(size == CAPACITY) {
        lostData++;    // lostData �� ����� ���� �翬�� ���̴�.
        return -1; //full
        }
    else {
            Fifo[rear] = data; // Put
            rear = (rear+1)%CAPACITY;
            OS_Signal(&size );
            return 0; // success
         }
}
Item OS_FIFO_Get(void) { // dequeue ����   ť���� �������� blocked ��Ű�� ���� �翬�� ��
    Item data;
    OS_Wait(&size);
    data = Fifo[front];
    front=(front+1)%CAPACITY; //place to get next
    return data;
}
