#ifndef __PCN_H
#define __PCN_H

#include <atomic>
#include <iostream>

template <class Pheet, typename T>
class PeriodicCountingNetwork
{
public:
    PeriodicCountingNetwork();
    ~PeriodicCountingNetwork();

    void incr();
    T get_sum();

    static void print_name();

private:
    std::atomic<T> sum;
};

template <class Pheet, typename T>
PeriodicCountingNetwork<Pheet, T>::PeriodicCountingNetwork() :
    sum(0)
{
}

template <class Pheet, typename T>
PeriodicCountingNetwork<Pheet, T>::~PeriodicCountingNetwork()
{
}

template <class Pheet, typename T>
void
PeriodicCountingNetwork<Pheet, T>::incr()
{
    sum++;
}

template <class Pheet, typename T>
T
PeriodicCountingNetwork<Pheet, T>::get_sum()
{
    return sum;
}

template <class Pheet, typename T>
void
PeriodicCountingNetwork<Pheet, T>::print_name()
{
    std::cout << "PeriodicCountingNetwork";
}

#endif /* __PCN_H */
