#include "userlib.h"


// FIFO 알고리즘
// 페이지 프레임에 가장 먼저 들어온 페이지를 교체하는 알고리즘
// Linked List로 구현한 Queue를 사용한다.
int fifo(int frame_num, int input_type, int* page_stream){

    int fault=0;  // 페이지 폴트 발생 횟수 저장하는 변수 
    queue *page_frame = (queue *)malloc(sizeof(queue)); // 페이지 프레임을 저장하는 queue
    char *fname = "FIFO_simulation.txt";  // 시뮬레이션 결과를 저장할 텍스트 파일명
    int fd;

    // fifo algorithm 수행 시뮬레이션 결과를 저장할 텍스트 파일 오픈
    if((fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }

    // 큐 초기화
    initQueue(page_frame);

    printf("FIFO Algorithm Start!\n\n");

    // page stream 순차적으로 탐색하며 페이지 교체 정책(FIFO)에 따라 교체
    for(int i=0; i<PAGE_NUM; i++){

        int input_page = page_stream[i]; // 페이지 스트림에서 현재 읽어 들인 페이지

        // page_frame이 비어 있는 경우 : 첫 번째 경우
        if(isEmptyQueue(page_frame)){
            // 페이지 프레임 상태 출력 및 파일에 저장
            enqueue(page_frame, input_page);
            print_page_frame_queue(i+1, page_frame, frame_num, FAULT, input_page);
            make_sim_file_queue(fd, i+1, page_frame, frame_num, FAULT, input_page);
            fault++;
            continue;
        }

        // page_frame을 확인하여 hit인 경우와 page fault인 경우를 처리
        node* cur = page_frame->front;
        int flag = FAULT; // page fault를 표현하는 flag

        // 페이지를 탐색하며 hit이 존재하는 지 확인
        while(cur->next != NULL){
            if(cur->data == input_page){
                flag = HIT;
                break;
            }
            cur = cur->next;
        }
        if(cur->data == input_page && flag == FAULT){
            flag = HIT;
        }
        
        // page fault인 경우
        if(flag == FAULT){ 
            // 페이지 프레임이 가득 차 있는 경우
            if(page_frame->numOfData == frame_num){
                dequeue(page_frame); // 가장 먼저 들어온 것을 하나 빼줌
            }
            enqueue(page_frame, input_page);
            fault++;
        }

        // 결과 확인 출력문
        if(flag == HIT){ // hit인 경우
            // 페이지 프레임 상태 출력 및 파일에 저장
            print_page_frame_queue(i+1, page_frame, frame_num, HIT, input_page);
            make_sim_file_queue(fd, i+1, page_frame, frame_num, HIT, input_page);
        }else{ // page fault인 경우
            // 페이지 프레임 상태 출력 및 파일에 저장
            print_page_frame_queue(i+1, page_frame, frame_num, FAULT, input_page);
            make_sim_file_queue(fd, i+1, page_frame, frame_num, FAULT, input_page);
        }
        
    }
    
    close(fd);
    free(page_frame);
    return fault;
}