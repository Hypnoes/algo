// stackqueeu.cpp -- stack and queue
#include "stackqueue.h"

template<typename T>
void Stack<T>::push(T const & e)
{
    insert(size(), e);
}

template<typename T>
T Stack<T>::pop()
{
    return remove(size() - 1);
}

template<typename T>
T & Stack<T>::pop()
{
    return (*this)[size() - 1];
}

template<typename T>
void Queue<T>::enqueue(T const & e)
{
    insert_as_last(e);
}

template<typename T>
T Queue<T>::dequeue()
{
    return remove(first());
}

template<typename T>
T &Queue<T>::front()
{
    return first()->data;
}