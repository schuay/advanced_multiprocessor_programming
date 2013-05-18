#ifndef __PCN_H
#define __PCN_H

#include <atomic>
#include <iostream>

#include "periodic.hpp"

thread_local int id = -1;

template <class Pheet, typename T>
class PeriodicCountingNetwork
{
public:
    PeriodicCountingNetwork();
    virtual ~PeriodicCountingNetwork();

    void incr();
    T get_sum();

    static void print_name();

private:
    Periodic *periodic;
    std::atomic<T> *out;

    int pcn_width;
};

/**
 * What's the point of the counting network if the sum is not returned
 * from incr()? If we need to sum up all values, why not just use one
 * atomic<T> per thread and sum those up? Less processing, better locality.
 */

template <class Pheet, typename T>
PeriodicCountingNetwork<Pheet, T>::PeriodicCountingNetwork()
{
    typename Pheet::MachineModel mm;
    int thread_count = std::min(mm.get_num_leaves(), Pheet::Environment::max_cpus);

    pcn_width = 1;
    while (pcn_width < thread_count) {
        pcn_width <<= 1;
    }

    periodic = new Periodic(pcn_width);
    out = new std::atomic<T>[pcn_width];

    for (int i = 0; i < pcn_width; i++) {
        out[i] = i - pcn_width + 1;
    }
}

template <class Pheet, typename T>
PeriodicCountingNetwork<Pheet, T>::~PeriodicCountingNetwork()
{
    delete periodic;
}

template <class Pheet, typename T>
void
PeriodicCountingNetwork<Pheet, T>::incr()
{
    if (id == -1) {
        id = Pheet::get_place_id();
    }
    out[periodic->traverse(id)].fetch_add(pcn_width, std::memory_order_relaxed);
}

template <class Pheet, typename T>
T
PeriodicCountingNetwork<Pheet, T>::get_sum()
{
    T max = 0;
    for (int i = 0; i < pcn_width; i++) {
        const T next = out[i].load(std::memory_order_relaxed);
        if (next > max) {
            max = next;
        }
    }
    return max;
}

template <class Pheet, typename T>
void
PeriodicCountingNetwork<Pheet, T>::print_name()
{
    std::cout << "PeriodicCountingNetwork";
}

#endif /* __PCN_H */
