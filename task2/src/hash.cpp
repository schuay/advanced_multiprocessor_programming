#include "hash.hpp"

/* Hash functions taken from
 * http://eternallyconfuzzled.com/tuts/algorithms/jsw_tut_hashing.aspx */

static unsigned long
oat_hash(const void *key,
         const int len)
{
    unsigned char *p = (unsigned char *)key;
    unsigned long h = 0;
    int i;

    for (i = 0; i < len; i++) {
        h += p[i];
        h += (h << 10);
        h ^= (h >> 6);
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h;
}

static unsigned
elf_hash(const void *key,
         const int len)
{
    unsigned char *p = (unsigned char *)key;
    unsigned long h = 0, g;
    int i;
    for (i = 0; i < len; i++) {
        h = (h << 4) + p[i];
        g = h & 0xf0000000L;

        if (g != 0) {
            h ^= g >> 24;
        }

        h &= ~g;
    }

    return h;
}

template <>
size_t
h0(const unsigned long &item)
{
    return oat_hash(&item, sizeof(item));
}

template <>
size_t
h1(const unsigned long &item)
{
    return elf_hash(&item, sizeof(item));
}

