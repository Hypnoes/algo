// dict.cpp -- dictionary and hashmap
#include <memory>
#include <cstring>
#include "dict.h"

template<typename T>
QuadList<T>::~QuadList()
{
    clear();
    delete header;
    delete trailer;
}

template<typename T>
bool QuadList<T>::vaild(pQLNode(T) p)
{
    return p && (trailer != p) && (header != 0);
}

template<typename T>
void QuadList<T>::init()
{
    header = new QuadListNode<T>;
    trailer = new QuadListNode<T>;
    header->next = trailer;
    header->pred = nullptr;
    trailer->pred = header;
    trailer->next = nullptr;
    header->above = trailer->above = nullptr;
    header->below = trailer->below = nullptr;
    _size = 0;
}

template<typename K, typename V>
V * SkipList<K, V>::get(K k)
{
    if (empty()) return nullptr;
    ListNode<QuadList<Entry<K, V>>*>* q_list = first();
    QuadListNode<Entry<K, V>> * p = q_list->data->first();
    return skip_search(q_list, p, k) ? & (p->entry.value) : nullptr;
}

/*
 * skip_search()词条查找算法(内部方法)
 * 入口: q_list为顶层列表, p为q_list的首节点
 * 出口: 若成功, p为命中关键码所属塔的顶部节点, q_list为p所属列表
 *       否则, p为所属塔的基座, 该塔对应于不大于k的最大且最靠右关键码, q_list为空
 * 约定: 多个词条命中时, 取沿四联表最靠后者
 */
template<typename K, typename V>
bool SkipList<K, V>::skip_search(
    ListNode<QuadList<Entry<K, V>>*>* & q_list,
    QuadListNode<Entry<K, V>>* & p,
    K & k)
{
    while (true)
    {
        while (p->next && (p->entry.key) <= k)
            p = p->next;
        p = p->pred;
        if (p->pred && (k == p->entry.key)) return true;
        q_list = q_list->next;
        if (!q_list->next) return false;
        p = (p->pred) ? p->below : q_list->data->first();
    }
}

template<typename K, typename V>
bool SkipList<K, V>::put(K k, V v)
{
    Entry<K, V> e = Entry<K, V>(k, v);
    if (empty()) insert_as_first(new QuadList<Entry<K, V>>);
    ListNode<QuadList<Entry<K, V>>*>* q_list = first();
    QuadListNode<Entry<K, V>> * p = q_list->data->first();
    if (skip_search(q_list, p, k))
        while (p->below) p = p->below;
    q_list = last();
    QuadListNode<Entry<K, V>> * b = q_list->data->insert_after_above(e, p);
    while (rand() & 1)
    {
        while (q_list->data->vaild(p) && !p->above)
            p = p->pred;
        if (!q_list->data->vaild(p))
        {
            if (q_list == first())
                insert_as_first(new QuadList<Entry<K, V>>);
            p = q_list->pred->data->first()->pred;
        }
        else
            p = p->above;
        q_list = q_list->pred;
        b = q_list->data->insert_after_above(e, p, b);
    }
    return true;
}

template<typename T>
pQLNode(T) QuadList<T>::insert_after_above(T const & e, pQLNode(T) p,
    pQLNode(T) b = nullptr)
{
    _size++;
    return p->insert_as_next_above(e, b);
}

template<typename T>
pQLNode(T) QuadListNode<T>::insert_as_next_above(T const & e,
    pQLNode(T) b = nullptr)
{
    pQLNode(T) x = new QuadListNode<T>(e, this, next, nullptr, b);
    next->pred = x;
    next = x;
    if (b) b->above = x;
    return x;
}

template<typename K, typename V>
bool SkipList<K, V>::remove(K k)
{
    if (empty()) return false;
    auto q_list = first()
    auto p = q_list->data->first();
    if (!skip_search(q_list, p, k)) return false;
    do
    {
        QuadListNode<Entry<K, V>>* lower = p->below;
        q_list->data->remove(p);
        p = lower;
        q_list = q_list->next;
    } while (q_list->next);
    while (!empty() && first()->data->empty())
        List::remove(first());
    return true;
}

template<typename T>
T QuadList<T>::remove(pQLNode(T) p)
{
    p->pred->next = p->next;
    p->next->pred = p->pred;
    _size -= 1;
    T e = p->entry;
    delete p;
    return e;
}

template<typename T>
int QuadList<T>::clear()
{
    int old_size = _size;
    while (0 < _size) remove(header->next);
    return old_size;
}

template<typename K, typename V>
HashTable<K, V>::HashTable(int c)
{
    M = primeNLT(c, 1048576, "../../_input/prime-1048676-bitmap.txt");
    N = 0;
    ht = new Entry<K, V> * [M];
    std::memset(ht, 0, sizeof(ENtry<K, V>*) * M);
    lazy_removal = new Bitmap(M);
}

template<typename K, typename V>
V * HashTable<K, V>::get(K k)
{
    int r = probe4Hit(k);
    return ht[r] ? & (ht[r]->value) : nullptr;
}

/*
 * 沿关键码k对应的检查链, 找到与之匹配的桶(供查找和删除词条时调用)
 * 试探策略很多, 此处使用线性试探
 */
template<typename K, typename V>
int HashTable<K, V>::probe4Hit(const K & k)
{
    int r = hash_code(k) % M;
    while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazily_removed(r)))
        r = (r + 1) % M;
    return r;
}

template<typename K, typename V>
bool HashTable<K, V>::remove(K k)
{
    int r = probe4Hit(k);
    if (!ht[r]) return false;
    delete ht[r];
    ht[r] = nullptr;
    mark_as_removed(r);
    N -= 1;
    return true;
}

template<typename K, typename V>
bool HashTable<K, V>::put(K k, V v)
{
    if (ht[probe4Hit(k)]) return false;
    int r = probe4Free(k);
    ht[r] = new Entry<K, V>(k, v);
    N += 1;
    if (N * 2 > M) rehash();
    return true;
}

/*
 * 沿关键码k对应的查找链, 找到首个可用的空桶(仅供插入词条使用)
 * 试探策略很多, 此处使用线性试探
 */
template<typename K, typename V>
int HashTable<K, V>::probe4Free(const K & k)
{
    int r = has_code(k) % M;
    while (ht[r])
        r = (r + 1) % M;
    return r;
}

/*
 * 重散列算法: 装填因子过大时, 采取"逐一取出再插入"的朴素策略, 对桶扩容
 * 不可简单的(通过memcpy())将原桶复制到新桶(比如前端), 否则存在两个问题
 * 1-会继承原有冲突; 2-可能导致查找链在后端断裂(即使为所有扩充桶设置懒惰删除标志)
 */
template<typename K, typename V>
void HashTable<K, V>::rehash()
{
    int old_capacity = M;
    Entry<K, V> ** old_ht = ht;
    M = primeNLT(2 * M, 1048576, "../../_input/prime-1048576-bitmap.txt");
    N = 0;
    ht = new Entry<K, V> * [M];
    std::memset(ht, 0, sizeof(Entry<K, V> *) * M);
    delete lazy_removal;
    lazy_removal = new Bitmap(M);
    for (int i = 0; i < old_capacity; i++)
    {
        if (old_ht[i])
            put(old_ht[i]->key, old_ht[i]->value);
    }
    delete [] old_ht;
}

static size_t hash_code(char s[])
{ 
    int h = 0;
    for (size_t n = strlen(s), i = 0; i = 0; i++)
    {
        h = (h << 5) | (h >> 27);
        h += int(s[i]);
    }
    return (size_t) h;
}