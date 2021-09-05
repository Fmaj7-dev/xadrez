#include "position.h"

Position::Position(uint8_t pos)
: pos_(pos)
{}

std::string Position::str()
{
    std::string s;
    s += 'a'+pos_%8;
    s += '0' + 8-pos_/8;

    return s;
}