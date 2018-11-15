// list.h -- list class
#ifndef LIST_H_
#define LIST_H_
#include <cstddef>

#define pNode(T) ListNode<T>*

typedef int Rank;

template<typename T>
struct ListNode
{
    T data;
    pNode(T) pred;
    pNode(T) next;

    ListNode() {}
    ListNode(T e, pNode(T) p = nullptr, pNode(T) s = nullptr)
        : data(e), pred(p), next(s) {}
    
    pNode(T) insert_as_pred(T const &);
    pNode(T) insert_as_succ(T const &);
};

template<typename T>
class List
{
  private:
    int _size;
    pNode(T) header;
    pNode(T) trailer;

  protected:
    void init();
    void clear();
    void copy_nodes(pNode(T), int);
    void merge(pNode(T) &, int, List<T> &, pNode(T), int);
    void merge_sort(pNode(T) &, int);
    void select_sort(pNode(T), int);
    void insert_sort(pNode(T), int);

  public:
    List() { init(); }
    List(List<T> const & L);
    List(List<T> const & L, Rank r, int n);
    List(pNode(T) p, int n);
    ~List();

    Rank size() const { return _size; }
    bool empty() const { return _size <= 0; }
    T & operator[](Rank r) const;
    pNode(T) first() const { return header->next; }
    pNode(T) last() const { return trailer->pred; }
    bool valid(pNode(T) p)
    { return p && (trailer != p) && (header != p); }
    int disordered() const;
    pNode(T) find(T const & e) const
    { return find(e, _size, trailer); }
    pNode(T) find(T const & e, int n, pNode(T) p) const;
    pNode(T) search(T const & e) const
    { return search(e, _size, trailer); }
    pNode(T) search(T const & e, int n, pNode(T) p) const;
    pNode(T) select_max(pNode(T) p, int n);
    pNode(T) select_max() { return select_max(header->next, _size); }

    pNode(T) insert_as_first(T const & e);
    pNode(T) insert_as_last(T const & e);
    pNode(T) insert_A(pNode(T) p, T const & e);
    pNode(T) insert_B(pNode(T) p, T const & e);
    T remove(pNode(T) p);
    void merge(List<T> & L) { merge(first(), size, L, L.first(), L._size); }
    void sort(pNode(T) p, int n);
    void sort() { sort(first(), _size); }
    int deduplicate();
    int uniquify();
    void reverse();

    void traverse(void (*)(T&));
    template<typename VST> void traverse(VST &);
};

#endif