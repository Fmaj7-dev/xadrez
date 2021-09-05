#include "position.h"

#include <cstdint>

class Movement
{
public:
    Movement(uint8_t from, uint8_t to);

    std::string str();

private:
    // index to the array
    Position fromSquare_;
    Position toSquare_;
};