// dict.h -- Map and Hash and dict
#ifndef DICT_H_
#define DICT_H_
#include "list.h"
#include "search.h"
#include "bitmap.h"

#define pQLNode(T) QuadListNode<T> *
template<typename T>
struct QuadListNode
{
    T entry;
    pQLNode(T) pred;
    pQLNode(T) next;
    pQLNode(T) above;
    pQLNode(T) below;
    QuadListNode(T e = T(), pQLNode(T) p = nullptr, pQLNode(T) s = nullptr,
        pQLNode(T) a = nullptr, pQLNode(T) b = nullptr) :
        entry(e), pred(p), next(s), above(a), below(b) {}
    pQLNode(T) insert_as_next_above(T const & e, pQLNode(T) b = nullptr);
};

template<typename T>
class QuadList
{
  private:
    int _size;
    pQLNode(T) header;
    pQLNode(T) trailer;

  protected:
    void init();
    int clear();

  public:
    QuadList() { init(); }
    ~QuadList();

    Rank size() const { return _size; }
    bool empty() const { return _size <= 0; }
    pQLNode(T) first() const { return header->next; }
    pQLNode(T) last() const { return trailer->pred; }
    bool vaild(pQLNode(T));

    T remove(pQLNode(T));
    pQLNode(T) insert_after_above(T const &, pQLNode(T), pQLNode(T) b = nullptr);

    void traverse(void (*)(T &));
    template<typename VST> void traverse(VST &);
};

template<typename K, typename V>
struct Dictionary
{
    virtual int size() const = 0;
    virtual bool put(K, V) = 0;
    virtual V * get(K k) = 0;
    virtual bool remove(K k) = 0;
};

template<typename K, typename V>
class SkipList : public Dictionary<K, V>, public List<QuadList<Entry<K, V>>*>
{
  protected:
    bool skip_search(
        ListNode<QuadList<Entry<K, V>>*>* & q_list,
        QuadListNode<Entry<K, V>>* & p,
        K & k);
    
  public:
    int size() const { return empty() ? 0 : last()->data->size(); }
    int level() { return List::size(); }
    bool put(K, V);
    V * get(K k);
    bool remove(K k);
};

template<typename K, typename V>
class HashTable : public Dictionary<K, V>
{
  private:
    Entry<K, V>** ht;
    int M;
    int N;
    Bitmap * lazy_removal;
#define lazily_removed(x) (lazy_removal->test(x))
#define mark_as_removed(x) (lzay_removal->set(x))
  protected:
    int probe4Hit(const K & k);
    int probe4Free(const K & k);
    void rehash();

  public:
    HashTable(int c = 5);
    ~HashTable();
    int size() const { return N; }
    bool put(K, V);
    V * get(K k);
    bool remove(K k);
};

static size_t hash_code(char c) { return (size_t) c; }
static size_t hash_code(int k) { return (size_t) k; }
static size_t hash_code(long long i) { return (size_t) i; }
static size_t hash_code(char s[]);
#endif