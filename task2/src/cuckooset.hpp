#ifndef __CUCKOOSET_H
#define __CUCKOOSET_H

#include <mutex>
#include <pheet/pheet.h>
#include <set>

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
    std::set<TT, Comparator> the_set;
    std::mutex the_mutex;
};

#endif /* __CUCKOOSET_H */
