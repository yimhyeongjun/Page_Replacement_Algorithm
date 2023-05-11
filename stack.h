#include "node.h"

typedef struct stack{
    struct node* top;
    int numOfData;
}stack;

// stack 초기화 함수
void initStack(stack* stack);

// stack이 비어있는지 확인하는 함수
int isEmptyStack(stack* xstack);

// stack이 가득 차있는지 확인하는 함수
int isFullStack(stack* stack, int maxV);

// stack에 값을 삽입하는 함수 - 가장 위에 있는 데이터 위에 넣기
void push(stack* stack, int data);

// stack에서 값을 추출하는 함수 - 가장 위에 있는 데이터 추출
int pop(stack* stack);