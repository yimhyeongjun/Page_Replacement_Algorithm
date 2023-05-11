#include "userlib.h"

// 최적 알고리즘
// 가장 나중에 접근될 페이지를 교체하는 알고리즘 
// 앞으로 들어올 페이지 스트림을 확인하여 가장 나중에 발견되는 것을 교체
int optimal(int frame_num, int input_type, int* page_stream){ 

    int fault = 0;  // 페이지 폴트 발생 횟수 저장하는 변수 
    int *page_frame = (int *)malloc(sizeof(int) * frame_num);   // 페이지 프레임을 저장하는 배열
    int *checked = (int *)malloc(sizeof(int) * SEED);   // 가장 나중에 사용되는 페이지를 찾을 때 사용하는 checked 배열
    char *fname = "Optimal_simulation.txt"; // 시뮬레이션 결과를 저장할 텍스트 파일명
    int fd; 

    // optimal algorithm 수행 시뮬레이션 결과를 저장할 텍스트 파일 오픈
    if((fd = open(fname, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0){
        fprintf(stderr, "open error for %s\n", fname);
        exit(1);
    }

    printf("Optimal Algorithm Start!\n\n");

    // 페이지 스트림의 페이지를 순차적으로 확인
    for(int i=0; i<PAGE_NUM; i++){
        
        int input_page = page_stream[i]; // 현재 들어온 페이지
        int flag =  FAULT; // 페이지 폴트 여부 저장하는 플래그 변수

        // 현재 들어온 페이지가 페이지 프레임에 존재하는 지 확인
        for(int j=0; j<frame_num; j++){

            // hit인 경우 플래그 변경
            if(page_frame[j] == input_page){ 
                flag = HIT;
                break;
            }
        }

        // hit인 경우
        if(flag == HIT){
            // 페이지 프레임 상태 출력 및 파일에 저장
            print_page_frame_arr(i+1, page_frame, frame_num, HIT, input_page);
            make_sim_file_arr(fd, i+1, page_frame, frame_num, HIT, input_page);
        }else if(flag == FAULT){ // page fault인 경우

            int ff = 0;

            
            for(int j=0; j<frame_num; j++){
                // 페이지 프레임이 비어있는 경우
                if(page_frame[j] == 0){
                    page_frame[j] = input_page;
                    ff = 1;
                    break;
                }
            }

            // ff == 0 이면 페이지 교체
            if(ff == 0){

                // 교체할 페이지 찾기 : 가장 나중에 접근될 페이지를 교체
                int count = 0, k = i+1;
                memset(checked, 0, SEED * sizeof(int));

                // page_stream의 i+1 번째 인덱스부터 탐색하면서 page_frame과 일치하는 페이지가 존재한다면 checked 배열에 체크
                for(int k=i+1; k<PAGE_NUM; k++){
                    for(int l=0; l<frame_num; l++){
                        if(page_stream[k] == page_frame[l] && !checked[page_frame[l]]){
                            // 체크한 숫자의 개수를 1 증가시킴
                            count++;
                            checked[page_stream[k]] = 1;
                        }
                    }

                    // 만약 체크한 숫자의 개수가 프레임 개수보다 하나 작을 경우 -> 페이지 스트림에서 페이지 프레임에 저장된 페이지 중 가장 나중에 들어올 페이지를 제외하고 다 찾았을 때
                    if(count == frame_num - 1){
                        break;
                    } 
                }
                
                // check되지 않은 인덱스의 페이지 교체
                for(int m=0; m<frame_num; m++){
                    if(!checked[page_frame[m]]){
                        page_frame[m] = input_page;
                        break;
                    }
                }

            }

            // 페이지 폴트 1 증가
            fault++;

            // 페이지 프레임 상태 출력 및 파일에 저장
            print_page_frame_arr(i+1, page_frame, frame_num, FAULT, input_page);
            make_sim_file_arr(fd, i+1, page_frame, frame_num, FAULT, input_page);
        }

        
        
    }
    
    close(fd);
    free(page_frame);
    free(checked);
    // page fault 수 리턴
    return fault;
}