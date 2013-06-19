#include "cuckooset.hpp"

#include <iostream>

#include "hash.hpp"

#define INITIAL_CAPACITY (1024)

template <class Pheet, typename TT, class Comparator>
CuckooSet<Pheet, TT, Comparator>::CuckooSet()
{
    the_capacity = INITIAL_CAPACITY;
    the_size = 0;
    the_table[0] = new ProbeSet<TT, Comparator>[the_capacity];
    the_table[1] = new ProbeSet<TT, Comparator>[the_capacity];
}

template <class Pheet, typename TT, class Comparator>
CuckooSet<Pheet, TT, Comparator>::~CuckooSet()
{
    delete[] the_table[0];
    delete[] the_table[1];
}

template <class Pheet, typename TT, class Comparator>
void
CuckooSet<Pheet, TT, Comparator>::put(const TT &item)
{
    LockGuard lock(this, item);

    if (contains_nolock(item)) {
        return;
    }

    const size_t hash0 = h0(item) % the_capacity;
    const size_t hash1 = h1(item) % the_capacity;

    ProbeSet<TT, Comparator> *set0 = the_table[0] + hash0;
    ProbeSet<TT, Comparator> *set1 = the_table[1] + hash1;

    int i = -1;
    size_t h;
    bool must_resize = false;

    if (set0->size() < PROBE_THRESHOLD) {
        set0->add(item);
        return;
    } else if (set1->size() < PROBE_THRESHOLD) {
        set1->add(item);
        return;
    } else if (set0->size() < PROBE_SIZE) {
        set0->add(item);
        i = 0;
        h = hash0;
    } else if (set1->size() < PROBE_SIZE) {
        set1->add(item);
        i = 1;
        h = hash1;
    } else {
        must_resize = true;
    }

    lock.release();

    if (must_resize) {
        resize();
        put(item);
    } else if (i != -1 && !relocate(i, h)) {
        resize();
    }
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::contains(const TT &item)
{
    LockGuard lock(this, item);
    return contains_nolock(item);
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::contains_nolock(const TT &item)
{
    return false;
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::remove(const TT &item)
{
    LockGuard lock(this, item);

    const size_t hash0 = h0(item) % the_capacity;
    ProbeSet<TT, Comparator> *set0 = the_table[0] + hash0;

    if (set0->contains(item)) {
        set0->remove(item);
        return true;
    }

    const size_t hash1 = h1(item) % the_capacity;
    ProbeSet<TT, Comparator> *set1 = the_table[1] + hash1;

    if (set1->contains(item)) {
        set1->remove(item);
        return true;
    }

    return false;
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::is_empty()
{
    return size() == 0;
}

template <class Pheet, typename TT, class Comparator>
size_t
CuckooSet<Pheet, TT, Comparator>::size()
{
    return the_size.load();
}

template <class Pheet, typename TT, class Comparator>
void
CuckooSet<Pheet, TT, Comparator>::acquire(const TT &item)
{
    the_mutex.lock();
}

template <class Pheet, typename TT, class Comparator>
void
CuckooSet<Pheet, TT, Comparator>::release(const TT &item)
{
    the_mutex.unlock();
}

template <class Pheet, typename TT, class Comparator>
void
CuckooSet<Pheet, TT, Comparator>::resize()
{
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::relocate(const int i, const size_t h)
{
    return false;
}

template <class Pheet, typename TT, class Comparator>
void
CuckooSet<Pheet, TT, Comparator>::print_name()
{
    std::cout << "CuckooSet"; 
}

template class CuckooSet<
    pheet::PheetEnv<
        pheet::BStrategyScheduler,
        pheet::SystemModel,
        pheet::Primitives,
        pheet::DataStructures,
        pheet::ConcurrentDataStructures>,
    unsigned long,
    std::less<unsigned long> >;
