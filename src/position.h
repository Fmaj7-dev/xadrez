#include <string>


class Position
{
public:
    Position(uint8_t pos);
    std::string str();

private:
    // from 0 to 63
    uint8_t pos_;

};