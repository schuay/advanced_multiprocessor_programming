#ifndef __CUCKOOLOCK_H
#define __CUCKOOLOCK_H

#include <mutex>

/*
 * Provides the 2 dimensional array of mutexes required for CuckooSet.
 */
template <typename TT>
class CuckooLock {
public:
    CuckooLock(const size_t capacity);
    virtual ~CuckooLock();

    void lock(const TT &item);
    void unlock(const TT &item);

    void lockAll();
    void unlockAll();

    void quiesce();

private:
    std::recursive_mutex *the_lock[2];
    const size_t the_capacity;
};

#endif /* __CUCKOOLOCK_H */
