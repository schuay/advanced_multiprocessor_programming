package probe

import (
    "errors"
)

const THRESHOLD int = 8
const SIZE int = 16

type Set interface {
    Add(item int64)
    Contains(item int64) bool
    First() (int64, error)
    Remove(item int64)
    Size() int
}

type set map[int64]bool

func NewSet() Set {
    return make(set)
}

func (p set) Add(item int64) {
    p[item] = true
}

func (p set) Contains(item int64) bool {
    return p[item]
}

func (p set) First() (int64, error) {
    for v := range p {
        return v, nil
    }
    return 0, errors.New("set: First() called on empty map")
}

func (p set) Remove(item int64) {
    delete(p, item)
}

func (p set) Size() int {
    return len(p)
}
