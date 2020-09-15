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
    uint_least64_t number;
    uint_least64_t timeout;
}Data;

typedef enum{

    PRODUCER = 0,
    CONSUMER = 1

}ThreadType;

MyQueue<Data> myq;

struct timeval tv;
struct timespec ts;

volatile unsigned int stop = 0;

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock,flock = PTHREAD_MUTEX_INITIALIZER;

const std::chrono::seconds TIME_LIMIT_SEC(2);
const uint64_t TIME_LIMIT_USEC = std::chrono::duration_cast<std::chrono::microseconds>(TIME_LIMIT_SEC).count();

std::chrono::steady_clock::time_point systemInit,systemTerminated,timePoint;
