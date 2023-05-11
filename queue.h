#include "node.h"

typedef struct queue{
    struct node* rear;
    struct node* front;
    int numOfData;
}queue;

// queue 초기화 함수
void initQueue(queue* queue);

// queue가 비어있는지 확인하는 함수
int isEmptyQueue(queue* queue);

// queue에 데이터를 삽입하는 함수 - rear로 삽입
void enqueue(queue* queue, int data);

// queue에서 데이터를 추출하는 함수 - front로 추출
int dequeue(queue* queue);