#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "position.h"

#include <cstdint>

class Movement
{
public:
    enum class Type
    {
        Normal,
        DoublePawnStep,
        Castling,
        Promotion,
        EnPassantCapture
    };

    Movement( uint8_t from=0, uint8_t to=0, Type type = Movement::Type::Normal, char info = 0 );

    std::string str() const;

    const Position& from();
    const Position& to();
    const Type type();
    const char info();

private:
    // index to the array
    Position fromSquare_;
    Position toSquare_;

    Type type_;

    // in the case of promotion, this is the piece that will be promoted to.
    // in the case of castling, this is the rook that will be moved.
    // in the case of en passant, this is the pawn that will be captured.
    char extraInfo_;
};

#endif
