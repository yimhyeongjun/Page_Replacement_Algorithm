#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>


#include "queue.h"
#include "stack.h"
#include "useralgo.h"

#define PAGE_NUM 500
#define SEED 31
#define BUF_SIZE 1024

#define OPTIMAL 1
#define FIFO 2
#define LIFO 3
#define LRU 4
#define LFU 5
#define SC 6
#define ESC 7

#define FAULT 1
#define HIT 0