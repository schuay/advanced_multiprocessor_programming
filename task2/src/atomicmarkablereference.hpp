#ifndef __ATOMOC_MARKABLE_REFERENCE_H
#define __ATOMOC_MARKABLE_REFERENCE_H

#include <atomic>

/**
 * A reference and a boolean value wrapped up as one atomic value.
 *
 * Note: Only works for systems using <=63 bits for addressing, since the boolean value is mapped into the reference
 */

template <typename T>
class AtomicMarkableReference {
public:
    AtomicMarkableReference() {}
    virtual ~AtomicMarkableReference() {}

    bool compareAndSet(const T expectedReference, const T newReference, const bool expectedMark, const bool newMark);
    T get(const bool *);

private:
    std::atomic<uint64_t> the_value;

};

#endif /*__ATOMOC_MARKABLE_REFERENCE_H*/
