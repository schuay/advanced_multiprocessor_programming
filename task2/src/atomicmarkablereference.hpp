#ifndef __ATOMOC_MARKABLE_REFERENCE_H
#define __ATOMOC_MARKABLE_REFERENCE_H

#include <atomic>
#include <thread>

/**
 * A reference and a boolean value wrapped up as one atomic value.
 *
 * Note: Only works for systems using <=63 bits for addressing, since the boolean value is mapped into the reference
 */

//template <typename T>
class AtomicMarkableReference {
public:
    AtomicMarkableReference()
    {
        static_assert(sizeof(uint64_t) == sizeof(std::thread::id), "This class heavily relies on std::thread::id being of equal size as uint64_t");
    }

    virtual ~AtomicMarkableReference() {}

    bool compareAndSet(const std::thread::id expectedReference, const std::thread::id newReference, const bool expectedMark, const bool newMark)
    {
        uint64_t expected, update;

        //TODO: casting std::thread::id to uint64_t might be dangerous!! std::thread::id does not give any guarantees except for it to be unique for each thread.
        expected = (uint64_t) expectedMark | *((uint64_t *) &expectedReference) << 1;
        update = (uint64_t) newMark  | *((uint64_t *) &newReference) << 1;
        return the_value.compare_exchange_strong(expected, update);
    }

   std::thread::id get(bool *mark)
   {
     *mark = the_value & 1;
     return  *((std::thread::id *) (the_value >> 1));
   }

private:
    std::atomic<uint64_t> the_value;

};

#endif /*__ATOMOC_MARKABLE_REFERENCE_H*/
