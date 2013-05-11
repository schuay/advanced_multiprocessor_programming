#ifndef __BLOCK_H
#define __BLOCK_H

#include "layer.hpp"

class Block
{
public:
    Block(const int width);
    virtual ~Block();

    int traverse(const int input);

private:
    const int width;
    Block *north, *south;
    Layer layer;
};

Block::Block(const int width) :
    width(width),
    layer(width)
{
    if (width > 2) {
        north = new Block(width >> 1);
        south = new Block(width >> 1);
    } else {
        north = south = NULL;
    }
}

Block::~Block()
{
    delete north;
    delete south;
}

int
Block::traverse(const int input)
{
    const int wire = layer.traverse(input);

    if (width <= 2) {
        return wire;
    }

    if (wire < width / 2) {
        return north->traverse(wire);
    } else {
        return width / 2 + south->traverse(wire - (width / 2));
    }
}

#endif /* __BLOCK_H */
