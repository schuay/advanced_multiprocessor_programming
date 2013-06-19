#include "hash.hpp"

template <>
size_t
h0(const unsigned long &item)
{
    return item; /* TODO */
}

template <>
size_t
h1(const unsigned long &item)
{
    return item + 10; /* TODO */
}

