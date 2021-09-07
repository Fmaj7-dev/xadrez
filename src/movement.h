#include "position.h"

#include <cstdint>

class Movement
{
public:
    Movement( uint8_t from=0, uint8_t to=0 );

    std::string str();

private:
    // index to the array
    Position fromSquare_;
    Position toSquare_;
};