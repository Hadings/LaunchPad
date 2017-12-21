#define QUEUE_SIZE 3
char queue[QUEUE_SIZE]; // circular array
int front = 0, rear = 0, size = 0;
int enqueue(char ch)
{
    if (size == QUEUE_SIZE)
        return 0;
    rear = (rear + 1) % QUEUE_SIZE;
    queue[rear] = ch;
    size++;
    return 1;
}
int dequeue()
{
    if (size == 0)
        return -1;
    char result = queue[front];
    front = (front + 1) % QUEUE_SIZE;
    size--;
    return result;
}
int isEmpty()
{
    return size == 0;
}
