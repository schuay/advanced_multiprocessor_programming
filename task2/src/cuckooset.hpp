#ifndef __CUCKOOSET_H
#define __CUCKOOSET_H

#include <atomic>
#include <mutex>
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

    /** Checks if the item is contained in this set.
     * A lock must have been acquired before this function is called. */
    bool contains_nolock(const TT &item);

private:
    ProbeSet<TT, Comparator> *the_table[2];
    std::mutex the_mutex;
    std::atomic<size_t> the_size;
    std::atomic<size_t> the_capacity;

private:
    class LockGuard {
    public:
        LockGuard(CuckooSet<Pheet, TT, Comparator> *set, const TT &item)
            : set(set), item(item) { set->acquire(item); }
        ~LockGuard() { set->release(item); }

    private:
        CuckooSet<Pheet, TT, Comparator> *set;
        const TT &item;
    };
};

#endif /* __CUCKOOSET_H */
