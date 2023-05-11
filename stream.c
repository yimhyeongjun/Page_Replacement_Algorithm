#include "userlib.h"

// page_stream과 rwbit 스트림을 사용자 입력으로 받는 함수
void fill_stream_user(int *page_stream, int (*rwbit)[2], char *fname){
    
    // a.out 컴파일 시 .txt 파일을 컴파일 매개변수로 받아 파일을 오픈하여 page_stream과 rwbit 배열에 채움

    printf("%s in fill_stream_user\n", fname);

    
    FILE* fp;
    char buf[BUF_SIZE];

    // 사용자 입력 파일 open
    if((fp = fopen(fname, "r")) == NULL){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }

    printf("input file %s\n", fname);
    int cnt=0, idx=0;

    // 사용자 파일 읽어 page_stream, rwbit 배열에 저장
    while(fgets(buf, sizeof(buf), fp) != NULL){
        char *result;
        // '/t' 단위로 tokenize하여 읽기 
        result = strtok(buf, "\t");
        while(result != NULL){

            // modular 연산을 통해 page_stream, R bit, W bit 분리
            if(cnt % 3 == 0){
                page_stream[idx] = atoi(result);
            }else if(cnt % 3 == 1){
                rwbit[idx][0] = atoi(result);
            }else{
                rwbit[idx++][1] = atoi(result);
            }
            cnt++;

            result = strtok(NULL, "\t");
        }
    
    }

    // fname close
    fclose(fp);
}

// 
// void make_txt_file(int* page_stream, int (*rwbit)[2], char *fname){
//     int fd;
//     char buf[40];
//     if((fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
//         fprintf(stderr, "open error for %s\n", fname);
//         exit(1);
//     }
//     for(int i=0;i<PAGE_NUM;i++){
//         sprintf(buf, "%d\t%d\t%d\n",page_stream[i], rwbit[i][0], rwbit[i][1]);
//         if(write(fd, buf, strlen(buf)) != strlen(buf)){
//             fprintf(stderr, "write error\n");
//             exit(1);
//         }
//     }
// }

// page_stream과 rwbit 스트림을 랜덤하게 생성하는 함수
void fill_stream_rand(int * page_stream, int (*rwbit)[2]){

        // 생성한 페이지 스트림의 개수
        int counter = 0;

        // rand 함수의 seed 값을 time 함수를 통해 난수로 설정한다.
        srand((unsigned int)time(NULL));

        //  페이지 스트림 랜덤하게 생성
        while(counter < PAGE_NUM){
            int temp=0;
            // 랜덤하게 뽑은 페이지가 0인 경우 제외하고 페이지 스트림 채우기
            if((temp = rand() % SEED) != 0){
                page_stream[counter++] = temp;
            }
        }   

        counter = 0;
        // R, W bit 스트림 랜덤하게 생성
        while(counter < PAGE_NUM){
            int r=0;
            int w=0;
            r = rand() % 2;
            w = rand() % 2;

            // 페이지 스트림에 들어왔는데 참조되지 않았다는 것은 모순
            // 초기 R, W bit값이 (0, 0)인 경우는 존재할 수 없으므로 건너 뜀
            if(r == 0 && w == 0) continue;
            // 초기 R, W bit값이 (0, 1)인 경우는 존재할 수 없으므로 건너 뜀
            if(r == 0 && w == 1) continue;

            // R, W bit 스트림 채우기
            rwbit[counter][0] = r;
            rwbit[counter][1] = w;
            counter++;
        }
}

// 배열 형태의 페이지 프레임 상태 출력 함수
void print_page_frame_arr(int stream_num, int *page_frame, int frame_num, int fault, int input_page){

    // 페이지 스트림 번호와 값 출력
    printf("page_stream #%d, page_string : %d\n", stream_num, input_page);
    
    // 페이지 fault인 경우 플래그를 사용하여 page fault임을 출력
    if(fault == FAULT){
        printf("page fault!\n");
    }

    // 페이지 프레임 출력
    for(int j=0; j<frame_num; j++){
        printf("page frame #%d : %d\n", j+1, page_frame[j]);
    }
    printf("\n");
}

// queue 형태의 페이지 프레임 상태 출력 함수
void print_page_frame_queue(int stream_num, queue *page_frame, int frame_num, int fault, int input_page){
    
    // 페이지 프레임에 저장된 데이터 개수를 카운트하는 변수 -> 프레임 개수보다 적을 때 나머지 프레임 출력부를 0으로 채우기 위함
    int cnt=0;
    
    // 페이지 스트림 번호와 값 출력
    printf("page_stream #%d, page_string : %d\n", stream_num, input_page);

    // Linked List 형태로 구현된 페이지 프레임을 탐색하기 위한 커서
    node* cur = page_frame->front;

    // 페이지 fault인 경우 플래그를 사용하여 page_fault임을 출력
    if(fault == FAULT){
        printf("page fault!\n");
    }

    // 페이지 프레임 출력하며 페이지 프레임 내 데이터 개수 카운트
    while(cur->next != NULL){
        printf("page frame #%d : %d\n", cnt+1 ,cur->data);
        cnt++;
        cur = cur->next;
    }
    printf("page frame #%d : %d\n", cnt+1, cur->data);
    cnt++;

    // 페이지 프레임 내 데이터 개수가 프레임의 개수보다 작다면 빈 페이지 프레임에 값이 없다는 것을 0으로 출력하여 표현
    while(cnt < frame_num){
        printf("page frame #%d : 0\n", cnt+1);
        cnt++;
    }
    printf("\n");
}


// stack 형태의 페이지 프레임 상태 출력 함수
void print_page_frame_stack(int stream_num, stack *page_frame, int frame_num, int fault, int input_page){

    // 페이지 프레임에 저장된 데이터 개수를 카운트하는 변수 -> 프레임 개수보다 적을 때 나머지 프레임 출력부를 0으로 채우기 위함
    int cnt=0;

    // 페이지 스트림 번호와 값 출력
    printf("page_stream #%d, page_string : %d\n", stream_num, input_page);

    // Linked List 형태로 구현된 페이지 프레임을 탐색하기 위한 커서
    node* cur = page_frame->top;

    // 페이지 fault인 경우 플래그를 사용하여 page_fault임을 출력
    if(fault == FAULT){
        printf("page fault!\n");
    }

    // 페이지 프레임 출력하며 페이지 프레임 내 데이터 개수 카운트
    while(cur->next != NULL){
        printf("page frame #%d : %d\n", cnt+1 ,cur->data);
        cnt++;
        cur = cur->next;
    }
    printf("page frame #%d : %d\n", cnt+1, cur->data);
    cnt++;

    // 페이지 프레임 내 데이터 개수가 프레임의 개수보다 작다면 빈 페이지 프레임에 값이 없다는 것을 0으로 출력하여 표현
    while(cnt < frame_num){
         printf("page frame #%d : 0\n", cnt+1);
        cnt++;
    }
    printf("\n");
}

// LFU 알고리즘의 페이지 프레임 상태 출력 함수 -> 페이지 프레임이 queue 형태이지만 참조 횟수를 출력하기 위해 따로 구현
void print_page_frame_lfu(int stream_num, queue *page_frame, int* refer, int frame_num, int fault, int input_page){

    // 페이지 프레임에 저장된 데이터 개수를 카운트하는 변수 -> 프레임 개수보다 적을 때 나머지 프레임 출력부를 0으로 채우기 위함
    int cnt=0;

    // 페이지 스트림 번호와 값 출력
    printf("page_stream #%d, page_string : %d\n", stream_num, input_page);

    // Linked List 형태로 구현된 페이지 프레임을 탐색하기 위한 커서
    node* cur = page_frame->front;

    // 페이지 fault인 경우 플래그를 사용하여 page_fault임을 출력
    if(fault == FAULT){
        printf("page fault!\n");
    }
    
    // 페이지 프레임과 해당 페이지의 참조 횟수를 출력하며 페이지 프레임 내 데이터 개수 카운트
    while(cur->next != NULL){
        printf("page frame #%d : %d / refer : %d\n", cnt+1 ,cur->data, refer[cur->data]);
        cnt++;
        cur = cur->next;
    }
    printf("page frame #%d : %d / refer : %d\n", cnt+1 ,cur->data, refer[cur->data]);
    cnt++;

     // 페이지 프레임 내 데이터 개수가 프레임의 개수보다 작다면 빈 페이지 프레임에 값이 없다는 것을 0으로 출력하여 표현
    while(cnt < frame_num){
         printf("page frame #%d : 0\n", cnt+1);
        cnt++;
    }
    printf("\n");
}


// sc 알고리즘의 페이지 프레임 상태 출력 함수 -> 페이지 프레임이 배열 형태이지만 참조 여부를 출력하기 위해 따로 구현하였음
void print_page_frame_sc(int stream_num, int *page_frame, int *clock, int frame_num, int fault, int cursor, int input_page){

    // 페이지 스트림 번호와 값 출력
    printf("page_stream #%d, page_string : %d\n", stream_num, input_page);
    
    // 페이지 fault인 경우 플래그를 사용하여 page_fault임을 출력
    if(fault ==FAULT){
        printf("page fault!\n");
    }

    // 페이지 프레임 출력하며 페이지 프레임 내 데이터 개수 카운트
    for(int j=0; j<frame_num; j++){
        // 만약 커서의 위치가 j와 같다면 화살표 모양을 출력
        if(j == cursor)
            printf("page frame #%d : %d / R : %d  <-\n", j+1, page_frame[j], clock[j]);
        else
            printf("page frame #%d : %d / R : %d\n", j+1, page_frame[j], clock[j]);
    }
    printf("\n");
}

// esc 알고리즘의 페이지 프레임 상태 출력 함수 -> 페이지 프레임이 배열 형태이지만 R, W bit를 출력하기 위해 따로 구현하였음
void print_page_frame_esc(int stream_num, int *page_frame, int **clock, int frame_num, int fault, int cursor, int input_page){

    // 페이지 스트림 번호와 값 출력
    printf("page_stream #%d, page_string : %d\n", stream_num, input_page);
    
    // 페이지 fault인 경우 플래그를 사용하여 page_fault임을 출력
    if(fault == FAULT){
        printf("page fault!\n");
    }

    // 페이지 프레임과 R, W 비트를 출력하며 페이지 프레임 내 데이터 개수 카운트
    for(int j=0; j<frame_num; j++){
        // 만약 커서의 위치가 j와 같다면 화살표 모양을 출력
        if(j == cursor)
            printf("page frame #%d : %d / R : %d, W : %d  <-\n", j+1, page_frame[j], clock[j][0], clock[j][1]);
        else
            printf("page frame #%d : %d / R : %d, W : %d\n", j+1, page_frame[j], clock[j][0], clock[j][1]);
    }
    printf("\n");
}




// 배열 형태의 페이지 프레임을 갖는 알고리즘 시뮬레이션 결과를 텍스트 파일에 저장하는 함수
void make_sim_file_arr(int desfd, int stream_num, int* page_frame, int frame_num, int fault, int input_page){
    char buf[40];

    // 파일에 페이지 스트림 번호와 값을 입력
    sprintf(buf, "page_stream #%d, page_string : %d\n", stream_num, input_page);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    // 페이지 fault인 경우 플래그를 사용하여 파일에 page_fault임을 입력
    if(fault == 1){
        char *fs = "page fault!\n";
        if(write(desfd, fs, strlen(fs)) != strlen(fs)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    // 페이지 프레임 내용을 파일에 입력
    for(int i=0; i<frame_num; i++){
        sprintf(buf, "page_frame #%d : %d\n", i+1, page_frame[i]);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    if(write(desfd, "\n", strlen("\n")) != strlen("\n")){
        fprintf(stderr, "write error\n");
        exit(1);
    }

}

// queue 형태의 페이지 프레임을 갖는 알고리즘 시뮬레이션 결과를 텍스트 파일에 저장하는 함수
void make_sim_file_queue(int desfd, int stream_num, queue* page_frame, int frame_num, int fault, int input_page){
    
    char buf[40];

    // 페이지 프레임에 저장된 데이터 개수를 카운트하는 변수 -> 프레임 개수보다 적을 때 나머지 프레임 출력부를 0으로 채우기 위함
    int cnt=0;

    // 파일에 페이지 스트림 번호와 값을 입력
    sprintf(buf, "page_stream #%d, page_string : %d\n", stream_num, input_page);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    // 페이지 fault인 경우 플래그를 사용하여 파일에 page_fault임을 입력
    if(fault == FAULT){
        char *fs = "page fault!\n";
        if(write(desfd, fs, strlen(fs)) != strlen(fs)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    // 페이지 프레임 내용을 파일에 입력하며 페이지 프레임 내 데이터 개수 카운트
    node* cur = page_frame->front;
    while(cur->next != NULL){
        sprintf(buf, "page frame #%d : %d\n", cnt+1, cur->data);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
        cnt++;
        cur = cur->next;
    }
    sprintf(buf, "page_frame #%d : %d\n", cnt+1, cur->data);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }
    cnt++;
    
    // 페이지 프레임 내 데이터 개수가 프레임의 개수보다 작다면 빈 페이지 프레임에 값이 없다는 것을 파일에 0으로 저장
    while(cnt < frame_num){
        sprintf(buf, "page_frame #%d : 0\n", cnt+1);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
        cnt++;
    }

    if(write(desfd, "\n", strlen("\n")) != strlen("\n")){
        fprintf(stderr, "write error\n");
        exit(1);
    }

}

// stack 형태의 페이지 프레임을 갖는 알고리즘 시뮬레이션 결과를 텍스트 파일에 저장하는 함수
void make_sim_file_stack(int desfd, int stream_num, stack* page_frame, int frame_num, int fault, int input_page){
    
    char buf[40];
    
    // 페이지 프레임에 저장된 데이터 개수를 카운트하는 변수 -> 프레임 개수보다 적을 때 나머지 프레임 출력부를 0으로 채우기 위함
    int cnt=0;

    // 파일에 페이지 스트림 번호와 값을 입력
    sprintf(buf, "page_stream #%d, page_string : %d\n", stream_num, input_page);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    // 페이지 fault인 경우 플래그를 사용하여 파일에 page_fault임을 입력
    if(fault == 1){
        char *fs = "page fault!\n";
        if(write(desfd, fs, strlen(fs)) != strlen(fs)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    // 페이지 프레임 내용을 파일에 입력하며 페이지 프레임 내 데이터 개수 카운트
    node* cur = page_frame->top;
    while(cur->next != NULL){
        sprintf(buf, "page_frame #%d : %d\n", cnt+1, cur->data);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
        cnt++;
        cur = cur->next;
    }
    sprintf(buf, "page_frame #%d : %d\n", cnt+1, cur->data);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }
    cnt++;

    // 페이지 프레임 내 데이터 개수가 프레임의 개수보다 작다면 빈 페이지 프레임에 값이 없다는 것을 파일에 0으로 저장
    while(cnt < frame_num){
        sprintf(buf, "page_frame #%d : 0\n", cnt+1);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
        cnt++;
    }

    if(write(desfd, "\n", strlen("\n")) != strlen("\n")){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    
}



// lfu 알고리즘의 시뮬레이션 결과를 텍스트 파일에 저장하는 함수
void make_sim_file_lfu(int desfd, int stream_num, queue* page_frame, int* refer, int frame_num, int fault, int input_page){
    
    char buf[40];
    
    // 페이지 프레임에 저장된 데이터 개수를 카운트하는 변수 -> 프레임 개수보다 적을 때 나머지 프레임 출력부를 0으로 채우기 위함
    int cnt=0;

    // 파일에 페이지 스트림 번호와 값을 입력
    sprintf(buf, "page_stream #%d, page_string : %d\n", stream_num, input_page);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    // 페이지 fault인 경우 플래그를 사용하여 파일에 page_fault임을 입력
    if(fault == FAULT){
        char *fs = "page fault!\n";
        if(write(desfd, fs, strlen(fs)) != strlen(fs)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    // 페이지 프레임 내용을 파일에 입력하며 페이지 프레임 내 데이터 개수 카운트
    node* cur = page_frame->front;
    while(cur->next != NULL){
        sprintf(buf, "page frame #%d : %d / refer : %d\n", cnt+1, cur->data, refer[cur->data]);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
        cnt++;
        cur = cur->next;
    }
    sprintf(buf, "page frame #%d : %d / refer : %d\n", cnt+1, cur->data, refer[cur->data]);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }
    cnt++;

    // 페이지 프레임 내 데이터 개수가 프레임의 개수보다 작다면 빈 페이지 프레임에 값이 없다는 것을 파일에 0으로 저장
    while(cnt < frame_num){
        sprintf(buf, "page frame #%d : 0\n", cnt+1);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
        cnt++;
    }

    if(write(desfd, "\n", strlen("\n")) != strlen("\n")){
        fprintf(stderr, "write error\n");
        exit(1);
    }

}

// sc 알고리즘의 시뮬레이션 파일에 저장하는 함수
void make_sim_file_sc(int desfd, int stream_num, int* page_frame, int *clock, int frame_num, int fault, int cursor, int input_page){
    char buf[40];

    // 파일에 페이지 스트림 번호와 값을 입력
    sprintf(buf, "page_stream #%d, page_string : %d\n", stream_num, input_page);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    // 페이지 fault인 경우 플래그를 사용하여 파일에 page_fault임을 입력
    if(fault == FAULT){
        char *fs = "page fault!\n";
        if(write(desfd, fs, strlen(fs)) != strlen(fs)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    // 페이지 프레임과 참조 여부, 커서 위치를 파일에 저장
    for(int i=0; i<frame_num; i++){

        // 만약 커서의 위치가 i와 같다면 화살표 모양으로 표현
        if(cursor == i)
            sprintf(buf, "page_frame #%d : %d / R : %d  <-\n", i+1, page_frame[i], clock[i]);
        else
            sprintf(buf, "page_frame #%d : %d / R : %d\n", i+1, page_frame[i], clock[i]);
        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }
    if(write(desfd, "\n", strlen("\n")) != strlen("\n")){
        fprintf(stderr, "write error\n");
        exit(1);
    }
} 

// esc 알고리즘의 시뮬레이션 파일에 저장하는 함수
void make_sim_file_esc(int desfd, int stream_num, int* page_frame, int **clock, int frame_num, int fault, int cursor, int input_page){
    char buf[40];

    // 파일에 페이지 스트림 번호와 값을 입력
    sprintf(buf, "page_stream #%d, page_string %d\n", stream_num, input_page);
    if(write(desfd, buf, strlen(buf)) != strlen(buf)){
        fprintf(stderr, "write error\n");
        exit(1);
    }

    // 페이지 fault인 경우 플래그를 사용하여 파일에 page_fault임을 입력
    if(fault == FAULT){
        char *fs = "page fault!\n";
        if(write(desfd, fs, strlen(fs)) != strlen(fs)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    // 페이지 프레임과 R, W 비트, 커서 위치를 파일에 저장
    for(int i=0; i<frame_num; i++){

        // 만약 커서의 위치가 i와 같다면 화살표 모양으로 표현
        if(cursor == i)
            sprintf(buf, "page frame #%d : %d / R : %d, W : %d  <-\n", i+1, page_frame[i], clock[i][0], clock[i][1]);
        else
            sprintf(buf, "page frame #%d : %d / R : %d, W : %d\n", i+1, page_frame[i], clock[i][0], clock[i][1]);

        if(write(desfd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }
    if(write(desfd, "\n", strlen("\n")) != strlen("\n")){
        fprintf(stderr, "write error\n");
        exit(1);
    }
}

// 시뮬레이션 결과를 fault.txt 파일에 저장하는 함수
void make_sim_file_fault(int* fault){
    char* fname = "fault.txt";
    int fd;
    char buf[40];
    if((fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0644)) < 0){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }

    for(int i=1; i<8; i++){
        if(i == OPTIMAL) sprintf(buf, "Optimal의 fault 수 : %d\n\n", fault[i]);
        else if(i == FIFO) sprintf(buf, "FIFO의 fault 수 : %d\n\n", fault[i]);
        else if(i == LIFO) sprintf(buf, "LIFO의 fault 수 : %d\n\n", fault[i]);
        else if(i == LRU) sprintf(buf, "LRU의 fault 수 : %d\n\n", fault[i]);
        else if(i == LFU) sprintf(buf, "LFU의 fault 수 : %d\n\n", fault[i]);
        else if(i == SC) sprintf(buf, "SC의 fault 수 : %d\n\n", fault[i]);
        else if(i == ESC) sprintf(buf, "ESC의 fault 수 : %d\n\n", fault[i]);
        if(write(fd, buf, strlen(buf)) != strlen(buf)){
            fprintf(stderr, "write error\n");
            exit(1);
        }
    }

    close(fd);
}