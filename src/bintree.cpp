// bintree.cpp -- methods of binary tree
#include "bintree.h"

template<typename T>
pNode(T) TreeNode<T>::insert_as_lc(T const & e)
{
    return lc = new Node(e, this);
}

template<typename T>
pNode(T) TreeNode<T>::insert_as_rc(T const & e)
{
    return rc = new Node(e, this);
}

template<typename T>
template<typename VST>
void TreeNode<T>::trav_in(VST & visit)
{
    switch(rand() % 5)
    {
        case 1: travIN_i1(this, visit); break;
        case 2: travIN_i2(this, visit); break;
        case 3: travIN_i3(this, visit); break;
        case 4: travIN_i4(this, visit); break;
        default: travIn_R(this, visit); break;
    }
}

template<typename T>
int BinTree<T>::update_height(pNode(T) x)
{
    return x->height = 1 + max(stature(x->lc), stature(x->rc));
}

template<typename T>
void BinTree<T>::update_height_above(pNode(T) x)
{
    while (x)
    {
        update_height(x);
        x = x->parent;
    }
}

template<typename T>
pNode(T) BinTree<T>::insert_as_root(T const & e)
{
    _size = 1;
    return _root = new TreeNode<T>(e);
}

template<typename T>
pNode(T) BinTree<T>::insert_as_lc(pNode(T) x, T const & e)
{
    _size += 1;
    x->insert_as_lc(e);
    update_height_above(x);
    return x->lc;
}

template<typename T>
pNode(T) BinTree<T>::insert_as_rc(pNode(T) x, T const & e)
{
    _size += 1;
    x->insert_as_lc(e);
    update_height_above(x);
    return x->rc;
}

template<typename T>
pNode(T) BinTree<T>::insert_as_lc(pNode(T) x, BinTree<T> * & S)
{
    if (x->lc = S->_root)
        x->lc->parent = x;
    _size += S->_size;
    update_height_above(x);
    S->_root = nullptr;
    S->_size = 0;
    delete S;
    S = nullptr;
    return x;
}

template<typename T>
int BinTree<T>::remove(pNode(T) x)
{
    from_parent_to(*x) = nullptr;
    update_height_above(x->parent);
    int n = remove_at(x);
    _size -= n;
    return n;
}

template<typename T>
static int remove_at(pNode(T) x)
{
    if (!x) return 0;
    int n = 1 + remove_at(x->lc) + remove_at(x->rc);
    delete x;
    x = nullptr;
    return n;
}

template<typename T>
BinTree<T> * BinTree<T>::secede(pNode(T) x)
{
    from_parent_to(*x) = nullptr;
    update_height_above(x->parent);
    BinTree<T> * S = new BinTree<T>;
    S->_root = x;
    x->parent = nullptr;
    S->_size = x->size();
    _size -= S->_size;
    return S;
}

template<typename T, typename VST>
void travPre_R(pNode(T) x, VST & visit)
{
    if (!x) return;
    visit(x->data);
    travPre_R(x->lc, visit);
    travPre_R(x->rc, visit);
}

template<typename T, typename VST>
void travPost_R(pNode(T) x, VST & visit)
{
    if (!x) return;
    travPost_R(x->lc, visit)
    travPost_R(x->rc, visit);
    visit(x->data);
}

template<typename T, typename VST>
void travIn_R(pNode(T) x, VST & visit)
{
    if (!x) return;
    travIn_R(x->lc, visit);
    visit(x->data);
    travIn_R(x->rc, visit);
}

template<typename T, typename VST>
static void visit_along_left_branch(pNode(T) x, VST & visit, Stack<pNode(T)> & S)
{
    while (x)
    {
        visit(x->data);
        S.push(x->rc);
        x = x->lc;
    }
}

template<typename T, typename VST>
void travPre_i2(pNode(T) x, VST & visit)
{
    Stack<pNode(T)> S;
    while (true)
    {
        visit_along_left_branch(x, visit, S);
        if (S.empty()) break;
        x = S.pop();
    }
}

template<typename T>
static void go_along_left_branch(pNode(T) x, Stack<pNode(T)> & S)
{
    while (x)
    {
        S.push(x);
        x = x->lc;
    }
}

template<typename T, typename VST>
void travIn_i1(pNode(T) x, VST & visit)
{
    Stack<pNode(T)> S;
    while (true)
    {
        go_along_left_branch(x, S);
        if (S.empty()) break;
        x = S.pop();
        visit(x->data);
        x = x->rc;
    }
}

template<typename T>
pNode(T) TreeNode<T>::succ()
{
    pNode(T) s = this;
    if (rc)
    {
        s = rc;
        while (has_lc(*s))
            s = s->lc;
    }
    else
    {
        while (is_rc(*s))
            s = s->parent;
        s = s->parent;
    }
    return s;
}

template<typename T, typename VST>
void travIN_i2(pNode(T) x, VST & visit)
{
    Stack<pNode(T)> S;
    while (true)
    {
        if (x)
        {
            S.push(x);
            x = x->lc;
        }
        else if (!S.empty())
        {
            x = S.pop();
            visit(x->data);
            x = x->rc;
        }
        else
            break;
    }
}

template<typename T, typename VST>
void travIn_i3(pNode(T) x, VST & visit)
{
    bool backtrack = false;
    while (true)
    {
        if (!backtrack && has_lc(*x))
            x = x->lc;
        else
        {
            visit(x->data);
            if (has_rc(*x))
            {
                x = x->rc;
                backtrack = false;
            }
            else
            {
                if (!(x = x->succ())) break;
                backtrack = true;
            }
        }
        
    }
}

template<typename T>
static void go_to_highest_leaf_visible_from_left(Stack<pNode(T)> & S)
{
    while (pNode(T) x = S.top())
    {
        if (has_lc(*x))
        {
            if (has_rc(*x))
                S.push(x->rc);
            S.push(x->lc);
        }
        else
            S.push(x->rc);
    }
    S.pop();
}

template<typename T, typename VST>
void travPost_I(pNode(T) x, VST & visit)
{
    Stack<pNode(T)> S;
    if (x) S.push(x);
    while (!S.empty())
    {
        if (S.top() != x->parent)
            go_to_highest_leaf_visible_from_left(S);
        x = S.pop();
        visit(x->data);
    }
}

template<typename T>
template<typename VST>
void TreeNode<T>::trav_level(VST & visit)
{
    Queue<pNode(T)> Q;
    Q.enqueue(this);
    while (!Q.empty())
    {
        pNode(T) x = Q.dequeue();
        visit(x->data);
        if (has_lc(*x)) Q.enqueue(x->lc);
        if (has_rc(*x)) Q.enqueue(x->rc);
    }
}