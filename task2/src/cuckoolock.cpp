#include "cuckoolock.hpp"

#include <assert.h>

#include "hash.hpp"

template <typename TT>
CuckooLock<TT>::CuckooLock(const size_t capacity)
    : the_capacity(capacity)
{
    the_lock[0] = new std::recursive_mutex[the_capacity];
    the_lock[1] = new std::recursive_mutex[the_capacity];
}

template <typename TT>
CuckooLock<TT>::~CuckooLock()
{
    delete[] the_lock[0];
    delete[] the_lock[1];
}

template <typename TT>
void
CuckooLock<TT>::lock(const TT &item)
{
    const size_t hash0 = h0(item) % the_capacity;
    const size_t hash1 = h1(item) % the_capacity;
    the_lock[0][hash0].lock();
    the_lock[1][hash1].lock();
}

template <typename TT>
void
CuckooLock<TT>::unlock(const TT &item)
{
    const size_t hash0 = h0(item) % the_capacity;
    const size_t hash1 = h1(item) % the_capacity;
    the_lock[0][hash0].unlock();
    the_lock[1][hash1].unlock();
}

template <typename TT>
void
CuckooLock<TT>::lockAll()
{
    for (size_t i = 0; i < the_capacity; i++) {
        the_lock[0][i].lock();
    }
}

template <typename TT>
void
CuckooLock<TT>::unlockAll()
{
    for (size_t i = 0; i < the_capacity; i++) {
        the_lock[0][i].unlock();
    }
}

template <typename TT>
void
CuckooLock<TT>::quiesce()
{
    for (size_t i = 0; i < the_capacity; i++) {
        the_lock[0][i].lock();
        the_lock[0][i].unlock();
    }
}

template class CuckooLock<unsigned long>;
