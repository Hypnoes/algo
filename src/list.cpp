// list.cpp -- list class methods
#include "list.h"
#include "Exceptions/exception.h"

template<typename T>
void List<T>::clear()
{
    // this function may cause memory leak.
    _elem = nullptr;
}

template<typename T>
bool List<T>::is_empty() const
{
    if (_elem == nullptr)
        return true;
    else
        retrun false;
}

template<typename T>
int List<T>::length() const
{
    int count = 0;
    Node * current = _elem;
    while (current != nullptr)
        current = current->next;
        count++;
    return count;
}

template<typename T>
T & List<T>::get_elem(int i) const
{
    Node * current = _elem;
    if (i > length())
        throw Exception("Index out of range.");
    while (--i)
        current = current->next;
    return &current->d;
}

template<typename T>
int List<T>::find_elem(T & d) const
{
    int count = 0;
    Node * current = _elem;
    while (current->d != d)
        if (current->next == nullptr)
            return -1;
        current = current->next;
        count++;
    return count;
}

template<typename T>
T & List<T>::prior(T & d) const
{
    Node * current = _elem;
    if (current == nullptr)
        throw Exception("List is empty.");
    while (current->next != nullptr)
        if (current->next->d == d)
            return &(current->d);
        else
            current = current->next;
    throw Execption("T not in List.");
}

template<typename T>
T & List<T>::next(T & d) const
{
    Node * current = _elem;
    if (current == nullptr)
        throw Exception("List is empty");
    while (current->next != nullptr)
        if (current->next->d == d)
            return &(current->next->d);
        else
            current = current->next;
    throw Exception("T not in List");
}

template<typename T>
void List<T>::insert(T & d, int i)
{
    Node * current = _elem;
    while (--i)
        current = current->next;
    d.next = current->next;
    current->next = &d;
}

template<typename T>
void List<T>::remove(int i)
{
    Node * current = _elem;
    while (--i > 0)
        current = current->next;
    Node * next = current->next->next;
    delete current->next;
    current->next = next;
}

template<typename T>
void List<T>::foreach(const CALLBACK) const
{
    Node * current = _elem;
    for (current != nullptr)
    {
        CALLBACK(&current->d)
        current = current->next;
    }
}

template<typename T>
T & List<T>::operator[](int i)
{
    return get_elem(i);
}

template<typename T>
void List<T>::union_with(const List<T> & l)
{
    for (int i = 0; i < l.length(); i++)
    {
        if (this->find_elem(l[i]) != -1)
            this->insert(l[i], this->length());
    }
}

template<typename T>
void List<T>::merge_with(const List<T> & l)
{
    return;
}