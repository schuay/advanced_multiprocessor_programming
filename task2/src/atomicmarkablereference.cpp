#include "AtomicMarkableReference.hpp"

template<T>
bool
AtomicMarkableReference::compareAndSet(const T expectedReference, const T newReference, const bool expectedMark, const bool newMark)
{
    return true;
}

template<T>
T
AtomicMarkableReference::get(const bool *)
{
    return NULL;
}

template class AtomicMarkableReference<T>;
