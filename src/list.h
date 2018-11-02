// list.h -- define of list class
#ifndef LIST_H_
#define LIST_H_

template<typename T>
struct Node
{
    T d;
    Node * next {nullptr};
};

template<typename T>
class List
{
  private:
    typedef void (*CALLBACK)(T &);
    Node<T> * _elem;

  public:
    List(): _elem(nullptr) {}
    ~List() {}

    void clear();
    bool is_empty() const;
    int length() const;
    T & get_elem(int) const;
    int find_elem(T &) const;
    T & prior(T &) const;
    T & next(T &) const;
    void insert(T &, int);
    void remove(int);
    void foreach(const CALLBACK) const;

    void union_with(const List &);
    void merge_with(const List &);

    T & operator[](int);
};

#endif