#ifndef VARIATION_H
#define VARIATION_H

#include "movement.h"
#include "chessboard.h"

#include <vector>
#include <iostream>

#define UNDO_FEN_STRING true

class Variation
{
public:
    Variation();
    Variation(Movement& movement);

//private:
    Movement movement_;

#if (UNDO_FEN_STRING)
#else
    Chessboard chessboard_;
#endif
};

typedef std::vector<Variation> Variations;

static void printVariations(const Variations& variations)
{
    for (const Variation& variation : variations)
        std::cout<<variation.movement_.str() << /*" -> " << variation.chessboard_.exportFen() << */ std::endl;
}
#endif