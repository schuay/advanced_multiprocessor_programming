#ifndef __ATOMIC_MARKABLE_REFERENCE_H
#define __ATOMIC_MARKABLE_REFERENCE_H

#include <atomic>
#include <thread>

/**
 * A reference and a boolean value wrapped up as one atomic value.
 *
 * Note: Only works for systems using <=63 bits for addressing, since the boolean value is mapped into the reference
 */

class AtomicMarkableReference {
public:
    AtomicMarkableReference();
    AtomicMarkableReference(std::thread::id reference,
                            bool mark);
    virtual ~AtomicMarkableReference() {}

    bool compareAndSet(const std::thread::id expectedReference,
                       const std::thread::id newReference,
                       const bool expectedMark,
                       const bool newMark);
    bool attemptMark(const std::thread::id newReference,
                    const bool newMark);
    void reset();
    std::thread::id get(bool *mark);

private:
    bool compareAndSet(const uint64_t expectedReference,
                       const uint64_t newReference,
                       const bool expectedMark,
                       const bool newMark);

private:
    std::atomic<uint64_t> the_value;
};

#endif /* __ATOMIC_MARKABLE_REFERENCE_H */
