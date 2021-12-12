#ifndef VARIATION_H
#define VARIATION_H

#include "movement.h"
#include "chessboard.h"

#include <vector>
#include <iostream>

class Variation
{
public:
    Variation();

//private:
    Movement movement_;

    Chessboard chessboard_;
};

typedef std::vector<Variation> Variations;

static void printVariations(const Variations& variations)
{
    for (const Variation& variation : variations)
        std::cout<<variation.movement_.str() << " -> " << variation.chessboard_.exportFen();
}
#endif