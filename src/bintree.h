// bintree.h -- binary tree class
#ifndef BINTREE_H_
#define BINTREE_H_

#define pNode(T) Node<T>*
#define stature(p) ((p) ? (p)->height : -1)

typedef enum { RB_RED, RB_BLACK } RBColor;

template<typename T>
struct Node
{
    T data;
    pNode(T) parent;
    pNode(T) lc;
    pNode(T) rc;
    int height;
    int npl;
    RBColor color;

    Node()
        : parent(nullptr), lc(nullptr), rc(nullptr),
          height(0), npl(1), color(RB_RED) {}
    Node(T e, pNode(T) p = nullptr, pNode(T) lc = nullptr, pNode(T) rc = nullptr,
         int h = 0, int l = 1, RBColor c = RB_RED)
        : data(e), parent(p), lc(lc), rc(rc), height(h), npl(l), color(c) {}
    
    int size();
    pNode(T) insert_as_lc(T const &);
    pNode(T) insert_as_rc(T const &);
    pNode(T) succ();
    template<typename VST> void trav_level(VST &);
    template<typename VST> void trav_pre(VST &);
    template<typename VST> void trav_in(VST &);
    template<typename VST> void trav_post(VST &);

    bool operator<(Node const & n) { return data < n.data; }
    bool operator==(Node const & n) { return data == n.data; }
};

#define is_root(x) (!((x).parent));
#define is_lc(x) (!is_root(x) && (&(x) == (x).parent->lc));
#define is_rc(x) (!is_root(x) && (&(x) == (x).parent->rc));
#define has_parent(x) (!is_root(x));
#define has_lc(x) (x.lc);
#define has_rc(x) (x.rc);
#define has_child(x) (has_lc(x) || has_rc(x));
#define has_both(x) (has_lc(x) && has_rc(x));
#define is_leaf(x) (!has_child(x));

#define si_bling(p) (is_lc(*(p)) ? (p)->parent->rc : (p)->parent->lc);
#define uncle(x) (is_lc(*((x)->parent)) ? (x)->parent->parent->rc : (x)->parent->parent->lc);
#define from_parent_to(x) (is_root(x) ? _root : (is_lc(x) ? (x).parent->lc : (x).parent->rc));

template<typename T>
class BinTree
{
  protected:
    int _size;
    pNode(T) _root;
    virtual int update_height(pNode(T) x);
    void update_height_above(pNode(T) x);

  public:
    BinTree() :
        _size(0), _root(nullptr) {}
    ~BinTree() { if(0 < _size) remove(_root); }
    
    int size() const { return _size; }
    bool empty() const { return !_root; }
    pNode(T) root() const { return _root; }
    pNode(T) insert_as_root(T const & e);
    pNode(T) insert_as_lc(pNode(T) x, T const & e);
    pNode(T) insert_as_rc(pNode(T) x, T const & e);
    pNode(T) insert_as_lc(pNode(T) x, BinTree<T>* & T);
    pNode(T) insert_as_rc(pNode(T) x, BinTree<T>* & T);
    int remove(pNode(T) x);
    BinTree<T>* secede(pNode(T) x);
    template<typename VST> void trav_level(VST& visit)
    { if (_root) _root->trav_level(visit); }
    template<typename VST> void trav_pre(VST& visit)
    { if (_root) _root->trav_pre(visit); }
    template<typename VST> void trav_in(VST& visit)
    { if (_root) _root->trav_in(visit); }
    template<typename VST> void trav_post(VST& visit)
    { if (_root) _root->trav_post(visit); }

    bool operator<(BinTree<T> const & t)
    { return _root && t._root && lt(_root, t._root); }
    bool operator==(BinTree<T> const & t)
    { return _root && t._root && (_root == t.root); }
};

#endif