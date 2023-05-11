#include "userlib.h"

// stack 초기화 함수
void initStack(stack* stack){
    stack->top = NULL;
    stack->numOfData = 0;
}

// stack이 비어있는지 확인하는 함수
int isEmptyStack(stack* stack){
    if(stack->numOfData == 0) return 1;
    else return 0;
}

// stack이 가득 차 있는지 확인하는 함수
int isFullStack(stack* stack, int maxV){
    if(stack->numOfData >= maxV) return 1;
    else return 0;
}

// stack에 값을 삽입하는 함수 - 가장 위에 있는 데이터 위에 넣기
void push(stack* stack, int data){
    node* newData = (node *)malloc(sizeof(node));
    newData->data = data;
    newData->next = NULL;

    if(!isEmptyStack(stack)){
        newData->next = stack->top;
    } 
    stack->top = newData;
    stack->numOfData++;
}

// stack에서 값을 추출하는 함수 - 가장 위에 있는 데이터 추출
int pop(stack* stack){
    int data;
    node* ptr;
    if(isEmptyStack(stack)){
        fprintf(stderr, "Stack is Empty\n");
        return -1;
    }
    ptr = stack->top;
    data = ptr->data;
    stack->top = ptr->next;
    free(ptr);
    stack->numOfData--;
    
    return data;
}