#ifndef MYQUEUE_HPP
#define MYQUEUE_HPP



#include <iostream>
#include <pthread.h>
/*Queue node representation*/

template <typename T>
class Node{
    public:
        Node* next;
        T data;
};

template <typename T>
class MyQueue{
    public:
        MyQueue();
        
        T pop();
        void push(T& data);
        virtual ~MyQueue();

        uint_least32_t sumAll();
        inline int getSize(){return qsize;}
        inline bool isEmpty() const{return (head == NULL)? 1 : 0;}
    
    private:
    
        int qsize;

        Node<T>* head;
        Node<T>* tail;
        Node<T>* prev;
        Node<T>* temp;

        pthread_mutex_t mutex;

        
    
};

#endif