#include "cuckooset.hpp"

#include <iostream>

template <class Pheet, typename TT, class Comparator>
void
CuckooSet<Pheet, TT, Comparator>::put(const TT &item)
{
    std::lock_guard<std::mutex> lock(the_mutex);
    the_set.insert(item);
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::contains(const TT &item)
{
    std::lock_guard<std::mutex> lock(the_mutex);
    return (the_set.find(item) != the_set.end());
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::remove(const TT &item)
{
    std::lock_guard<std::mutex> lock(the_mutex);
    return (the_set.erase(item) > 0);
}

template <class Pheet, typename TT, class Comparator>
bool
CuckooSet<Pheet, TT, Comparator>::is_empty()
{
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_set.empty();
}

template <class Pheet, typename TT, class Comparator>
size_t
CuckooSet<Pheet, TT, Comparator>::size()
{
    std::lock_guard<std::mutex> lock(the_mutex);
    return the_set.size();
}

template <class Pheet, typename TT, class Comparator>
size_t
CuckooSet<Pheet, TT, Comparator>::get_length()
{
   return size();
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
