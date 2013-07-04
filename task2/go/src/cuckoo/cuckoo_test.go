package cuckoo

import (
    "runtime"
    "sync"
    "testing"
)

func Test_Empty(t *testing.T) {
    s := NewSet()
    if !s.IsEmpty() {
        t.Fail()
    }
}

func Test_Put(t *testing.T) {
    s := NewSet()
    s.Put(42)
    if s.Size() != 1 {
        t.Fail()
    }
    if !s.Contains(42) {
        t.Fail()
    }
    if s.Contains(41) {
        t.Fail()
    }
}

func Test_PutDuplicate(t *testing.T) {
    s := NewSet()
    s.Put(42)
    s.Put(42)
    if s.Size() != 1 {
        t.Fail()
    }
    if !s.Contains(42) {
        t.Fail()
    }
    if s.Contains(41) {
        t.Fail()
    }
}

func Test_Remove(t *testing.T) {
    s := NewSet()
    s.Put(42)
    s.Put(43)
    s.Remove(42)
    if s.Size() != 1 {
        t.Fail()
    }
    if !s.Contains(43) {
        t.Fail()
    }
    if s.Contains(42) {
        t.Fail()
    }
}

func Test_Smoke1000(t *testing.T) {
    test_Smoke(t, 1000)
}

func Test_Smoke10000(t *testing.T) {
    test_Smoke(t, 10000)
}

func Test_Smoke123456(t *testing.T) {
    test_Smoke(t, 123456)
}

func test_Smoke(t *testing.T, n int) {
    threads :=  runtime.NumCPU()
    runtime.GOMAXPROCS(threads)
    s := NewSet()
    step := n / threads

    var wg sync.WaitGroup
    wg.Add(threads * 2)

    for i := 0; i < threads; i++ {
        go func(i int) {
            defer wg.Done()
            for j := i * step; j < (i + 1) * step; j++ {
                s.Put(int64(j))
            }
        }(i)
        go func(i int) {
            defer wg.Done()
            for j := i * step; j < (i + 1) * step; j++ {
                s.Contains(int64(j))
            }
        }(i)
    }

    wg.Wait()

    if s.Size() != step * threads {
        t.Errorf("Unexpected Size(): %d != %d\n", s.Size(), step * threads)
    }

    wg.Add(threads)

    for i := 0; i < threads; i++ {
        go func(i int) {
            defer wg.Done()
            for j := i * step; j < (i + 1) * step; j++ {
                s.Remove(int64(j))
            }
        }(i)
    }

    wg.Wait()

    if s.Size() != 0 {
        t.Errorf("Unexpected Size(): %d != %d\n", s.Size(), step * threads)
    }

    wg.Add(threads * 2)

    for i := 0; i < threads; i++ {
        go func(i int) {
            defer wg.Done()
            for j := i * step; j < (i + 1) * step; j++ {
                s.Put(int64(j))
            }
        }(i)
        go func(i int) {
            defer wg.Done()
            for j := i * step; j < (i + 1) * step; j++ {
                s.Remove(int64(j))
            }
        }(i)
    }

    wg.Wait()
}

func Benchmark_1Thread(b *testing.B) {
    s := NewSet()

    threads := runtime.NumCPU()
    step := b.N / threads

    var wg sync.WaitGroup
    wg.Add(threads)

    b.ResetTimer()
    for i := 0; i < threads; i++ {
        go func(i int) {
            defer wg.Done()
            for j := i * step; j < (i + 1) * step; j++ {
                s.Put(int64(j))
            }
        }(i)
    }

    wg.Wait()
}
