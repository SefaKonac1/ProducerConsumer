#include <pthread.h>
#include <signal.h>
#include <cstdio>
#include <unistd.h>
#include <chrono>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


#include "MyQueue.hpp"
#include "MyQueue.cpp"


#define MODE 6666
#define BUF_SIZE 512
#define SECOND_TO_MICROSECOND 1000000
#define FLAGS O_CREAT | O_WRONLY | O_TRUNC

#define PROD_LOG "producer.log"
#define CONS_LOG "consumer.log"

typedef struct Data{
    Data():number(0),timeout(0){}
    uint_least32_t number;
    uint_least32_t timeout;
}Data;

typedef enum{

    PRODUCER = 0,
    CONSUMER = 1

}ThreadType;

MyQueue<Data> myq;

struct timeval tv;
struct timespec ts;

int_least32_t sumOfNum = 0;
volatile unsigned int stop = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock,flock = PTHREAD_MUTEX_INITIALIZER;

std::chrono::steady_clock::time_point systemInit,systemTerminated,timePoint;
