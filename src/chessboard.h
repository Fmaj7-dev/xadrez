#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "movement.h"

#include <iostream>

class Variation;
typedef std::vector<Variation> Variations;

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

    constexpr static char WHITE_PAWN = 'P';
    constexpr static char WHITE_ROOK = 'R';
    constexpr static char WHITE_KNIGHT = 'N';
    constexpr static char WHITE_BISHOP = 'B';
    constexpr static char WHITE_KING = 'K';
    constexpr static char WHITE_QUEEN = 'Q';
    constexpr static char BLACK_PAWN = 'p';
    constexpr static char BLACK_ROOK = 'r';
    constexpr static char BLACK_KNIGHT = 'n';
    constexpr static char BLACK_BISHOP = 'b';
    constexpr static char BLACK_KING = 'k';
    constexpr static char BLACK_QUEEN = 'q';

    constexpr static char EMPTY_SQUARE = '0';

    constexpr static char WHITE_TURN = 'w';
    constexpr static char BLACK_TURN = 'b';

    const float PAWN_WEIGHT = 1.0f;
    const float ROOK_WEIGHT = 5.0f;
    const float KNIGHT_WEIGHT = 3.0f;
    const float BISHOP_WEIGHT = 3.0f;
    const float QUEEN_WEIGHT = 9.0f;
    const float KING_WEIGHT = 1000.0f;

public:
    Chessboard();
    const Chessboard& operator=(const Chessboard& rhs);
    
    void initDefault();

    // fem 
    void importFen(const std::string& fen);
    std::string exportFen() const;

    // variations
    void appendVariation(Variations& variations, int from, int to ) const;
    void findVariations(Variations& variations) const;
    void findPawnVariations(Variations& variations, int square) const;
    void findKnightVariations(Variations& variations, int square ) const;
    void findRookVariations(Variations& variations, int square ) const;
    void findBishopVariations(Variations& variations, int square ) const;
    void findQueenVariations(Variations& variations, int square ) const;

    // move
    void applyMovement( Movement& m );

    // queries about squares, coordinates
    bool isSquareOccupied( int square ) const;
    bool isSquareWhite( int square ) const;
    bool isSquareBlack( int square ) const;
    bool validCoordinates(int x, int y) const;

    // evaluation
    float evaluation() const;

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
};

#endif
