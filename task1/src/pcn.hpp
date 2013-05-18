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

    int thread_count;
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
	thread_count = std::min(mm.get_num_leaves(), Pheet::Environment::max_cpus);

    periodic = new Periodic(thread_count);
    out = new std::atomic<T>[thread_count];

    for (int i = 0; i < thread_count; i++) {
        out[i] = i - thread_count + 1;
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
    out[periodic->traverse(id)].fetch_add(thread_count, std::memory_order_relaxed);
}

template <class Pheet, typename T>
T
PeriodicCountingNetwork<Pheet, T>::get_sum()
{
    T max = 0;
    for (int i = 0; i < thread_count; i++) {
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
