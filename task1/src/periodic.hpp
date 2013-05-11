#ifndef __PERIODIC_H
#define __PERIODIC_H

#include "block.hpp"

class Periodic
{
public:
    Periodic(const int width);
    virtual ~Periodic();

    int traverse(const int input);

private:
    const int width;
    int block_count;
    Block **blocks;
};

static int log2(const int in)
{
    int out = 0;
    int w = in;
    while (w >>= 1) {
        out++;
    }
    return out;
}

Periodic::Periodic(const int width) :
    width(width)
{
    block_count = log2(width);
    blocks = new Block*[block_count];
    for (int i = 0; i < block_count; i++) {
        blocks[i] = new Block(width);
    }
}

Periodic::~Periodic()
{
    for (int i = 0; i < block_count; i++) {
        delete blocks[i];
    }
    delete[] blocks;
}

int
Periodic::traverse(const int input)
{
    int wire = input;
    for (int i = 0; i < block_count; i++) {
        wire = blocks[i]->traverse(wire);
    }
    return wire;
}

#endif /* __PERIODIC_H */
