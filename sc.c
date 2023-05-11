#include "userlib.h"

// use bit가 0인 페이지 중 가장 먼저 발견되는 페이지를 교체 대상으로 선정하는 알고리즘
// 커서가 존재 -> 커서가 이동하면서 페이지의 use bit를 확인하고 커서가 이동하면서 use bit를 0으로 변경
int sc(int frame_num, int input_type, int* page_stream){

    int fault = 0; // 페이지 폴트 발생 횟수를 카운트하는 변수
    int *page_frame  = (int *)malloc(sizeof(int) * frame_num); // 페이지 프레임을 저장하는 배열 -> Circular Queue 형태
    int *clock = (int *)malloc(sizeof(int) * frame_num); // 페이지 프레임에 저장된 페이지의 참조 여부를 저장하는 clokc 배열 -> Circular Queue 형태
    char *fname = "SC_simulation.txt"; // 시뮬레이션 결과를 저장하는 텍스트 파일명
    int fd;

    // SC algorithm 수행 시뮬레이션 결과를 저장할 텍스트 파일 오픈
    if((fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }

    // page_frame, clock 배열 0으로 초기화
    memset(page_frame, 0, sizeof(int) * frame_num);
    memset(clock, 0, sizeof(int) * frame_num);

    // queue의 front를 구하기 위해 모듈러 연산에 활용될 커서
    int cursor = 0;

    printf("SC Algorithm Start!\n\n");

    // page stream 순차적으로 탐색하며 페이지 교체 정책(SC)에 따라 교체
    for(int i=0; i<PAGE_NUM; i++){
        
        int input_page = page_stream[i]; // 현재 들어온 페이지
        int flag = FAULT; // 페이지 폴트 여부 저장하는 플래그 변수

        // hit 검사
        for(int j=0; j<frame_num; j++){
            // 페이지가 참조되면 clock 비트 1로 수정
            if(page_frame[j] == input_page){
                clock[j] = 1;
                // 플래그 변경
                flag = HIT;
                break;
            }
        }

        // 시계 알고리즘 : 모든 페이지가 환형 구조
        // page fault인 경우
        if(flag == FAULT){
            // 페이지를 교체할 때 운영체제는 현재 바늘이 가리키고 있는 페이지의 use bit가 1인지 검사
            while(1){
               // 큐 형태의 clock 배열의 front를 가리키고 있는 커서
               int front = cursor % frame_num;
               // use bit가 0인 경우
               if(!clock[front]){
                    // use bit를 1로 바꾸기
                    clock[front] = 1;
                    // 페이지 교체
                    page_frame[front] = input_page;
                    // page_fault 1 증가
                    fault++;
                    // 참조 커서 1증가
                    cursor++;
                    break;
               }else{ // 만약 1이라면 페이지는 교체 대상이 아님 -> 0으로 변경 후 다음 페이지 검사
                   clock[front] = 0;
                   cursor++;
               }

            }
        }

         // 결과 확인 출력문
        if(flag == FAULT){ // page fault인 경우
            // 페이지 프레임 상태 출력 및 파일에 저장
            print_page_frame_sc(i+1, page_frame, clock, frame_num, FAULT, cursor % frame_num, input_page);
            make_sim_file_sc(fd, i+1, page_frame, clock, frame_num, FAULT, cursor % frame_num, input_page);
             
        }else{ // hit인 경우
            // 페이지 프레임 상태 출력 및 파일에 저장
            print_page_frame_sc(i+1, page_frame, clock, frame_num, HIT, cursor % frame_num, input_page);
            make_sim_file_sc(fd, i+1, page_frame, clock, frame_num, HIT, cursor % frame_num, input_page);
        }
            
    }

    close(fd);
    return fault;
}