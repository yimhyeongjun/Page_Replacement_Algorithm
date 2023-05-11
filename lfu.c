#include "userlib.h"

// 교체
void page_out(queue* page_frame, int* refer, int input_page, int minR){
    node* tcur = page_frame->front;
    node* bcur = tcur;
    int ff = 1;

    while(tcur->next != NULL){
        // 현재까지의 최소 참조 횟수보다 현재 노드의 참조 횟수가 작으면
        if(refer[tcur->data] == minR){
        // 교체 페이지를 dequeue 후 참조 횟수 0으로 초기화
            // 만약 tcur이 rear에 위치한다면
            if(tcur == page_frame->rear){
                page_frame->rear = bcur;
            }else if(tcur == page_frame->front){
                page_frame->front = tcur->next;
            }else{
                ;
            }
            refer[tcur->data] = 0;
            node* temp = tcur;
            bcur->next = tcur->next;
            temp->next = NULL;
            page_frame->numOfData--;
            free(temp);
            // 새 페이지를 enqueue 후 참조 횟수를 1 증가
            enqueue(page_frame, input_page);
            refer[input_page] = 1;
            ff = 0;
            break;
        }
        bcur = tcur;
        tcur = tcur->next;
    }

    // 마지막 노드까지 확인
    if(ff && refer[tcur->data] == minR){
        // 만약 tcur이 rear에 위치한다면
        if(tcur == page_frame->rear){
            page_frame->rear = bcur;
        }else if(tcur == page_frame->front){
            page_frame->front = tcur->next;
        }else{
            ;
        }

        refer[tcur->data] = 0;
        node* temp = tcur;
        bcur->next = tcur->next;
        temp->next = NULL;
        page_frame->numOfData--;
        free(temp);
            
        // 새 페이지를 enqueue 후 참조 횟수를 1증가
        enqueue(page_frame, input_page);
        refer[input_page] = 1;
    }

}

// LFU 알고리즘
// 페이지 프레임에서 가장 적게 참조된 페이지를 교체하는 알고리즘
// 각 페이지마다 참조횟수를 저장하는 배열이 필요함
// 최저 참조 횟수가 같을 경우 가장 먼저 들어온 것 부터 교체 FIFO
// 기본적으로 FIFO 구조에 참조 횟수 비교
// Linked List로 구현한 Queue 사용
int lfu(int frame_num, int input_type, int* page_stream){

    int fault=0; // 페이지 폴트 발생 횟수 저장하는 변수 
    queue *page_frame = (queue *)malloc(sizeof(queue)); // 페이지 프레임을 저장하는 queue
    int *refer = (int *)malloc(sizeof(int) * (SEED+1)); // 페이지의 참조 횟수를 저장하는 배열
    char *fname = "LFU_simulation.txt"; // 시뮬레이션 결과를 저장할 텍스트 파일명
    int fd;

    // LFU algorithm 수행 시뮬레이션 결과를 저장할 텍스트 파일 오픈
    if((fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }

    // Queue 초기화
    initQueue(page_frame);
    // 참조 횟수 0으로 초기화
    memset(page_frame, 0, sizeof(int) * (SEED + 1));

    // page stream 순차적으로 탐색하며 페이지 교체 정책(LFU)에 따라 교체
    for(int i=0; i<PAGE_NUM; i++){
        
        int input_page = page_stream[i];

        // page_frame이 비어 있는 경우 : 첫 번째 경우
        if(isEmptyQueue(page_frame)){
            enqueue(page_frame, input_page);
            refer[input_page] = 1;
            fault++;
            print_page_frame_lfu(i+1, page_frame, refer, frame_num, FAULT, input_page);
            make_sim_file_lfu(fd, i+1, page_frame, refer, frame_num, FAULT, input_page);
            continue;
        }

        // page_frame을 확인하여 hit인 경우와 page fault인 경우를 처리
        node* cur = page_frame->front;
        int flag = FAULT; // hit을 표현하는 flag

        // 페이지를 탐색하며 hit이 존재하는 지 확인
        while(cur->next != NULL){
            // hit이 존재한다면
            if(cur->data == input_page){
                // hit flag 변경
                flag = HIT;
                // 참조 횟수 1증가
                refer[input_page]++;
                break;
            }
            cur = cur->next;
        }
        // 마지막 노드까지 hit 확인
        if(cur->data == input_page && flag == FAULT){
            flag = HIT;
            refer[input_page]++;
        }
        
        // page fault인 경우
        if(flag == FAULT){ 
            // 페이지 프레임이 가득 차 있지 않은 경우
            if(page_frame->numOfData < frame_num){
                enqueue(page_frame, input_page);
                refer[input_page] = 1;
            }else if(page_frame->numOfData == frame_num){ // 페이지 프레임이 가득 차 있는 경우
                
                int minR = PAGE_NUM*2; // 가장 작은 참조 횟수를 저장하는 변수
                int ff = 1;
                // Linked List 탐색을 위한 커서
                node* tcur = page_frame->front;

                // 각 프레임마다 참조 횟수를 확인하여 가장 작은 참조 횟수를 저장
                while(tcur->next != NULL){
                    // 현재까지의 최소 참조 횟수보다 현재 노드의 참조 횟수가 작으면
                    if(refer[tcur->data] < minR){
                        minR = refer[tcur->data];
                    }
                    tcur = tcur->next;
                }
                // 마지막 노드까지 확인
                if(refer[tcur->data] < minR){
                    minR = refer[tcur->data];
                }
                
                // 가장 작은 참조 회수를 가진 값을 front부터 탐색하여 발견 시 dequeue 후 enqueue
                page_out(page_frame, refer, input_page, minR);
                
            }
        }
        

        // 결과 확인 출력문
        if(flag == HIT){ // hit인 경우
            print_page_frame_lfu(i+1, page_frame, refer, frame_num, HIT, input_page);
            make_sim_file_lfu(fd, i+1, page_frame, refer, frame_num, HIT, input_page);
        }else{ // page fault인 경우
            fault++;
            print_page_frame_lfu(i+1, page_frame, refer, frame_num, FAULT, input_page);
            make_sim_file_lfu(fd, i+1, page_frame, refer, frame_num, FAULT, input_page);
        }
        
    }

    close(fd);
    free(page_frame);
    return fault;
}

