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
    bool remove(const T & e)
};

template<typename NodePosi>
inline void attach_as_lc(NodePosi p, NodePosi lc)
{ p->lc = lc; if (lc) lc->parent = p; }

template<typename NodePosi>
inline void attach_as_rc(NodePosi p, NodePosi rc)
{ p->rc = rc; if (rc) rc->parent = p; }

#define pBTNode(T) BTNode<T> *

template<typename T>
struct BTNode
{
    pBTNode(T) parent;
    Vector<T> key;
    Vector<pBTNode(T)> child;

    BTNode()
    {
        parent = nullptr;
        child.insert(0, nullptr);
    }

    BTNode(T e, pBTNode(T) lc = nullptr, pBTNode(T) rc = nullptr)
    {
        parent = nullptr;
        key.insert(0, e);
        child.insert(0, lc);
        child.insert(0, rc);
        if (lc) lc->parent = this;
        if (rc) rc->parent = this;
    }
};

template<typename T>
class BTree
{
  protected:
    int _size;
    int _order;
    pBTNode(T) _root;
    pBTNode(T) _hot;
    void solve_overflow(pBTNode(T));
    void solve_underflow(pBTNode(T));

  public:
    BTree(int order = 3) : _order(order), _size(0) { _root = new BTNode<T>(); }
    ~BTree() { if (_root) delete _root; _root = nullptr; }
    int const order() { return _order; }
    int const size() { return _size; }
    pBTNode(T) & root() { return _root; }
    bool empty() const { return !_root; }
    pBTNode(T) search(const T & e);
    bool insert(const T & e);
    bool remove(const T & e);
};

template<typename T>
class RedBlackTree : public BinSearchTree<T>
{
  protected:
    void solve_double_red(pBTNode(T) x);
    void solve_double_black(pBTNode(T) x);
    int update_height(pBTNode(T) x);

  public:
    pBTNode(T) insert(const T & e);
    bool remove(const T & e);
};

#define is_black(p) (!(p) || (RB_BLACK == (p)->color))
#define is_red(p) (!is_black(p))
#define black_height_updated(x) (\
    (stature((x).lc) == stature((x).rc)) &&\
    ((x).height == (is_red(&x) ? stature((x).lc) : stature((x).lc) + 1)))
#endif