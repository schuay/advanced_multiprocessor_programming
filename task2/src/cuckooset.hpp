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
    CuckooSet() { }
    virtual ~CuckooSet() { }

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

private:
    ProbeSet<TT, Comparator> *the_table[2];
    std::mutex the_mutex;
    std::atomic<size_t> the_size;
};

#endif /* __CUCKOOSET_H */
