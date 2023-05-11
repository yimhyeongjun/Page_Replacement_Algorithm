#include "userlib.h"

// queue 초기화 함수
void initQueue(queue* queue){
    queue->front = NULL;
    queue->rear = NULL;
    queue->numOfData=0; 
}

// queue가 가득 차 있는지 확인하는 함수
int isEmptyQueue(queue* queue){
    if(queue->numOfData == 0) return 1;
    else return 0;
}

// queue에 값을 삽입하는 함수 - rear로 삽입
void enqueue(queue* queue, int data){
    node* pnode = (node *)malloc(sizeof(node));
    pnode->data = data;
    pnode->next = NULL;

    if(isEmptyQueue(queue)){
        queue->front = pnode;
    }else{
        queue->rear->next = pnode;
    }
    queue->rear = pnode;
    queue->numOfData++;
}

// queue에서 값을 추출하는 함수 - front로 추출
int dequeue(queue* queue){
    int data;
    node* ptr;
    if(isEmptyQueue(queue)){
        fprintf(stderr,"Queue is Empty\n");
        return -1;
    }
    ptr = queue->front;
    queue->front = ptr->next;
    data = ptr->data;
    free(ptr);
    queue->numOfData--;

    return data;
}