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
    delete w->data;
    delete w;
    w = nullptr;
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

template<typename T>
pNode(T) Splay<T>::splay(pNode(T) v)
{
    if (!v) return nullptr;
    pNode(T) p;
    pNode(T) g;
    while ((p = v->parent) && (g = p->parent))
    {
        pNode(T) gg = g->parent;
        if (is_lc(*v))
        {
            if (is_lc(*p))
            {
                attach_as_lc(g, p->rc);
                attach_as_lc(p, v->rc);
                attach_as_rc(p, g);
                attach_as_rc(v, p);
            }
            else
            {
                attach_as_lc(p, v->rc);
                attach_as_rc(g, v->lc);
                attach_as_lc(v, g);
                attach_as_rc(v, p);
            }
        }
        else if (is_rc(*p))
        {
            attach_as_rc(g, p->lc);
            attach_as_rc(p, v->lc);
            attach_as_lc(p, g);
            attach_as_lc(v, p);
        }
        else
        {
            attach_as_rc(p, v->lc);
            attach_as_lc(g, v->rc);
            attach_as_rc(p, g);
            attach_as_lc(v, p);
        }
        if (!gg)
            v->parent = nullptr;
        else
            (g == gg->lc) ? attach_as_lc(gg, v) : attach_as_rc(gg, v);
        update_height(g);
        update_height(p);
        update_height(v);
    }
    if (p = v->parent)
    {
        if (is_lc(*c))
        {
            attach_as_lc(p, v->rc);
            attach_as_rc(v, p);
        }
        else
        {
            attach_as_rc(p, v->lc);
            attach_as_lc(v, p);
        }
        update_height(p);
        update_height(v);
    }
    v->parent = nullptr;
    return v;
}

template<typename T>
pNode(T) & Splay<T>::search(const T & e)
{
    pNode(T) p = search_in(_root, e, _hot = nullptr);
    _root = splay(p ? p : _hot);
    return _root;
}

template<typename T>
pNode(T) Splay<T>::insert(const T & e)
{
    if (!_root)
    {
        _size += 1;
        return _root = new TreeNode<T>(e);
    }
    if (e == search(e)->data)
        return _root;
    _size += 1;
    pNode(T) t = _root;
    if (_root->data < e)
    {
        t->parent = _root = new TreeNode<T>(e, nullptr, t, t->rc);
        if (has_rc(*t))
        {
            t->rc->parent = _root;
            t->rc = nullptr;
        }
    }
    else
    {
        t->parent = _root = new TreeNode<T>(e, nullptr, t->lc = nullptr);
        if (has_lc(*t))
        {
            t->lc->parent = _root;
            t->lc = nullptr;
        }
    }
    update_height_above(t);
    return _root;
}

template<typename T>
bool Splay<T>::remove(const T & e)
{
    if (!_root || (e != search(e)->data)) return false;
    pNode(T) w = _root;
    if (!has_lc(*_root))
    {
        _root = _root->rc;
        if (_root)
            _root->parent = nullptr;
    }
    else if (!has_rc(*_root))
    {
        _root = _root->lc;
        if (_root)
            _root->parent = nullptr;
    }
    else
    {
        pNode(T) lTree = _root->lc;
        lTree->parent = nullptr;
        _root->lc = nullptr;
        _root = _root->rc;
        _root->parent = nullptr
        search(w->data);
        _root->lc = lTree;
        lTree->parent = _root;
    }
    delete w->data;
    delete w;
    w = nullptr;
    _size -= 1;
    if (_root)
        update_height(_root);
    return true;
}

template<typename T>
pBTNode(T) BTree<T>::search(const T & e)
{
    pBTNode(T) v = _root;
    _hot = nullptr;
    while (v)
    {
        Rank r = v->key.search(e);
        if ((0 <= r) && (e == v->key[r])) return v;
        _hot = v;
        v = v->child[r + 1];
    }
    return nullptr;
}

template<typename T>
bool BTree<T>::insert(const T & e)
{
    pBTNode(T) v = search(e);
    if (v) return false;
    Rank r = _hot->key.search(e);
    _hot->key.insert(r + 1, e);
    _hot->child.insert(r + 2, nullptr);
    _size += 1;
    solve_overflow(_hot);
    return true;
}

template<typename T>
void BTree<T>::solve_overflow(pBTNode(T) v)
{
    if (_order >= v->child.size()) return;
    Rank s = _order / 2;
    pBTNode(T) u = new BTNode<T>();
    for (Rank j = 0; j < _order; j++)
    {
        u->child.insert(j, v->child.remove(s + 1));
        u->key.insert(j, v->key.remove(s + 1));
    }
    u->child[_order - s - 1] = v->child.remove(s + 1);
    if (u->child[0])
        for (Rank j = 0; j < _order - s; j++)
            u->child[j]->parent = u;
    pBTNode(T) p = v->parent;
    if (!p)
    {
        _root = p = new BTNode<T>();
        p->child[0] = v;
        v->parent = p;
    }
    Rank r = 1 + p->key.search(v->key[0]);
    p->key.insert(r, v->key.remove(s));
    p->child.insert(r + 1, u);
    u->parent = p;
    solve_overflow(p);
}

template<typename T>
bool BTree<T>::remove(const T & e)
{
    pBTNode(T) v = search(e);
    if (!v) return false;
    Rank r = v.key->search(e);
    if (v->child[0])
    {
        u = v->child[r + 1];
        while (u->child[0])
            u = u->child[0];
        v->key[r] = u->key[0];
        v = u;
        r = 0;
    }
    v->key.remove(v);
    v->child.remove(r + 1);
    _size -= 1;
    return true;
}

template<typename T>
void BTree<T>::solve_underflow(pBTNode(T) v)
{
    if ((_order + 1) / 2 <= v->child.size()) return;
    pBTNode(T) p = v->parent;
    // root
    if (!p)
    {
        if (!v->key.size() && v->child[0])
        {
            _root = v->child[0];
            _root->parent = nullptr;
            v->child[0] = nullptr;
            delete v;
            v = nullptr;
        }
        return;
    }
    Rank r = 0;
    while (p->child[r] != v)
        r += 1;
    // case 1: borrow from left
    if (0 < r)
    {
        pBTNode(T) ls = p->child[r - 1];
        if ((_order + 1) / 2 < ls->child.size())
        {
            v->key.insert(0, p->key[r - 1]);
            p->key[r - 1] = ls->key.remove(ls->key.size() - 1);
            v->child.insert(0, ls->child.remove(ls->child.size() - 1));
            if (v->child[0]) v->child[0]->parent = v;
            return;
        }
    }
    // case 2: borrow from right
    if (p->child.size() - 1 > r)
    {
        pBTNode(T) rs = p->child[r + 1];
        if ((_order + 1) / 2 < rs->child.size())
        {
            v->key.insert(v->key.size(), p->key[r]);
            p->key[r] = rs->key.remove(0);
            v->child.insert(v->child.size(), rs->child.remove(0));

            if (v->child[v->child.size() - 1])
                v->child[v->child.size() - 1]->parent = v;
            return;
        }    
    }
    // case 3: merge left and right
    if (0 < r)
    {
        // merge with left
        pBTNode(T) ls -> p->child[r - 1];
        ls->key.insert(ls->key.size(), p->key.remove(r - 1));
        p->child.remove(r);
        ls->child.insert(ls->child.size(), v->child.remove(0));
        if (ls->child[ls->child.size() - 1])
            ls->child[ls->child.size() - 1]->parent = ls;
        while (!v->key.empty())
        {
            ls->key.insert(ls->key.size(), v->key.remove(0));
            ls->child.insert(ls->child.size(), v->child.remove(0));
            if (ls->child[ls->child.size() - 1])
                ls->child[ls->child.size() - 1]->parent = ls;
        }
    }
    else
    {
        // merge with right
        pBTNode(T) rs = p->child[r + 1];
        rs->key.insert(0, p->key.remove(r));
        p->child.remove(r);
        rs->child.insert(0, v->child.remove(v->child.size() - 1));
        if (rs->child[0])
            rs->child[0]->parent = rs;
        while (!v->key.empty())
        {
            rs->key.insert(0, v->key.remove(v->key.size() - 1));
            rs->child.insert(0, v->child.remove(v->child.size() - 1));
            if (rs->child[0])
                rs->child[0]->parent = rs;
        }
        delete v;
        v = nullptr;
    }
    solve_underflow(p);
    return;
}

template<typename T>
int RedBlackTree<T>::update_height(pBTNode(T) x)
{
    x->height = max(stature(x->lc), stature(x->rc));
    return is_black(x) ? x->height++ : x->height;
}

template<typename T>
pBTNode(T) RedBlackTree<T>::insert(const T & e)
{
    pNode(T) & x = search(e);
    if (x) return x;
    x = new TreeNode<T>(e, _hot, nullptr, nullptr, -1);
    _size += 1;
    solve_double_red(x);
    return x ? x : _hot->parent;
}

/*
 * RedBlack双红调整算法: 解决节点x与父节点均为红色的问题。分为两种情况:
 *      RR-1: 2次颜色翻转, 2次黑高度更新, 1~2次旋转, 不递归
 *      RR-2: 3次颜色翻转, 3次黑高度更新, 0次旋转, 需要递归
 */
template<typename T>
void RedBlackTree<T>::solve_double_red(pBTNode(T) x)
{
    if (is_root(*x))
    { 
        _root->color = RB_BLACK;
        _root->height += 1;
        return;
    }
    pBTNode(T) p = x->parent;
    if (is_black(p)) return;
    pBTNode(T) g = p->parent;
    pBTNode(T) u = uncle(x);
    if (is_black(u))
    {
        if (is_lc(*x) == is_lc(*p))
            p->color = RB_BLACK;
        else
            x->color = RB_BLACK;
        g->color = RB_RED;
        pBTNode(T) gg = g->parent;
        pBTNode(T) r = from_parent_to(*g) = rotate_at(x);
        r->parent = gg;
    }
    else
    {
        p->color = RB_BLACK;
        p->height += 1;
        u->color = RB_BLACK;
        u->height += 1;
        if (!is_root(*g))
            g->color = RB_RED;
        solve_double_red(g);
    }
}

template<typename T>
bool RedBlackTree<T>::remove(const T & e)
{
    pBTNode(T) & x = search(e);
    if (!x) return false;
    pBTNode(T) r = remove_at(x, _hot);
    if (!(--_size)) return true;
    // balance check
    if (!_hot)
    {
        _root->color = RB_BLACK;
        update_height(_root);
        return true;
    }
    // check root
    if (black_height_updated(*_hot)) return true;
    if (is_red(r))
    {
        r->color = RB_BLACK;
        r->height += 1;
        return true;
    }
    // check red-black order
    solve_double_black(r);
    return true;
}

/*
 * RedBlack双黑调整算法: 解决节点x与被其替代的节点均为黑色的问题
 * 分为3大类4种情况: 
 *      BB-1:  2次颜色翻转, 2次黑高度更新, 1~2次旋转, 不递归
 *      BB-2R: 2次颜色翻转, 2次黑高度更新, 0次旋转, 不递归
 *      BB-2B: 1次颜色翻转, 1次黑高度更新, 0次旋转, 需要递归
 *      BB-3:  2次颜色翻转, 2次黑高度更新, 1次旋转, 转为BB-1或BB-2R
 */
template<typename T>
void RedBlackTree<T>::solve_double_black(pBTNode(T) r)
{
    pBTNode(T) p = r ? r->parent : _hot;
    if (!p) return;
    pBTNode(T) s = (r == p->lc) ? p->rc : p->lc;
    if (is_black(s))
    {
        pBTNode(T) t = nullptr;
        if (is_red(s->rc)) t = s->rc;
        if (is_red(s->lc)) t = s->lc;
        if (t)
        {
            // case: BB-1
            RBColor old_color = p->color;
            pBTNode(T) b = from_parent_to(*p) = rotate_at(t);
            if (has_lc(*b))
            {
                b->lc->color = RB_BLACK;
                update_height(b->lc);
            }
            if (has_rc(*b))
            {
                b->rc->color = RB_BLACK;
                update_height(b->rc);
            }
            b->color = old_color;
            update_height(b);
        }
        else
        {
            // case: BB-2
            s->color = RB_RED;
            s->height -= 1;
            if (is_red(p))
            {
                // BB-2R
                p->color = RB_BLACK;
            }
            else
            {
                // BB-2B
                p->height -= 1;
                solve_double_black(p);
            }
        }
    }
    else
    {
        // case: BB-3
        s->color = RB_BLACK;
        p->color = RB_RED;
        pBTNode(T) t = is_lc(*s) ? s->lc : s->rc;
        _hot = p;
        from_parent_to(*p) = rotate_at(t);
        solve_double_black(r);
    }
}