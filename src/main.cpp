
#include "includes.hpp"


void signalHandler(int signum){
    fprintf(stderr,"CTRL-C Signal Received.\n");
    stop = 1;
}


void* consumer(void* args);
void* producer(void* args);
static void checkError(int ret);
static void writeFile(int type, int fd, uint_least32_t data);

int main(int argc, char **argv){
    
    int ret = 0;
    pthread_t cons,prod;

    fprintf(stderr,"Please press CTRL-C for terminating process ! \n");

    int fdp = open(PROD_LOG,FLAGS,MODE);
    int fdc = open(CONS_LOG,FLAGS,MODE);

    signal(SIGINT,signalHandler);   

    systemInit = std::chrono::steady_clock::now();

    ret = pthread_create(&prod, NULL,producer, &fdp);
    checkError(ret);
    ret = pthread_create(&cons,NULL,consumer,&fdc);
    checkError(ret);

    ret = pthread_join(prod,NULL);
    checkError(ret);
    ret = pthread_join(cons,NULL);
    checkError(ret);


    systemTerminated = std::chrono::steady_clock::now();
    fprintf(stderr,"Program was running %ld seconds...\n",
        std::chrono::duration_cast<std::chrono::seconds>(systemTerminated - systemInit).count());
    
    fprintf(stderr,"Main Thread is dying...\n");
    close(fdp);
    close(fdc);
    return 0;
}

void* producer(void* args){

    int fd = *(int*)args;
    
    pthread_mutex_lock(&lock);
    while(!stop){

        //int ret = pthread_cond_wait(&cond,&lock);

        /*wake producer thread.*/ 
        /*produce random awake time between range 100-300micro seconds */  
        time_t timeout = (100 + rand() % 200 ) ;

        //if(ret == ETIMEDOUT){}
        
        Data sendData;
        int num = rand()%100;
        sendData.number = num;
        sendData.timeout = timeout;

        myq.push(sendData);
        writeFile(0,fd,num);
        
       
        pthread_mutex_unlock(&lock);
        usleep(timeout);
    }
    fprintf(stderr,"Procuder Thread is dying...\n");
    pthread_exit(NULL);
}

void* consumer(void* args){

    int fd = *(int*)args;
    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    
    pthread_mutex_lock(&lock);
    while(!stop){
        
        if(!myq.isEmpty()){
            Data receivedData = myq.pop();
            sumOfNum += receivedData.number;    

            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            /*measure 1 seconds for reset total summarization.*/
            if(std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() >= 1){
            
                sumOfNum = 0;
               
                begin = std::chrono::steady_clock::now();
            }
            writeFile(1,fd,sumOfNum);
        }
    
        
        //pthread_cond_signal(&cond);    
        pthread_mutex_unlock(&lock);

    }
    fprintf(stderr,"Consumer Thread is dying...\n");
    pthread_exit(NULL);
}

static void checkError(int ret){

    switch (ret)
    {
        case EINVAL:
            fprintf(stderr,"The thread corresponding to the given thread ID is a detached thread.\n");
            exit(EINVAL);
        
        case EAGAIN:
            fprintf(stderr,"The thread corresponding to the)necessary resources are not available.\n");
            exit(EAGAIN);

        case ENOMEM:
            fprintf(stderr,"There is not enough memory to initialize the thread.\n");
            exit(ENOMEM);

        case EDEADLK:
            fprintf(stderr,"A deadlock would exist.\n");
            exit(ENOMEM);

        case ESRCH:
            fprintf(stderr,"No thread could be found corresponding to the given thread ID.\n");
            exit(ENOMEM);

        default:
            break;
    }

}

static void writeFile(int type, int fd, uint_least32_t data) {

    pthread_mutex_lock(&flock);
    int64_t sumOfTime;
    std::string msg,unitOfTime,time;
                 
    switch (type)
    {
        case ThreadType::PRODUCER:
            
            timePoint = std::chrono::steady_clock::now();
            sumOfTime = std::chrono::duration_cast<std::chrono::microseconds>(timePoint - systemInit).count();
            
            if(1000 <= sumOfTime && sumOfTime < 1000000){
                unitOfTime.assign("milisaniye'de ");
                time.assign(std::to_string(sumOfTime*0.001)+". ");
            }
            else if(sumOfTime >= 1000000){
                unitOfTime.assign("saniye'de ");
                time.assign(std::to_string(sumOfTime*0.000001)+". ");
            }
            else{
                unitOfTime.assign("mikrosaniye'de ");
                time.assign(std::to_string(sumOfTime)+". ");
            }
            
            msg.append(time);
            msg.append(unitOfTime);
            msg.append(std::to_string(data));
            msg.append(" gönderdi.\n");
            //fprintf(stderr,"%s\n",msg.c_str());
            write(fd,msg.c_str(),msg.size());
            break;
            
        case ThreadType::CONSUMER:

            msg.append("Toplam: ");
            msg.append(std::to_string(data) + "\n");
           // fprintf(stderr,"%s\n",msg.c_str()); 
           
            write(fd,msg.c_str(),msg.size());
            break;

        default:
            break;
    }

    pthread_mutex_unlock(&flock);
}

