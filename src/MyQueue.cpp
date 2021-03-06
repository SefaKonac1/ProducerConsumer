
#include "MyQueue.hpp"

template <typename T>
MyQueue<T>::MyQueue(){ 
    qsize = 0;
    this->head = NULL;
    this->tail = NULL;
}

template <typename T>
void MyQueue<T>::push(T& data){ 

    pthread_mutex_lock(&mutex);

    this->temp = new Node<T>();
    /*insert data and next pointer into temp node pointer.*/
    this->temp->data = data;
    this->temp->next = NULL;

    if (isEmpty()){
        this->head = this->temp;
        this->tail = this->temp;
        qsize++;
    }else{
        this->prev = this->tail;
        this->tail->next = this->temp;
        this->tail = this->temp;
        qsize++;
    }
    pthread_mutex_unlock(&mutex);
}

template <typename T>
T MyQueue<T>::pop(){

    pthread_mutex_lock(&mutex);

    this->temp = this->head;
    T d = this->temp->data;
    this->head = this->head->next;
    delete this->temp;
    /*avoiding dangling pointer*/
    this->temp = NULL;
    qsize--;

    pthread_mutex_unlock(&mutex);

    return d;
}

template <typename T>
MyQueue<T>::~MyQueue(){ 
    /*Destroy all queue when program is terminated.*/
    while(this->head != NULL){

        Node<T>* tmp = this->head->next;
        delete[] this->head;
        this->head = tmp;
    }   
}

template <typename T>
uint_least32_t MyQueue<T>::sumAll()
{
    pthread_mutex_lock(&mutex);
    uint_least32_t sum = 0;
    if(!isEmpty())
    {
        for(this->temp = this->head; this->temp != NULL; this->temp=this->temp->next){
            sum += this->temp->data.number;   
        }
       
    }
    pthread_mutex_unlock(&mutex);
    return sum;
}

template<typename T>
T MyQueue<T>::operator[](int i){

    pthread_mutex_lock(&mutex);
    this->temp = this->head;

    for (int j = 0; j < i && this->temp != NULL; j++){
        this->temp = this->temp->next;
    }
    pthread_mutex_unlock(&mutex);

    return this->temp->data;
}
