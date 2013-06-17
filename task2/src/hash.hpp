#ifndef __HASH_H
#define __HASH_H

#include <cstddef>

using std::size_t;

/**
 * The first hash function used by our cuckoo hashing.
 * An instance of this function must exist for each hash set
 * element type.
 */

template <typename T>
size_t
h0(const T &item);

/**
 * The second hash function used by our cuckoo hashing.
 * An instance of this function must exist for each hash set
 * element type.
 */

template <typename T>
size_t
h1(const T &item);

#endif /* __HASH_H */
