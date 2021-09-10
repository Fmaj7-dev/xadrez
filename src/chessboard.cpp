#include "chessboard.h"
#include "variation.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

Chessboard::Chessboard()
{
    enpassant_[0] = 0;
    enpassant_[1] = 0;
}

const Chessboard& Chessboard::operator=(const Chessboard& rhs)
{
    std::copy(std::begin(rhs.data_), std::end(rhs.data_), std::begin(data_));
    turn_ = rhs.turn_;
    castling_ = rhs.castling_;
    std::copy(std::begin(rhs.enpassant_), std::end(rhs.enpassant_), std::begin(enpassant_));
    halfCount_ = rhs.halfCount_;
    fullCount_ = rhs.fullCount_;

    return *this;
}

    
void Chessboard::initDefault()
{
    importFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
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

void Chessboard::importFen(const std::string& fen)
{
    std::vector<std::string> fen_strings;
    boost::split(fen_strings, fen, boost::is_any_of("\t "));

// 1. Import pieces
    // the place of the array to be filled next
    int insertPosition = 0;

    // i = the position of the string we are working on with
    for ( size_t i = 0; i < fen_strings[0].length(); ++i )
    {
        char c = fen_strings[0][i];

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
    turn_ = fen_strings[1][0];

// 3. import castling
    for ( size_t i = 0; i < fen_strings[2].length(); ++i )
    {
        if (fen_strings[2][i] == 'K')
            castling_ |= (char) CASTLING::WHITE_KING;

        if (fen_strings[2][i] == 'Q')
            castling_ |= (char) CASTLING::WHITE_QUEEN;

        if (fen_strings[2][i] == 'k')
            castling_ |= (char) CASTLING::BLACK_KING;

        if (fen_strings[2][i] == 'q')
            castling_ |= (char) CASTLING::BLACK_QUEEN;            
    }

// 4. import en passant
    enpassant_[0]=fen_strings[3][0];
    enpassant_[1]=fen_strings[3][1];

// 5. import halfmove (movement count for the 50-move rule)
    halfCount_ = (char)std::stoi( fen_strings[4] );
// 6. import fullmove (total # of movements)
    fullCount_ = (char)std::stoi( fen_strings[5] );
}

void Chessboard::print() const
{
    std::cout<<"DATA: \n";
    for (size_t i = 0; i < 64; ++i)
        std::cout<<data_[i]<<" ";

    std::cout<<std::endl;
}

std::string Chessboard::exportFen() const
{
    std::string fen;

// 1. export pieces
    int sequencyEmpty = 0;

    for (int i = 0; i < 64; ++i )
    {
        // end of rank, insert '/'
        if (i!=0 && i%8 == 0)
        {
            if (sequencyEmpty != 0)
            {
                fen.push_back(sequencyEmpty + '0');
                sequencyEmpty = 0;
            }
            fen.push_back('/');
        }

        // accumulate empty squares...
        if (data_[i] == '0')
            ++sequencyEmpty;
        // ...until a new piece is found, then write the number (if any)
        else
        {
            if (sequencyEmpty != 0)
            {
                fen.push_back(sequencyEmpty + '0');
                sequencyEmpty = 0;
            }
            fen.push_back(data_[i]);
        }
    }

// 2. export active colour
    fen.push_back(' ');
    fen.push_back(turn_);

// 3. export castling
    fen.push_back(' ');
    if ( castling_ == 0 )
        fen.push_back('-');
    else
    {
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_KING) )
            fen.push_back('K');
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_QUEEN) )
            fen.push_back('Q');
        if ( castling_ & static_cast<uint8_t>(CASTLING::BLACK_KING) )
            fen.push_back('k');
        if ( castling_ & static_cast<uint8_t>(CASTLING::BLACK_QUEEN) )
            fen.push_back('q');
    }

// 4. export en passant
    fen.push_back(' ');

    if ( enpassant_[0] == '-' )
        fen.push_back('-');
    else
    {
        fen.push_back(enpassant_[0]);
        fen.push_back(enpassant_[1]);
    }

// 5. export halfmove (movement count for the 50-move rule)
    fen.push_back(' ');
    fen += std::to_string((int)halfCount_ );
// 6. export fullmove (total # of movements)
    fen.push_back(' ');
    fen += std::to_string((int)fullCount_);

    return fen;
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

void Chessboard::appendVariation(Variations& variations, int from, int to )
{
    Movement movement(from, to);

    Variation variation;
    variation.chessboard_ = *this;
    variation.movement_ = movement;

    if (turn_ == 'b')
        variation.chessboard_.turn_ = 'w';
    if (turn_ == 'w')
        variation.chessboard_.turn_ = 'b';

    variation.chessboard_.applyMovement( movement );

    variations.push_back( variation );
}

void Chessboard::applyMovement( Movement& m )
{
    data_[m.to().getValue()] = data_[m.from().getValue()];
    data_[m.from().getValue()] = '0';
}

void Chessboard::findVariations( Variations& variations )
{
    for (int i = 0; i < 64; ++i )
    {
        if ( turn_ == 'w' )
        {
            if ( data_[i] == WHITE_PAWN )
                findPawnVariations(variations, i);
            /*if ( data_[i] == WHITE_ROOK )
                evaluation += ROOK_WEIGHT;*/
            if ( data_[i] == WHITE_KNIGHT )
                findKnightVariations(variations, i);
            /*if ( data_[i] == WHITE_BISHOP )
                evaluation += BISHOP_WEIGHT;
            if ( data_[i] == WHITE_QUEEN )
                evaluation += QUEEN_WEIGHT;
            if ( data_[i] == WHITE_KING )
                evaluation += KING_WEIGHT;*/
        }

        if ( turn_ == 'b' )
        {
            if ( data_[i] == BLACK_PAWN )
                findPawnVariations(variations, i);
            /*if ( data_[i] == BLACK_ROOK )
                evaluation -= ROOK_WEIGHT;*/
            if ( data_[i] == BLACK_KNIGHT )
                findKnightVariations(variations, i);
            /*if ( data_[i] == BLACK_BISHOP )
                evaluation -= BISHOP_WEIGHT;
            if ( data_[i] == BLACK_QUEEN )
                evaluation -= QUEEN_WEIGHT;
            if ( data_[i] == BLACK_KING )
                evaluation -= KING_WEIGHT;*/
        }
    }
}

void Chessboard::findPawnVariations(Variations& variations, int square)
{
    if ( turn_ == 'w' )
    {
        // if first movement of the pawn
        if( square >= 48 && square <= 55 )
        {
            if ( !isSquareOccupied(square - 16) )
                appendVariation(variations, square, square - 16);
        }
        if ( !isSquareOccupied(square - 8) )
                appendVariation(variations, square, square - 8);
    }
    else if (turn_ == 'b')
    {
        // if first movement of the pawn
        if( square >= 8 && square <= 15 )
        {
            if ( !isSquareOccupied(square + 16) )
                appendVariation(variations, square, square + 16);
        }
        if ( !isSquareOccupied(square + 8) )
                appendVariation(variations, square, square + 8);
    }
}

void Chessboard::findKnightVariations(Variations& variations, int square )
{
    
}


bool Chessboard::isSquareOccupied( int square )
{
    return data_[square] != '0';
}

/*bool Chessboard::isSquareWhite( int square )
{

}

bool Chessboard::isSquareBlack( int square )
{

}*/