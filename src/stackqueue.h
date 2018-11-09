// stackqueue.h -- stack and queue
#ifndef STACKQUEUE_H_
#define STACKQUEUE_H_

#include "vector.h"
#include "list.h"

template<typename T>
class Stack : public Vector<T>
{
  public:
    void push(T const &);
    T pop();
    T & pop();
};

template<typename T>
class Queue : public List<T>
{
  public:
    void enqueue(T const &);
    T dequeue();
    T & front();
};
#endif