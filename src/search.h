// search.h -- bin-search-tree and avl and red-black-tree
#ifndef SEARCH_H_
#define SEARCH_H_
#include "bintree.h"

template<typename K, typename V>
struct Entry
{
    K key;
    V value;

    Entry(K k = K(), V v = V()) : key(k), value(v) {}
    Entry(Entry<K, V> const & e) : key(e.key), value(e.value) {}

    bool operator<(Entry<K, V> const & e) { return key < e.key; }
    bool operator>(Entry<K, V> const & e) { return key > e.key; }
    bool operator==(Entry<K, V> const & e) { return key == e.key; }
    bool operator!=(Entry<K, V> const & e) { return key !== e.key; }
};

template<typename T>
class BinSearchTree : public BinTree<T>
{
  protected:
    pNode(T) _hot;
    pNode(T) connect34(
        pNode(T), pNode(T), pNode(T),
        pNode(T), pNode(T), pNode(T), pNode(T)
    );
    pNode(T) rotate_at(pNode(T) x);

  public:
    virtual pNode(T) & search (const T & e);
    virtual pNode(T) insert(const T & e);
    virtual bool remove(const T & e);
};

template<typename T>
static pNode(T) & search_in(pNode(T) &, const T &, pNode(T) &);

template<typename T>
class AVL : public BinSearchTree<T>
{
  public:
    pNode(T) insert(const T & e);
    bool remove(const T & e);
};

#define Balanced(x) (stature((x).lc) == stature((x).rc))
#define BalFac(x) (stature((x).lc) - stature((x).rc))
#define AvlBalanced(x) ((-2 < BalFac(x)) && (BalFac(x) < 2))

#define TallerChild(x) (\
    stature((x)->lc) > stature((x)->rc) ? (x)->lc : (\
    stature((x)->lc) < stature((x)->rc) ? (x)->rc : (\
    is_lc(*(x)) ? (x)->lc : (x)->rc)))

template<typename T>
class Splay : public BinSearchTree<T>
{
  protected:
    pNode(T) splay(pNode(T) v);

  public:
    pNode(T) & search(const T & e);
    pNode(T) insert(const T & e);
    bool remove(const T & e(;))
};

template<typename NodePosi>
inline void attach_as_lc(NodePosi p, NodePosi lc)
{ p->lc = lc; if (lc) lc->parent = p; }

template<typename NodePosi>
inline void attach_as_rc(NodePosi p, NodePosi rc)
{ p->rc = rc; if (rc) rc->parent = p; }

#endif