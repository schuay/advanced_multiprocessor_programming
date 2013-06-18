#include "cuckooset.hpp"

#include <iostream>

template <class Pheet, typename TT, class Comparator>
void
CuckooSet<Pheet, TT, Comparator>::put(const TT &item)
{
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::contains(const TT &item)
{
    return false;
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::remove(const TT &item)
{
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
