package cuckoo

import (
    "encoding/binary"
    "probe"
    "sync"
    "sync/atomic"
)

type itemLocker interface {
    Lock(item int64)
    LockAll()
    Unlock(item int64)
    UnlockAll()
}

type itemLock struct {
    s *set
}

func (i *itemLock) Lock(item int64) {
    i.s.mutex[0][h0(item) % uint(LOCK_CAPACITY)].Lock()
    i.s.mutex[1][h1(item) % uint(LOCK_CAPACITY)].Lock()
}

func (i *itemLock) Unlock(item int64) {
    i.s.mutex[0][h0(item) % uint(LOCK_CAPACITY)].Unlock()
    i.s.mutex[1][h1(item) % uint(LOCK_CAPACITY)].Unlock()
}

func (i *itemLock) LockAll() {
    for j := 0; j < LOCK_CAPACITY; j++ {
        i.s.mutex[0][j].Lock()
    }
}

func (i *itemLock) UnlockAll() {
    for j := 0; j < LOCK_CAPACITY; j++ {
        i.s.mutex[0][j].Unlock()
    }
}

func newItemLock(s *set) *itemLock {
    i := new(itemLock)
    i.s = s
    return i
}

type noopLock int8

func (i noopLock) Lock(item int64) {
    /* Nothing */
}

func (i noopLock) Unlock(item int64) {
    /* Nothing */
}

func (i noopLock) LockAll() {
    /* Nothing */
}

func (i noopLock) UnlockAll() {
    /* Nothing */
}

func newNoopLock() *noopLock {
    var lock noopLock
    return &lock
}

type Set interface {
    Put(item int64)
    Contains(item int64) bool
    Remove(item int64) bool

    IsEmpty() bool
    Size() int
}

const INITIAL_CAPACITY int64 = 1024
const LOCK_CAPACITY int = 1024 /* Must divide all possible capacities evenly. */

type set struct {
    size, capacity int64
    table [2][]probe.Set
    mutex [2][]sync.Mutex
}

func NewSet() Set {
    s := new(set)
    s.capacity = INITIAL_CAPACITY;
    s.initSets(int(INITIAL_CAPACITY))
    s.mutex[0] = make([]sync.Mutex, LOCK_CAPACITY)
    s.mutex[1] = make([]sync.Mutex, LOCK_CAPACITY)
    return s
}

func (s *set) putStep1(item int64) (i, h int, mustResize bool) {
    i = -1

    lock := newNoopLock()
    if s.contains(item, lock) {
        return
    }

    hash0, hash1 := s.h0(item), s.h1(item)
    p0, p1 := s.table[0][hash0], s.table[1][hash1]

    switch {
    case p0.Size() < probe.THRESHOLD:
        p0.Add(item)
        atomic.AddInt64(&s.size, 1)
        return
    case p0.Size() < probe.THRESHOLD:
        p1.Add(item)
        atomic.AddInt64(&s.size, 1)
        return
    case p0.Size() < probe.SIZE:
        p0.Add(item)
        atomic.AddInt64(&s.size, 1)
        i = 0
        h = hash0
        return
    case p1.Size() < probe.SIZE:
        p1.Add(item)
        atomic.AddInt64(&s.size, 1)
        i = 1
        h = hash1
        return
    default:
        mustResize = true
    }
    return
}

func (s *set) Put(item int64) {
    s.put(item, newItemLock(s))
}

func (s *set) put(item int64, lock itemLocker) {
    lock.Lock(item)
    capacity := s.capacity
    i, h, mustResize := s.putStep1(item)
    lock.Unlock(item)

    if mustResize {
        s.resize(capacity, lock)
        s.Put(item)
    } else if i != -1 && !s.relocate(i, h, lock) {
        s.resize(capacity, lock)
    }
}

func (s *set) Contains(item int64) bool {
    lock := newItemLock(s)
    return s.contains(item, lock)
}

func (s *set) contains(item int64, lock itemLocker) bool {
    lock.Lock(item)
    defer lock.Unlock(item)

    p0, p1 := s.table[0][s.h0(item)], s.table[1][s.h1(item)]
    return p0.Contains(item) || p1.Contains(item)
}

func (s *set) Remove(item int64) bool {
    lock := newItemLock(s)
    lock.Lock(item)
    defer lock.Unlock(item)

    p0, p1 := s.table[0][s.h0(item)], s.table[1][s.h1(item)]
    for _, p := range []probe.Set{p0, p1} {
        if p.Contains(item) {
            p.Remove(item)
            atomic.AddInt64(&s.size, -1)
            return true
        }
    }

    return false
}

func (s *set) IsEmpty() bool {
    return s.Size() == 0
}

func (s *set) Size() int {
    u := uint64(s.size)
    return int(atomic.LoadUint64(&u))
}

func (s *set) resize(capacity int64, lock itemLocker) {
    lock.LockAll()
    defer lock.UnlockAll()

    if capacity != s.capacity {
        return
    }

    prevCapacity := s.capacity
    s.capacity *= 2

    ps0, ps1 := s.table[0], s.table[1]

    s.initSets(int(s.capacity))

    noopLock := newNoopLock()
    for i := 0; i < int(prevCapacity); i++ {
        ps := [...]probe.Set{ps0[i], ps1[i]}
        for _, p := range ps {
            for p.Size() > 0 {
                if elem, err := p.First(); err != nil {
                    panic(err)
                } else {
                    p.Remove(elem)
                    s.put(elem, noopLock)
                }
            }
        }
    }
}

func (s *set) initSets(capacity int) {
    s.table[0] = make([]probe.Set, capacity)
    s.table[1] = make([]probe.Set, capacity)
    for i := 0; i < int(capacity); i++ {
        s.table[0][i] = probe.NewSet()
        s.table[1][i] = probe.NewSet()
    }
    s.size = 0
}

func (s *set) relocate(k, h int, lock itemLocker) bool {
    const RELOCATE_LIMIT int = 512

    hi, hj, i, j := h, 0, k, 1 - k

    for round := 0; round < RELOCATE_LIMIT; round++ {
        if ret, keepGoing := s.relocateOne(&hi, &hj, &i, &j, lock); !keepGoing {
            return ret
        }
    }

    return false
}

func (s *set) relocateOne(hi, hj, i, j *int, lock itemLocker) (retCode, keepGoing bool) {
    pi := s.table[*i][*hi]
    if pi == nil {
        return false, false
    }

    y, ok := pi.First()
    if ok != nil {
        return true, true
    }

    lock.Lock(y)
    defer lock.Unlock(y)

    if *i == 0 {
        *hj = s.h1(y)
    } else {
        *hj = s.h0(y)
    }

    pj := s.table[*j][*hj]

    switch {
    case pi.Contains(y):
        pi.Remove(y)
        switch {
        case pj.Size() < probe.THRESHOLD:
            pj.Add(y)
            return true, false
        case pj.Size() < probe.SIZE:
            pj.Add(y)
            *j = *i
            *i = 1 - *i
            hi = hj
        default:
            pi.Add(y)
            return false, false
        }
    case pi.Size() >= probe.THRESHOLD:
        return true, true
    }
    return true, false
}

func (s *set) h0(key int64) int {
    return int(h0(key) % uint(s.capacity))
}

func (s *set) h1(key int64) int {
    return int(h1(key) % uint(s.capacity))
}

func h0(key int64) uint {
    buf := make([]byte, 8)
    bytes := binary.PutVarint(buf, key)
    var h uint = 0

    for i := 0; i < bytes; i++ {
        h += uint(buf[i])
        h += (h << 10)
        h ^= (h >> 6)
    }

    h += (h << 3);
    h ^= (h >> 11);
    h += (h << 15);

    return h
}

func h1(key int64) uint {
    buf := make([]byte, 8)
    bytes := binary.PutVarint(buf, key)
    var h, g uint = 0, 0

    for i := 0; i < bytes; i++ {
        h = (h << 4) + uint(buf[i])
        g = h & 0xf0000000
        if g != 0 {
            h ^= (g >> 24)
        }
        h &= ^g
    }

    return h
}
