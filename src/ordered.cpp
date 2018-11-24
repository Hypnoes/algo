// oredered.cpp -- ordered queue
#include "ordered.h"

template<typename T>
PQ_ComplHeap<T>::PQ_ComplHeap(T * A, Rank n)
{
    copy_from(A, 0, n);
    heapify(n);
}

template<typename T>
T PQ_ComplHeap<T>::get_max()
{
    return _elem[0];
}

template<typename T>
void PQ_ComplHeap<T>::insert(T e)
{
    Vector<T>::insert(e);
    percolate_up(_size - 1);
}

template<typename T>
Rank PQ_ComplHeap<T>::percolate_up(Rank i)
{
    while (parent_vaild(i))
    {
        Rank j = parent(i);
        if (lt(_elem[i], _elem[j]))
            break;
        swap(_elem[i], _elem[j]);
        i = j;
    }
    return i;
}

template<typename T>
T PQ_ComplHeap<T>::del_max()
{
    T max_elem = _elem[0];
    _elem[0] = _elem[--_size];
    percolate_down(_size, 0);
    return max_elem;
}

template<typename T>
Rank PQ_ComplHeap<T>::percolate_down(Rank n, Rank i)
{
    Rank j;
    while (i != (j = proper_parent(_elem, n, i)))
    {
        swap(_elem[i], _elem[j]);
        i = j;
    }
    return i;
}

template<typename T>
void PQ_ComplHeap<T>::heapify(Rank n)
{
    for (int i = last_internal(n); in_heap(n, i); i--)
        percolate_down(n, i);
}

template<typename T>
static pNode(T) merge(pNode(T) a, pNode(T) b)
{
    if (!a) return b;
    if (!b) return a;
    if (lt(a->data, b->data)) swap(a, b);
    a->rc = merge(a->rc, b);
    a->rc->parent = a;
    if (!a->rc || a->lc>npl < a->rc>npl)
        swap(a->lc, a->rc);
    a->npl = a->rc ? a->rc->npl + 1 : 1;
    return a;
}

template<typename T>
T PQ_LeftHeap<T>::del_max()
{
    pNode(T) l_heap = _root->lc;
    pNode(T) r_heap = _root->rc;
    T e = _root->data;
    delete _root;
    _size -= 1;
    _root = merge(l_heap, r_heap);
    if (_root) _root->parent = nullptr;
    return e;
}

template<typename T>
void PQ_LeftHeap<T>::insert(T e)
{
    pNode(T) v = new TreeNode<T>(e);
    _root = merge(_root, v);
    _root->parent = nullptr;
    _size += 1;
}