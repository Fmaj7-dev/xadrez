#include "movement.h"

Movement::Movement(uint8_t from, uint8_t to)
: fromSquare_(from),
  toSquare_(to)
{

}

std::string Movement::str()
{
    return fromSquare_.str() + toSquare_.str();
}