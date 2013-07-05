#ifndef __CUCKOOSET_H
#define __CUCKOOSET_H

#include <atomic>
#include <mutex>
#include <pheet/pheet.h>
#include <vector>

#include "atomicmarkablereference.hpp"
#include "cuckoolock.hpp"
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
    void resize(const size_t capacity);
    bool relocate(const int k, const size_t h);

    /** Checks if the item is contained in this set.
     * A lock must have been acquired before this function is called. */
    bool contains_nolock(const TT &item);
    void quiesce();


private:
    ProbeSet<TT, Comparator> *the_table[2];
    std::atomic<size_t> the_size;
    std::atomic<size_t> the_capacity;
    CuckooLock<TT> *the_lock;
    std::vector<CuckooLock<TT> *> old_locks;
    AtomicMarkableReference the_owner;

private:
    class LockGuard {
    public:
        LockGuard(CuckooSet<Pheet, TT, Comparator> *set, const TT &item);
        ~LockGuard();
        void release();

    private:
        CuckooSet<Pheet, TT, Comparator> *set;
        const TT &item;
        bool is_released;
    };

    class GlobalLockGuard {
    public:
        GlobalLockGuard(CuckooSet<Pheet, TT, Comparator> *set);
        ~GlobalLockGuard();
        void release();

    private:
        CuckooSet<Pheet, TT, Comparator> *set;
        bool is_released;
    };

};

#endif /* __CUCKOOSET_H */
