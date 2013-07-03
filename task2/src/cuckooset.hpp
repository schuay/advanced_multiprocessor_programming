#ifndef __CUCKOOSET_H
#define __CUCKOOSET_H

#include <atomic>
#include <mutex>
#include <vector>
#include <pheet/pheet.h>

#include "probeset.hpp"

/**
 * A set implemented using cuckoo hashing.
 */

template <class Pheet,
          typename TT,
          class Comparator = std::less<TT> >
class CuckooSet {
public:
    CuckooSet();
    virtual ~CuckooSet();

    void put(const TT &item);
    bool contains(const TT &item);
    bool remove(const TT &item);

    bool is_empty();
    size_t size();

    static void print_name();

private:
    void acquire(const TT &item);
    void release(const TT &item);
    void resize();
    bool relocate(const int k, const size_t h);

    /** Checks if the item is contained in this set.
     * A lock must have been acquired before this function is called. */
    bool contains_nolock(const TT &item);

private:
    ProbeSet<TT, Comparator> *the_table[2];
    std::mutex the_mutex;
    std::atomic<size_t> the_size;
    std::atomic<size_t> the_capacity;
    
    static const size_t LOCK_CAPACITY = 5;
    std::mutex *the_lock[2][LOCK_CAPACITY];

private:
    class LockGuard {
    public:
        LockGuard(CuckooSet<Pheet, TT, Comparator> *set, const TT &item)
            : set(set), item(item), is_released(false)
        {
            set->acquire(item);
        }

        void release()
        {
            set->release(item);
            is_released = true;
        }

        ~LockGuard()
        {
            if (!is_released) {
                set->release(item);
            }
        }

    private:
        CuckooSet<Pheet, TT, Comparator> *set;
        const TT &item;
        bool is_released;
    };

private:
    class LockGuardAll {
    public:
        LockGuardAll(CuckooSet<Pheet, TT, Comparator> *set)
            : set(set), is_released(false)
        {
            //set->the_mutex.lock();
            for(int i = 0; i < LOCK_CAPACITY; i++) {
                set->the_lock[0][i]->lock();
            }
        }

        void release()
        {
            releaseAll();
        }

        ~LockGuardAll()
        {
            releaseAll();
        }
    
    private:
        void releaseAll()
        {
            if(!is_released) {
                for(int i = 0; i < LOCK_CAPACITY; i++) {
                    set->the_lock[0][i]->unlock(); 
                    
                } 
                //set->the_mutex.unlock();  
            }
            is_released=true;
        }
    
    private:
        CuckooSet<Pheet, TT, Comparator> *set;
        bool is_released;
    };

};

#endif /* __CUCKOOSET_H */
