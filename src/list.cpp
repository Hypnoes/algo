// list.cpp -- list class
#include "list.h"

template<typename T>
void List<T>::init()
{
    header = new ListNode<T>;
    trailer = new ListNode<T>;
    header->next = trailer;
    header->pred = nullptr;
    trailer->pred = header;
    trailer->next = nullptr;
    _size = 0;
}

template<typename T>
T & List<T>::operator[](Rank r) const
{
    pNode(T) p = first();
    while (0 < r--)
        p = p->next;
    return p->data;
}

template<typename T>
pNode(T) List<T>::find(T const & e, int n, pNode(T) p) const
{
    while (0 < n--)
        if (e == (p = p->pred)->data) return p;
    return nullptr;
}

template<typename T>
pNode(T) List<T>::insert_as_first(T const & e)
{
    _size += 1;
    return header->insert_as_succ(e);
}

template<typename T>
pNode(T) List<T>::insert_as_last(T const & e)
{
    _size += 1;
    return trailer->insert_as_pred(e);
}

template<typename T>
pNode(T) List<T>::insert_A(pNode(T) p, T const & e)
{
    _size += 1;
    return p->insert_as_succ(e);
}

template<typename T>
pNode(T) List<T>::insert_B(pNode(T) p, T const & e)
{
    _size += 1;
    return p->insert_as_pred(e);
}

template<typename T>
pNode(T) ListNode<T>::insert_as_pred(T const & e)
{
    pNode(T) x = new ListNode(e, pred, this);
    pred->next = x;
    pred = x;
    return x;
}

template<typename T>
pNode(T) ListNode<T>::insert_as_succ(T const & e)
{
    pNode(T) x = new ListNode(e, this, next);
    next->pred = x;
    next = x;
    return x;
}

template<typename T>
T List<T>::remove(pNode(T) p)
{
    T e = p->data;
    p->pred->next = p->next;
    p->next->pred = p->pred;
    delete p;
    p = nullptr;
    _size -= 1;
    return e;
}