#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "movement.h"

#include <iostream>
#include <vector>
#include <string>

class Variation;
typedef std::vector<Variation> Variations;

/*  
    Chessboard numbered positions:                  Algebraic positions:

    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 0  | 1  | 2  | 3  | 4  | 5  | 6  |  7 |       | a8 | b8 | c8 | d8 | e8 | f8 | g8 | h8 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 8  | 9  | 10 | 11 | 12 | 13 | 14 | 15 |       | a7 | b7 | c7 | d7 | e7 | f7 | g7 | h7 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 16 | 17 | 18 | 19 | 20 | 21 | 22 | 23 |       | a6 | b6 | c6 | d6 | e6 | f6 | g6 | h6 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 24 | 25 | 26 | 27 | 28 | 29 | 30 | 31 |       | a5 | b5 | c5 | d5 | e5 | f5 | g5 | h5 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 32 | 33 | 34 | 35 | 36 | 37 | 38 | 39 |       | a4 | b4 | c4 | d4 | e4 | f4 | g4 | h4 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 40 | 41 | 42 | 43 | 44 | 45 | 46 | 47 |       | a3 | b3 | c3 | d3 | e3 | f3 | g3 | h3 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 48 | 49 | 50 | 51 | 52 | 53 | 54 | 55 |       | a2 | b2 | c2 | d2 | e2 | f2 | g2 | h2 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+
    | 56 | 57 | 58 | 59 | 60 | 61 | 62 | 63 |       | a1 | b1 | c1 | d1 | e1 | f1 | g1 | h1 |
    +----+----+----+----+----+----+----+----+       +----+----+----+----+----+----+----+----+

*/

class Chessboard
{
public: 
        typedef char Piece;

private:
    enum class Player 
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

    constexpr static Piece WHITE_PAWN = 'P';
    constexpr static Piece WHITE_ROOK = 'R';
    constexpr static Piece WHITE_KNIGHT = 'N';
    constexpr static Piece WHITE_BISHOP = 'B';
    constexpr static Piece WHITE_KING = 'K';
    constexpr static Piece WHITE_QUEEN = 'Q';
    constexpr static Piece BLACK_PAWN = 'p';
    constexpr static Piece BLACK_ROOK = 'r';
    constexpr static Piece BLACK_KNIGHT = 'n';
    constexpr static Piece BLACK_BISHOP = 'b';
    constexpr static Piece BLACK_KING = 'k';
    constexpr static Piece BLACK_QUEEN = 'q';

    constexpr static Piece EMPTY_SQUARE = '0';

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
    void findVariations(Variations& variations) const;

    // evaluation
    float evaluation() const;

    // move
    Piece makeMove( Movement& m );
    void undoMove( /*Movement& m, Piece captured*/ );

private:
    void reset();

    void appendVariation(Variations& variations, int from, int to, Movement::Type type = Movement::Type::Normal, char promotion = 0 ) const;
    void appendPromotion(Variations& variations, int from, int to, Player player) const;
    void findPawnVariations(Variations& variations, int square) const;
    void findKnightVariations(Variations& variations, int square ) const;
    void findRookVariations(Variations& variations, int square ) const;
    void findBishopVariations(Variations& variations, int square ) const;
    void findQueenVariations(Variations& variations, int square ) const;
    void findKingVariations(Variations& variations, int square ) const;
    bool isKingThreatened(int square) const;
    int findKing() const;

    // queries about squares, coordinates
    bool isSquareOccupied( int square ) const;
    bool isSquareWhite( int square ) const;
    bool isSquareBlack( int square ) const;
    bool validCoordinates(int x, int y) const;

    int getEnPassantSquare() const;

    void switchTurn();

private:
    // 64 squares of the board starting top-left
    uint8_t data_[64];

    // past positions
    std::vector< std::string > pastPositions_;

    // BLACK_TURN or WHITE_TURN
    char turn_;

    // castling flags, only four least significant bits used: KQkq
    char castling_;

    // en passant square if any
    char enpassant_[2];

    // 50-movement rule counter
    char halfCount_;

    // full counter
    char fullCount_;
};

#endif
