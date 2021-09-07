#ifndef VARIATION_H
#define VARIATION_H

#include "movement.h"
#include "chessboard.h"

#include <vector>

class Variation
{
public:
    Variation();

private:
    Movement movement_;

    Chessboard chessboard_;
};

typedef std::vector<Variation> Variations;

#endif