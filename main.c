#include "userlib.h"

int frame_num, input_type; // frame_num : 프레임의 개수, input_type : 데이터 입력 방식
int algo[9] = {0,}; // 선택된 알고리즘 종류
int fault[9] = {0, };
int page_stream[PAGE_NUM]; // 페이지 스트림이 저장된 배열
int rwbit[PAGE_NUM][2]; // 페이지 스트림의 W와 R 비트
char *fname; // 인자로 들어온 파일 명

int main(int argc, char* argv[]){

    char ch;
    int algo_num = 0; // 선택한 알고리즘 개수
    fname = argv[1]; // 데이터 입력 방법 중 사용자 입력 선택 시 main함수 인자로 받은 파일명 저장

    system("clear");

    // main 함수 인자의 개수가 최대 2개
    if(argc > 2){
        fprintf(stderr, "Usage : %s file\n", argv[0]);
        exit(1);
    }
    printf("Page Replacement 알고리즘 시큘레이터를 선택하시오.\n");
    printf("(1)Optimal  (2)FIFO  (3)LIFO  (4)LRU  (5)LFU  (6)SC  (7)ESC  (8)All\n");
    printf("원하는 알고리즘의 번호를 입력하시오.(최대 3개, All 입력시 모든 알고리즘 선택) : ");

    // Optimal은 필수적으로 해야 함
    algo[1] = 1;

    // 원하는 알고리즘 입력 및 알고리즘 개수 체크
    while((ch = getchar()) != '\n'){ // '\n' 받으면 종료 조건
        if((ch - '0') > 0 && (ch - '0') < 8){
            algo[ch - '0'] = 1;
            algo_num++;
        }else if((ch - '0') == 8){
            for(int i=1; i<8; i++)
                algo[i] = 1;
            algo_num = 8;
        }
    }

    // 알고리즘은 1 - 3개 까지만 입력 받을 수 있음
    if((algo_num != 8 && algo_num > 3) || algo_num == 0){
        fprintf(stderr, "Usage : You should choice a range of number of algorithms between 1 and 3\n");
        exit(1);
    }

    printf("페이지 프레임의 개수를 입력하시오. (3 ~ 10) : ");
    scanf("%d", &frame_num);

    // 페이지 프레임 입력 범위 외 입력 시 에러 처리
    if(frame_num < 3 || frame_num > 10){ 
        fprintf(stderr, "Usage : Input a range of page frames between 3 and 10\n");
        exit(1);        
    }

    printf("데이터 입력 방식을 선택하시오. (1, 2) : ");
    scanf("%d", &input_type);

    // input_type을 입력받음
    if(input_type == 1){ // input_type이 1일 시
        fill_stream_rand(page_stream, rwbit);

    }else if(input_type == 2){ // input_type이 2일 시 페이지 스트림 파일 오픈
        if(argc < 2){
            fprintf(stderr, "Usage : You should input two arguments to start User input type\n");
            exit(1);
        }
        // fill_stream_rand(page_stream, rwbit); // 페이지 스트림을 랜덤하게 생성한 후
        // make_txt_file(page_stream, rwbit, fname); // main 인자로 받은 파일 명에 랜덤하게 생성된 데이터 저장
        fill_stream_user(page_stream, rwbit, fname); // main 인자로 받은 파일의 데이터를 page_stream, rwbit에 저장

    }else{ // input type 입력 범위 외 입력 시 에러 처리
        fprintf(stderr, "Usage : Input a range of input_type between 1 and 2\n");
        exit(1);
    }

    //system("clear");
    printf("페이지 교체 알고리즘 실행 및 분석을 시작하겠습니다. \n");
    printf("설정 사항\n");
    printf("선택하신 알고리즘 : ");
    for(int i=1; i<=8; i++) {
        if(algo[i])
            printf("%d ", i);
    }
    printf("\n페이지 프레임 수 : %d\n데이터 입력 방식 : %d\n", frame_num, input_type);

    // 페이지 스트림 출력
    printf("페이지 스트림 : ");
    for(int i=0; i<PAGE_NUM; i++){
        printf("%d/(r:%d, w:%d) ", page_stream[i], rwbit[i][0], rwbit[i][1]);
    }
    printf("\n\n");

    // 선택된 알고리즘 실행
    for(int i=1; i<=8; i++){
        if(algo[i] || algo_num == 8){
                if(i == OPTIMAL) printf("Optimal의 fault 수 : %d\n\n", fault[i] = optimal(frame_num, input_type, page_stream));
                else if(i == FIFO) printf("FIFO의 fault 수 : %d\n\n", fault[i] = fifo(frame_num, input_type, page_stream));
                else if(i == LIFO) printf("LIFO의 fault 수 : %d\n\n", fault[i] = lifo(frame_num, input_type, page_stream));
                else if(i == LRU) printf("LRU의 fault 수 : %d\n\n", fault[i] = lru(frame_num, input_type, page_stream));
                else if(i == LFU) printf("LFU의 fault 수 : %d\n\n", fault[i] = lfu(frame_num, input_type, page_stream));
                else if(i == SC) printf("SC의 fault 수 : %d\n\n", fault[i] = sc(frame_num, input_type, page_stream));
                else if(i == ESC) printf("ESC의 fault 수 : %d\n\n", fault[i] = esc(frame_num, input_type, page_stream, rwbit));
                
        }   
    }

    // page fault 수를 파일에 저장
    make_sim_file_fault(fault);
}