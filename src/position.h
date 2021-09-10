#ifndef POSITION_H
#define POSITION_H

#include <string>

class Position
{
public:
    Position(uint8_t pos);
    std::string str();

    uint8_t getValue() const;

private:
    // from 0 to 63
    uint8_t pos_;

};

#endif