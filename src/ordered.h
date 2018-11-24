// ordered.h -- ordered queue
#ifndef ORDERED_H_
#define ORDERED_H_
#include "vector.h"
#include "bintree.h"

template<typename T>
struct PQ
{
    virtual void insert(T) = 0;
    virtual T get_max() = 0;
    virtual T del_max() = 0;
};

// ordered heap
#define in_heap(n, i) (((-1) < (i)) && ((i) < (n)))
#define parent(i) ((i - 1) >> 1)
#define last_internal(n) parent(n - 1)
#define l_child(i) (1 + ((i) << 1))
#define r_child(i) ((1 + (i)) << 1)
#define parent_vaild(i) (0 < i)
#define l_child_vaild(n, i) in_heap(n, l_child(i))
#define r_child_valid(n, i) in_heap(n, r_child(i))
#define bigger(PQ, i, j) (lt(PQ[i], PQ[j]) ? j : i)
#define proper_parent(PQ, n, i) \
        (r_child_valid(n, i) ? bigger(PQ, bigger(PQ, i, l_child(i)), r_child(i))\
            : (l_child_vaild(n, i) ? bigger(PQ, i, l_child(i)) : i))

template<typename T>
class PQ_ComplHeap : public PQ<T>, public Vector<T>
{
  protected:
    Rank percolate_down(Rank n, Rank i);
    Rank percolate_up(Rank i);
    void heapify(Rank n);

  public:
    PQ_ComplHeap() {}
    PQ_ComplHeap(T * A, Rank n);
    void insert(T);
    T get_max();
    T del_max();
};

template<typename T>
class PQ_LeftHeap : public PQ<T>, public BinTree<T>
{
  public:
    PQ_LeftHeap() {}
    PQ_LeftHeap(T * E, int n)
    { for (int i = 0; i < n; i++) insert(E[i]); }

    void insert(T);
    T get_max();
    T del_max();
};
#endif