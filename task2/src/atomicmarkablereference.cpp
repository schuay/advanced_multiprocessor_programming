#include "AtomicMarkableReference.hpp"


//template<T>
AtomicMarkableReference::AtomicMarkableReference()
{
    static_assert(sizeof(uint32_t) == sizeof(std::thread::id), "This class heavily relies on T being of equal size as uint64_t");
}


//template<T>
bool
AtomicMarkableReference::compareAndSet(const std::thread::id expectedReference, const std::thread::id newReference, const bool expectedMark, const bool newMark)
{


    return true;
}

//template<T>
//T
std::thread::id
AtomicMarkableReference::get(const bool *)
{
    return NULL;
}

//template class AtomicMarkableReference<T>;
