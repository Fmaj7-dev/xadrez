#include "position.h"

Position::Position(uint8_t pos)
: pos_(pos)
{}

uint8_t Position::getValue() const
{
    return pos_;
}

std::string Position::getStr() const
{
    std::string s;
    s += 'a'+pos_%8;
    s += '0' + 8-pos_/8;
    //s += char(0);

    return s;
}

uint8_t Position::char2int( const char* pos )
{
    return (8-pos[1]-'0')*8+(pos[0]-'a');
}
