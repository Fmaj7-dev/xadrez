#include "movement.h"

Movement::Movement(uint8_t from, uint8_t to, Type type, char info )
: fromSquare_(from),
  toSquare_(to),
  type_(type),
  extraInfo_(info)
{

}

std::string Movement::str() const
{
    if ( fromSquare_.getValue() == 0 && toSquare_.getValue() == 0 )
        return std::string("null");

    std::string move = fromSquare_.getStr() + toSquare_.getStr();

    if (type_ == Type::Promotion)
        move += extraInfo_;

    return move;
}

const Position& Movement::from()
{
    return fromSquare_;
}

const Position& Movement::to()
{
    return toSquare_;
}

const Movement::Type Movement::type()
{
    return type_;
}

const char Movement::info()
{
    return extraInfo_;
}