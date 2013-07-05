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
        the_value = 0;
    }

    AtomicMarkableReference(std::thread::id reference, bool mark)
    {
        static_assert(sizeof(uint64_t) == sizeof(std::thread::id), "This class heavily relies on std::thread::id being of equal size as uint64_t");
        the_value = (uint64_t) mark | *((uint64_t *) &reference) << 1;
    }

    virtual ~AtomicMarkableReference() {}

    bool compareAndSet(const std::thread::id expectedReference, const std::thread::id newReference, const bool expectedMark, const bool newMark)
    {
        return compareAndSet(*((uint64_t *) &expectedReference), *((uint64_t *) &newReference), expectedMark, newMark);
   }

   bool attemptMark(const std::thread::id newReference, const bool newMark)
   {
        compareAndSet(0, *((uint64_t *) &newReference), false, newMark);
   }

   void reset()
   {
        the_value = 0;
   }


   std::thread::id get(bool *mark)
   {
     *mark = the_value & 1;
     uint64_t val = (the_value >> 1);
     return  *((std::thread::id *) &val);
   }

private:
    bool compareAndSet(const uint64_t expectedReference, const uint64_t newReference, const bool expectedMark, const bool newMark)
    {
        uint64_t expected, update;

        //TODO: casting std::thread::id to uint64_t might be dangerous!! std::thread::id does not give any guarantees except for it to be unique for each thread.
        expected = (uint64_t) expectedMark | expectedReference << 1;
        update = (uint64_t) newMark  | newReference << 1;
        return the_value.compare_exchange_strong(expected, update);
   }


private:
    std::atomic<uint64_t> the_value;

};

#endif /*__ATOMOC_MARKABLE_REFERENCE_H*/
