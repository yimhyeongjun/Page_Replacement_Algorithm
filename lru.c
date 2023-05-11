#include "userlib.h"

// hit 발생 시
void hit(queue* queue, node* cur, node* bef){
    
    // hit이 발생한 위치에 따라 hit 발생 노드를 rear로 옮겨주고 queue의 front와 rear를 갱신
    if(cur == queue->rear){
        return ;
    }else if(cur == queue->front){
        queue->front = cur->next;
    }else{
        bef->next = cur->next;
    }
    queue->rear->next = cur;
    queue->rear = cur;
    cur->next = NULL;
}

// LRU 알고리즘
// 페이지 프레임에서 가장 오래동안 참조되지 않은 페이지를 교체하는 알고리즘
// Linked List로 구현한 Queue 사용
// hit 발생 시 페이지 프레임에서 hit된 페이지를 Queue의 rear로 옮겨 주어 교체 대상 우선순위에서 가장 나중으로 변경해줘야 함
int lru(int frame_num, int input_type, int* page_stream){

    int fault=0; // 페이지 폴트 발생 횟수를 카운트하는 변수
    queue *page_frame = (queue *)malloc(sizeof(queue)); // 페이지 프레임을 저장하는 queue
    char *fname = "LRU_simulation.txt"; // 시뮬레이션 결과를 저장하는 텍스트 파일명
    int fd;

    // LRU algorithm 수행 시뮬레이션 결과를 저장할 텍스트 파일 오픈
    if((fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }

    // 큐 초기화
    initQueue(page_frame);

    printf("LRU Algorithm Start!\n\n");

    
    // page stream 순차적으로 탐색하며 페이지 교체 정책(LRU)에 따라 교체
    for(int i=0; i<PAGE_NUM; i++){
        
        int input_page = page_stream[i];

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
        node* bef = cur;
        int flag = FAULT;

        // hit 발견 시 처리
        while(cur->next != NULL){
            if(cur->data == input_page){
                flag = HIT;
                // hit 발생시 해당 노드를 rear로 옮겨줘야 함
                hit(page_frame, cur, bef);
                break;
            }
            bef = cur;
            cur = cur->next;
        }
        if(cur->data == input_page){
            flag = HIT;
            hit(page_frame, cur, bef);
        }

        // page fault인 경우
        if(flag == FAULT && page_frame->numOfData == frame_num){ // 페이지가 가득 찬 경우
            dequeue(page_frame);
            enqueue(page_frame, input_page);
        }else if(flag == FAULT && page_frame->numOfData < frame_num){ // 페이지가 가득 차지 않은 경우
            enqueue(page_frame, input_page);
        }
        
        // 결과 확인 출력문
        if(flag == HIT){ // hit인 경우
            // 페이지 프레임 상태 출력 및 파일에 저장
            print_page_frame_queue(i+1, page_frame, frame_num, HIT, input_page);
            make_sim_file_queue(fd, i+1, page_frame, frame_num, HIT, input_page);
        }else{ // page fault인 경우
            // 페이지 프레임 상태 출력 및 파일에 저장
            fault++;
            print_page_frame_queue(i+1, page_frame, frame_num, FAULT, input_page);
            make_sim_file_queue(fd, i+1, page_frame, frame_num, FAULT, input_page);
        }
        
    }

    close(fd);
    free(page_frame);
    return fault;
}