#include <iostream>

class Chessboard
{
    enum class CASTLING : uint8_t
    {
        WHITE_KING  = 1 << 0,
        WHITE_QUEEN = 1 << 1,
        BLACK_KING  = 1 << 2,
        BLACK_QUEEN = 1 << 3
    };

public:
    Chessboard();
    
    void initDefault();

    void importFem(const std::string& fem);
    std::string exportFem() const;

    void print() const;

private:
    // 64 squares of the board starting top-left
    char data_[64];

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