#ifndef __CUCKOOLOCK_H
#define __CUCKOOLOCK_H

#include <assert.h>
#include <mutex>

#include "hash.hpp"

/*
 * Provides the 2 dimensional array of mutexes required for CuckooSet.
 */
template <typename TT>
class CuckooLock {
public:
    CuckooLock(const size_t capacity)
        : the_capacity(capacity)
    {
        the_lock[0] = new std::recursive_mutex[the_capacity];
        the_lock[1] = new std::recursive_mutex[the_capacity];
    }

    virtual ~CuckooLock()
    {
        delete[] the_lock[0];
        delete[] the_lock[1];
    }

    void lock(const TT &item)
    {
        const size_t hash0 = h0(item) % the_capacity;
        const size_t hash1 = h1(item) % the_capacity;
        the_lock[0][hash0].lock();
        the_lock[1][hash1].lock();
    }

    void unlock(const TT &item)
    {
        const size_t hash0 = h0(item) % the_capacity;
        const size_t hash1 = h1(item) % the_capacity;
        the_lock[0][hash0].unlock();
        the_lock[1][hash1].unlock();
    }

    void lockAll()
    {
        for (size_t i = 0; i < the_capacity; i++) {
            the_lock[0][i].lock();
        }
    }

    void unlockAll()
    {
        for (size_t i = 0; i < the_capacity; i++) {
            the_lock[0][i].unlock();
        }
    }

    void quiesce()
    {
        for (size_t i = 0; i < the_capacity; i++) {
            the_lock[0][i].lock();
            the_lock[0][i].unlock();
        }
    }

private:
    std::recursive_mutex *the_lock[2];
    const size_t the_capacity;
};

#endif /* __CUCKOOLOCK_H */
