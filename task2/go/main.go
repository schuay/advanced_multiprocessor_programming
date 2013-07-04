package main

import (
    "cuckoo"
    "fmt"
)

func main() {
    c := cuckoo.NewSet()

    c.Put(30)
    fmt.Printf("c.Contains(30) = %v\n", c.Contains(30))
    fmt.Printf("c.Size() = %v\n", c.Size())

    c.Remove(30)
    fmt.Printf("c.Contains(30) = %v\n", c.Contains(30))
    fmt.Printf("c.Size() = %v\n", c.Size())
}
