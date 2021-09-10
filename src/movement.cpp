#include "movement.h"

Movement::Movement(uint8_t from, uint8_t to)
: fromSquare_(from),
  toSquare_(to)
{

}

std::string Movement::str()
{
    if ( fromSquare_.getValue() == 0 && toSquare_.getValue() == 0 )
        return std::string("null");

    return fromSquare_.str() + toSquare_.str();
}

const Position& Movement::from()
{
    return fromSquare_;
}

const Position& Movement::to()
{
    return toSquare_;
}
