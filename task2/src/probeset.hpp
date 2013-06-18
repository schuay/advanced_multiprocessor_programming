#ifndef __PROBESET_H
#define __PROBESET_H

#include <cstddef>
#include <set>

#define PROBE_THRESHOLD (8)
#define PROBE_SIZE (16)

template <typename T,
          class Comparator = std::less<T> >
class ProbeSet {
public:
    void add(const T &item);
    bool contains(const T &item) const;
    void remove(const T &item);
    size_t size() const;

private:
    std::set<T, Comparator> the_set;
};

#endif /* __PROBESET_H */
