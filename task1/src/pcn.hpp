#ifndef __PCN_H
#define __PCN_H

#include <atomic>
#include <iostream>

#include "periodic.hpp"

#define THREAD_COUNT (4)

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
};

template <class Pheet, typename T>
PeriodicCountingNetwork<Pheet, T>::PeriodicCountingNetwork()
{
    periodic = new Periodic(THREAD_COUNT);
    out = new std::atomic<T>[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; i++) {
        out[i] = 0;
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
    out[periodic->traverse(id)].fetch_add(1, std::memory_order_relaxed);
}

template <class Pheet, typename T>
T
PeriodicCountingNetwork<Pheet, T>::get_sum()
{
    T sum = 0;
    for (int i = 0; i < THREAD_COUNT; i++) {
        sum += out[i].load(std::memory_order_relaxed);
    }
    return sum;
}

template <class Pheet, typename T>
void
PeriodicCountingNetwork<Pheet, T>::print_name()
{
    std::cout << "PeriodicCountingNetwork";
}

#endif /* __PCN_H */
