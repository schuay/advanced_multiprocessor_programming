#ifndef __BALANCER_H
#define __BALANCER_H

#include <atomic>

class Balancer
{
public:
    Balancer();

    int traverse();
private:
    std::atomic<int> toggle;
};

Balancer::Balancer() :
    toggle(0)
{
}

int
Balancer::traverse()
{
    return toggle.fetch_xor(1);
}

#endif /* __BALANCER_H */
