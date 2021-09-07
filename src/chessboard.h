#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "movement.h"

#include <iostream>

class Chessboard
{
    enum class PLAYER 
    {
        WHITE,
        BLACK
    };

    enum class CASTLING : uint8_t
    {
        WHITE_KING  = 1 << 0,
        WHITE_QUEEN = 1 << 1,
        BLACK_KING  = 1 << 2,
        BLACK_QUEEN = 1 << 3
    };

    const char WHITE_PAWN = 'P';
    const char WHITE_ROOK = 'R';
    const char WHITE_KNIGHT = 'N';
    const char WHITE_BISHOP = 'B';
    const char WHITE_KING = 'K';
    const char WHITE_QUEEN = 'Q';
    const char BLACK_PAWN = 'p';
    const char BLACK_ROOK = 'r';
    const char BLACK_KNIGHT = 'n';
    const char BLACK_BISHOP = 'b';
    const char BLACK_KING = 'k';
    const char BLACK_QUEEN = 'q';

    const float PAWN_WEIGHT = 1.0f;
    const float ROOK_WEIGHT = 5.0f;
    const float KNIGHT_WEIGHT = 3.0f;
    const float BISHOP_WEIGHT = 3.0f;
    const float QUEEN_WEIGHT = 9.0f;
    const float KING_WEIGHT = 1000.0f;

    //typedef std::vector<Chessboard> ChessboardArray;

public:
    Chessboard();
    
    void initDefault();

    // fem 
    void importFem(const std::string& fem);
    std::string exportFem() const;

    // variations
    /*void appendVariation(ChessboardArray& array, );
    ChessboardArray findVariations();
    ChessboardArray findPawnVariations(int square, PLAYER player);*/

    bool isSquareOccupied( int square );
    bool isSquareWhite( int square );
    bool isSquareBlack( int square );

    // evaluation
    float evaluation();

    void print() const;

private:
    // 64 squares of the board starting top-left
    uint8_t data_[64];

    // 'b' or 'w'
    char turn_;

    // castling flags, only four least significant bits used: KQkq
    char castling_;

    // en passe square if any
    char enpassant_[2];

    // 50-movement rule counter
    char halfCount_;

    // full counter
    char fullCount_;

    // Movement that led to this position
    // Movement movement_;
};

#endif
