#include "chessboard.h"

#include <boost/algorithm/string.hpp>

Chessboard::Chessboard()
{
    enpassant_[0] = 0;
    enpassant_[1] = 0;
}

    
void Chessboard::initDefault()
{
    importFem("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

bool isValidNumber(char c)
{
    return c > '0' && c < '9';
}

bool isValidPiece(char c)
{
    return c == 'r' || c == 'R' ||
           c == 'n' || c == 'N' ||
           c == 'b' || c == 'B' ||
           c == 'q' || c == 'Q' ||
           c == 'k' || c == 'K' ||
           c == 'p' || c == 'P';
}

bool isNewLine(char c)
{
    return c == '/';
}

bool isSpace(char c)
{
    return c == ' ';
}

void Chessboard::importFem(const std::string& fem)
{
    std::vector<std::string> fem_strings;
    boost::split(fem_strings, fem, boost::is_any_of("\t "));

// 1. Import pieces
    // the place of the array to be filled next
    int insertPosition = 0;

    // i = the position of the string we are working on with
    for ( size_t i = 0; i < fem_strings[0].length(); ++i )
    {
        char c = fem_strings[0][i];

        if ( isValidNumber(c) )
            for (int n = 0; n < c-'0'; ++n)
                data_[insertPosition++] = '0';

        else if ( isValidPiece(c) )
            data_[insertPosition++] = c;

        else if ( isNewLine(c) )
        {
            // no need to proccess it.
        }
    }

// 2. import active colour
    turn_ = fem_strings[1][0];

// 3. import castling
    for ( size_t i = 0; i < fem_strings[2].length(); ++i )
    {
        if (fem_strings[2][i] == 'K')
            castling_ |= (char) CASTLING::WHITE_KING;

        if (fem_strings[2][i] == 'Q')
            castling_ |= (char) CASTLING::WHITE_QUEEN;

        if (fem_strings[2][i] == 'k')
            castling_ |= (char) CASTLING::BLACK_KING;

        if (fem_strings[2][i] == 'q')
            castling_ |= (char) CASTLING::BLACK_QUEEN;            
    }

// 4. import en passant
    enpassant_[0]=fem_strings[3][0];
    enpassant_[1]=fem_strings[3][1];

// 5. import halfmove (movement count for the 50-move rule)
    halfCount_ = (char)std::stoi( fem_strings[4] );
// 6. import fullmove (total # of movements)
    fullCount_ = (char)std::stoi( fem_strings[5] );
}

void Chessboard::print() const
{
    std::cout<<"DATA: \n";
    for (size_t i = 0; i < 64; ++i)
        std::cout<<data_[i]<<" ";

    std::cout<<std::endl;
}

std::string Chessboard::exportFem() const
{
    std::string fem;

// 1. export pieces
    int sequencyEmpty = 0;

    for (int i = 0; i < 64; ++i )
    {
        // end of rank, insert '/'
        if (i!=0 && i%8 == 0)
        {
            if (sequencyEmpty != 0)
            {
                fem.push_back(sequencyEmpty + '0');
                sequencyEmpty = 0;
            }
            fem.push_back('/');
        }

        // accumulate empty squares...
        if (data_[i] == '0')
            ++sequencyEmpty;
        // ...until a new piece is found, then write the number (if any)
        else
        {
            if (sequencyEmpty != 0)
            {
                fem.push_back(sequencyEmpty + '0');
                sequencyEmpty = 0;
            }
            fem.push_back(data_[i]);
        }
    }

// 2. export active colour
    fem.push_back(' ');
    fem.push_back(turn_);

// 3. export castling
    fem.push_back(' ');
    if ( castling_ == 0 )
        fem.push_back('-');
    else
    {
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_KING) )
            fem.push_back('K');
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_QUEEN) )
            fem.push_back('Q');
        if ( castling_ & static_cast<uint8_t>(CASTLING::BLACK_KING) )
            fem.push_back('k');
        if ( castling_ & static_cast<uint8_t>(CASTLING::BLACK_QUEEN) )
            fem.push_back('q');
    }

// 4. export en passant
    fem.push_back(' ');

    if ( enpassant_[0] == '-' )
        fem.push_back('-');
    else
    {
        fem.push_back(enpassant_[0]);
        fem.push_back(enpassant_[1]);
    }

// 5. export halfmove (movement count for the 50-move rule)
    fem.push_back(' ');
    fem += std::to_string((int)halfCount_ );
// 6. export fullmove (total # of movements)
    fem.push_back(' ');
    fem += std::to_string((int)fullCount_);

    return fem;
}

float Chessboard::evaluation()
{
    float evaluation = 0;

    for (int i = 0; i < 64; ++i )
    {
        if ( data_[i] == WHITE_PAWN )
            evaluation += PAWN_WEIGHT;
        if ( data_[i] == WHITE_ROOK )
            evaluation += ROOK_WEIGHT;
        if ( data_[i] == WHITE_KNIGHT )
            evaluation += KNIGHT_WEIGHT;
        if ( data_[i] == WHITE_BISHOP )
            evaluation += BISHOP_WEIGHT;
        if ( data_[i] == WHITE_QUEEN )
            evaluation += QUEEN_WEIGHT;
        if ( data_[i] == WHITE_KING )
            evaluation += KING_WEIGHT;

        if ( data_[i] == BLACK_PAWN )
            evaluation -= PAWN_WEIGHT;
        if ( data_[i] == BLACK_ROOK )
            evaluation -= ROOK_WEIGHT;
        if ( data_[i] == BLACK_KNIGHT )
            evaluation -= KNIGHT_WEIGHT;
        if ( data_[i] == BLACK_BISHOP )
            evaluation -= BISHOP_WEIGHT;
        if ( data_[i] == BLACK_QUEEN )
            evaluation -= QUEEN_WEIGHT;
        if ( data_[i] == BLACK_KING )
            evaluation -= KING_WEIGHT;    
    }

    return evaluation;
}
/*
Chessboard::ChessboardArray Chessboard::findVariations()
{
    ChessboardArray array;

    for (int i = 0; i < 64; ++i )
    {
        if ( data_[i] == WHITE_PAWN )
            findPawnVariations(i);
        if ( data_[i] == WHITE_ROOK )
            evaluation += ROOK_WEIGHT;
        if ( data_[i] == WHITE_KNIGHT )
            evaluation += KNIGHT_WEIGHT;
        if ( data_[i] == WHITE_BISHOP )
            evaluation += BISHOP_WEIGHT;
        if ( data_[i] == WHITE_QUEEN )
            evaluation += QUEEN_WEIGHT;
        if ( data_[i] == WHITE_KING )
            evaluation += KING_WEIGHT;

        if ( data_[i] == BLACK_PAWN )
            evaluation -= PAWN_WEIGHT;
        if ( data_[i] == BLACK_ROOK )
            evaluation -= ROOK_WEIGHT;
        if ( data_[i] == BLACK_KNIGHT )
            evaluation -= KNIGHT_WEIGHT;
        if ( data_[i] == BLACK_BISHOP )
            evaluation -= BISHOP_WEIGHT;
        if ( data_[i] == BLACK_QUEEN )
            evaluation -= QUEEN_WEIGHT;
        if ( data_[i] == BLACK_KING )
            evaluation -= KING_WEIGHT;    
    }

    return array;
}

ChessboardArray Chessboard::findPawnVariations(int square, PLAYER player)
{
    if ( player == PLAYER::WHITE )
    {
        // if first movement of the pawn
        if( square >= 48 && square <= 55 )
        {
            if ( !isSquareOccupied(square - 8) )

        }
    }
}


bool Chessboard::isSquareOccupied( int square )
{

}

bool Chessboard::isSquareWhite( int square )
{

}

bool Chessboard::isSquareBlack( int square )
{

}

*/