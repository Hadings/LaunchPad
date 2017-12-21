#ifndef QUEUE_H_
#define QUEUE_H_

#include <stdint.h>
#include "os.h"
typedef struct item {
    int32_t thread_id;
    uint32_t count;
} Item;
void OS_FIFO_Init(void);
int OS_FIFO_Put(Item data);
Item OS_FIFO_Get(void);

#endif /* QUEUE_H_ */
