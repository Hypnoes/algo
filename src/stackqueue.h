// stackqueue.h -- stack and queue
#ifndef STACKQUEUE_H_
#define STACKQUEUE_H_

#include "vector.h"
#include "list.h"

template<typename T>
class Stack : public Vector<T>
{
  public:
    void push(T const & e) { insert(size(), e); }
    T pop() { return remove(size() - 1); }
    T& pop() { return (*this)[size() - 1]; ]}
};

template<typename T>
class Queue : public List<T>
{
  public:
    void enqueue(T const & e) { insert_as_last(e); }
    T dequeue() { return remove(first()); }
    T& front() { return first()->data; }
};
#endif