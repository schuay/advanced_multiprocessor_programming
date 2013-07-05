#ifndef __CUCKOOLOCK_H
#define __CUCKOOLOCK_H

#include <mutex>
#include <assert.h>

class CuckooLock {
public:
    CuckooLock(size_t capacity)
    {
        the_capacity = capacity;
        the_lock[0] = new std::recursive_mutex[the_capacity];
        the_lock[1] = new std::recursive_mutex[the_capacity];
    }

    ~CuckooLock()
    {
        delete[] the_lock[0];
        delete[] the_lock[1];
    }

    void lock(size_t r, size_t c)
    {
        assert(r < 2 && c < the_capacity);
        the_lock[r][c].lock();
    }

    void unlock(size_t r, size_t c)
    {
        assert(r < 2 && c < the_capacity);
        the_lock[r][c].unlock();

    }

    void quiesce()
    {
        /*TODO:
          The book uses lock.isLocked() for this, but since std::mutex doesn't provide such a method,
          this is the only way to do it. Might be bad for performance if the locks are not locked'
         */
        for(int i = 0; i < the_capacity; i++) {
            the_lock[0][i].lock();
            the_lock[0][i].unlock();
        }
    }

private:
    std::recursive_mutex *the_lock[2];
    size_t the_capacity;


};


#endif //
