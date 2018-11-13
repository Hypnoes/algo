// search.cpp -- bin-search-tree and avl and red-black-tree
#include "search.h"

template<typename T>
static pNode(T) & search_in(pNode(T) & v, const T & e, pNode(T) & hot)
{
    if (!v || (e == v->data)) return v;
    hot = v;
    return search_in(((e< v->data) ? v->lc : v->rc), e, hot);
}

template<typename T>
pNode(T) & BinSearchTree<T>::search(const T & e)
{
    return search_in(_root, e, _hot = NULL);
}

template<typename T>
pNode(T) BinSearchTree<T>::insert(const T & e)
{
    pNode(T) & x = search(e);
    if (x) return x;
    x = new TreeNode<T>(e, _hot);
    _size += 1;
    update_height_above(x);
    return x;
}

template<typename T>
bool BinSearchTree<T>::remove(const T & e)
{
    pNode(T) & x = search(e);
    if (!x) return false;
    remove_at(x, _hot);
    _size -= 1;
    update_height_above(_hot);
    return true;
}

/*
 * BST节点删除算法: 删除位置x所指向的节点, 全局静态模板函数, 适用于AVL, Splay, RedBlack
 * 等各种BST。目标x在此前经查找定位, 并确定非NULL, 故必定删除成功; 与search_in不同, 
 * 调用之前不必将hot置空。返回值指向实际被删除节点的接替者, hot指向实际被删除节点的父节点
 * 且两者有可能是NULL。
 */
template<typename T>
static pNode(T) remove_at(pNode(T) & x, pNode(T) & hot)
{
    pNode(T) w = x;
    pNode(T) succ = nullptr;
    if (!has_lc(*x))
        succ = x = x->rc;
    else if (!has_rc(*x))
        succ = x = x->lc;
    else
    {
        w = w->succ();
        swap(x->data, w->data);
        pNode(T) u = w->parent;
        ((u == x) ? u->rc : u->lc) = succ = w->rc;
    }
    hot = w->parent;
    if (succ) succ->parent = hot;
    release(w->data);
    release(w);
    return succ;
}

template<typename T>
pNode(T) AVL<T>::insert(const T & e)
{
    pNode(T) & x = search(e);
    if (x) return x;
    pNode(T) xx = x = new TreeNode<T>(e, _hot);
    _size += 1;
    for (pNode(T) g = _hot; g; g = g->parent)
    {
        if (!AvlBalanced(*g))
        {
            from_parent_to(*g) = rotate_at(TallerChild(TallerChild(g)));
            break;
        }
        else
            update_height(g);
    }
    return xx;
}

template<typename T>
bool AVL<T>::remove(const T & e)
{
    pNode(T) & x = search(e);
    if (!x) return false;
    remove_at(x, _hot);
    _size -= 1;
    for (pNode(T) g = _hot; g; g = g->parent)
    {
        if (!AvlBalanced(*g))
            g = from_parent_to(*g) = rotate_at(TallerChild(TallerChild(g)));
        update_height(g);
    }
    return true;
}

/* 
 * 按照 "3 + 4" 结构连接3个节点及其4棵子树, 发挥重组之后的局部子树根节点位置(即b)
 * 子树根节点与上层节点之间的双向连接, 均须由上层调用者完成。可用于AVL与RedBlack
 * 的局部平衡调整
 */
template<typename T>
pNode(T) BinSearchTree<T>::connect34(
        pNode(T) a, pNode(T) b , pNode(T) c,
        pNode(T) T0, pNode(T) T1, pNode(T) T2, pNode(T) T3)
{
    a->lc = T0; if (T0) T0->parent = a;
    a->rc = T1; if (T1) T1->parent = a; update_height(a);
    c->lc = T2; if (T2) T2->parent = c;
    c->rc = T3; if (T3) T3->parent = c; update_height(c);
    b->lc = a; a->parent = b;
    b->rc = c; c->parent = b; update_height(b);
    return b; 
}

/* 
 * BST节点旋转变换同意算法(3节点 + 4子树), 返回调整之后局部子树根节点的位置。
 * 注意: 尽管子树根会正确指向上层节点(如果存在), 但反向的联接需由上层函数完成
 */
template<typename T>
pNode(T) BinSearchTree<T>::rotate_at(pNode(T) v)
{
    pNode(T) p = v->parent;
    pNode(T) g = p->parent;
    if (is_lc(*p))
    {
        if (is_lc(*v))
        {
            p->parent = g->parent;
            return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
        }
        else 
        {
            v->parent = g->parent;
            return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
        }
    }
    else
    {
        if (is_rc(*v))
        {
            p->parent = g->parent;
            return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
        }
        else
        {
            v->parent = g->parent;
            return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
        }
    }
}