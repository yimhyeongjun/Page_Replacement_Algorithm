// algorithm c file
int optimal(int frame_num, int input_type, int* page_stream); // optimal 알고리즘
int fifo(int frame_num, int input_type, int* page_stream); // FIFO 알고리즘
int lifo(int frame_num, int input_type, int* page_stream); // LIFO 알고리즘
int lru(int frame_num, int input_type, int* page_stream); // LRU 알고리즘
int lfu(int frame_num, int input_type, int* page_stream); // LFU 알고리즘
int sc(int frame_num, int input_type, int* page_stream); // SC 알고리즘
int esc(int frame_num, int input_type, int* page_stream, int (*rwbit)[2]); // ESC 알고리즘

// stream.c
void fill_stream_rand(int *page_stream, int (*rwbit)[2]); // 난수로 페이지를 생성하여 페이지 프레임을 채우는 함수 
void fill_stream_user(int *page_stream, int (*rwbit)[2], char *fname); // 사용자가 제공하는 텍스트 파일에 저장된 문자열 형태의 페이지를 읽어 페이지 스트림을 채우는 함수 

void print_page_frame_arr(int stream_num, int *page_frame, int frame_num, int fault, int input_page); // arr 형태의 페이지 프레임을 출력하는 함수
void print_page_frame_queue(int stream_num, queue *page_frame, int frame_num, int fault, int input_page); // queue 형태의 페이지 프레임을 출력하는 함수
void print_page_frame_stack(int stream_num, stack *page_frame, int frame_num, int fault, int input_page); // stack 형태의 페이지 프레임을 출력하는 함수
void print_page_frame_lfu(int stream_num, queue *page_frame, int *refer, int frame_num, int fault, int input_page); // lfu 알고리즘의 페이지 프레임을 출력하는 함수
void print_page_frame_sc(int stream_num, int *page_frame, int *clock, int frame_num, int fault, int cursor, int input_page); // sc 알고리즘의 페이지 프레임을 출력하는 함수
void print_page_frame_esc(int stream_num, int *page_frame, int **clock, int frame_num, int fault, int cursor, int input_page); // esc 알고리즘의 페이지 프레임을 출력하는 함수

void make_sim_file_arr(int desfd, int stream_num, int* page_frame, int frame_num, int fault, int input_page); // arr 형태의 페이지 프레임을 파일에 저장하는 함수
void make_sim_file_queue(int desfd, int stream_num, queue* page_frame, int frame_num, int fault, int input_page); // queue 형태의 페이지 프레임을 파일에 저장하는 함수
void make_sim_file_stack(int desfd, int stream_num, stack* page_frame, int frame_num, int fault, int input_page); // stack 형태의 페이지 프레임을 파일에 저장하는 함수
void make_sim_file_lfu(int desfd, int stream_num, queue *page_frame, int *refer, int frame_num, int fault, int input_page); // lfu 형태의 페이지 프레임을 파일에 저장하는 함수
void make_sim_file_sc(int desfd, int stream_num, int* page_frame, int *clock, int frame_num, int fault, int cursor, int input_page); // sc 형태의 페이지 프레임을 파일에 저장하는 함수
void make_sim_file_esc(int desfd, int stream_num, int* page_frame, int **clock, int frame_num, int fault, int cursor, int input_page); // esc 형태의 페이지 프레임을 파일에 저장하는 함수

void make_sim_file_fault(int* fault); // 시뮬레이션 결과를 fault.txt 파일에 저장하는 함수