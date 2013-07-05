#include "probeset.hpp"

template<typename T, class Comparator>
void
ProbeSet<T, Comparator>::add(const T &item)
{
    the_set.insert(item);
}

template<typename T, class Comparator>
T
ProbeSet<T, Comparator>::first() const
{
    return *the_set.begin();
}

template<typename T, class Comparator>
bool
ProbeSet<T, Comparator>::contains(const T &item) const
{
    return (the_set.find(item) != the_set.end());
}

template<typename T, class Comparator>
void
ProbeSet<T, Comparator>::remove(const T &item)
{
    the_set.erase(item);
}

template<typename T, class Comparator>
size_t
ProbeSet<T, Comparator>::size() const
{
    return the_set.size();
}

template class ProbeSet<
    unsigned long,
    std::less<unsigned long>>;
