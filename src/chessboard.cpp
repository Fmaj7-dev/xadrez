#include "chessboard.h"
#include "variation.h"
#include "util/log.h"
#include "util/ettune.h"

#include <algorithm>
#include <boost/algorithm/string.hpp>

Chessboard::Chessboard()
{MEASURE
}

const Chessboard& Chessboard::operator=(const Chessboard& rhs)
{MEASURE
    std::copy(std::begin(rhs.data_), std::end(rhs.data_), std::begin(data_));
    turn_ = rhs.turn_;
    castling_ = rhs.castling_;
    std::copy(std::begin(rhs.enpassant_), std::end(rhs.enpassant_), std::begin(enpassant_));
    halfCount_ = rhs.halfCount_;
    fullCount_ = rhs.fullCount_;

    return *this;
}

void Chessboard::initDefault()
{MEASURE
    importFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

bool isValidNumber(char c)
{MEASURE
    return c > '0' && c < '9';
}

bool isValidPiece(char c)
{MEASURE
    return c == 'r' || c == 'R' ||
           c == 'n' || c == 'N' ||
           c == 'b' || c == 'B' ||
           c == 'q' || c == 'Q' ||
           c == 'k' || c == 'K' ||
           c == 'p' || c == 'P';
}

bool isNewLine(char c)
{MEASURE
    return c == '/';
}

bool isSpace(char c)
{MEASURE
    return c == ' ';
}

void Chessboard::reset()
{MEASURE
    std::fill(std::begin(data_), std::end(data_), 0);
    turn_ = 0;
    castling_ = 0;
    enpassant_[0] = 0;
    enpassant_[1] = 0;
    halfCount_ = 0;
    fullCount_ = 0;
}

void Chessboard::importFen(const std::string& fen)
{MEASURE
    reset();

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
                data_[insertPosition++] = EMPTY_SQUARE;

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

std::string Chessboard::exportFen() const
{MEASURE
    std::string fen;

    // 1. export pieces
    int sequencyEmpty = 0;

    for ( int i = 0; i < 64; ++i )
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
        if (data_[i] == EMPTY_SQUARE)
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

    // remaining empty squares
    if (sequencyEmpty != 0)
        fen.push_back(sequencyEmpty + '0');

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

void Chessboard::prettyPrint() const
{MEASURE
    std::cout << "  a b c d e f g h" << std::endl;

    for ( int i = 0; i < 8; ++i )
    {
        std::cout << 8-i << " ";
        for ( int j = 0; j < 8; ++j )
        {
            std::cout << data_[i*8+j] << " ";
        }
        std::cout << std::endl;
    }
}

float Chessboard::evaluation() const
{MEASURE
    float evaluation = 0;

    for (int i = 0; i < 64; ++i )
    {
        switch( data_[i] )
        {
            case WHITE_PAWN:    evaluation += PAWN_WEIGHT;      break;
            case WHITE_ROOK:    evaluation += ROOK_WEIGHT;      break;
            case WHITE_KNIGHT:  evaluation += KNIGHT_WEIGHT;    break;
            case WHITE_BISHOP:  evaluation += BISHOP_WEIGHT;    break;
            case WHITE_QUEEN:   evaluation += QUEEN_WEIGHT;     break;
            case WHITE_KING:    evaluation += KING_WEIGHT;      break;

            case BLACK_PAWN:    evaluation -= PAWN_WEIGHT;      break;
            case BLACK_ROOK:    evaluation -= ROOK_WEIGHT;      break;
            case BLACK_KNIGHT:  evaluation -= KNIGHT_WEIGHT;    break;
            case BLACK_BISHOP:  evaluation -= BISHOP_WEIGHT;    break;
            case BLACK_QUEEN:   evaluation -= QUEEN_WEIGHT;     break;
            case BLACK_KING:    evaluation -= KING_WEIGHT;      break;
        }
    }

    return evaluation;
}

void Chessboard::appendPromotion(Variations& variations, int from, int to, Player player) const
{MEASURE
    if ( player == Player::WHITE )
    {
        appendVariation( variations, from, to, Movement::Type::Promotion, WHITE_QUEEN );
        appendVariation( variations, from, to, Movement::Type::Promotion, WHITE_ROOK );
        appendVariation( variations, from, to, Movement::Type::Promotion, WHITE_BISHOP );
        appendVariation( variations, from, to, Movement::Type::Promotion, WHITE_KNIGHT );
    }
    else
    {
        appendVariation( variations, from, to, Movement::Type::Promotion, BLACK_QUEEN );
        appendVariation( variations, from, to, Movement::Type::Promotion, BLACK_ROOK );
        appendVariation( variations, from, to, Movement::Type::Promotion, BLACK_BISHOP );
        appendVariation( variations, from, to, Movement::Type::Promotion, BLACK_KNIGHT );
    }
}

void Chessboard::appendVariation(Variations& variations, int from, int to, Movement::Type type, char promotion) const
{MEASURE
    Movement movement(from, to, type, promotion);

    Variation variation;
    
    variation.movement_ = movement;

#if (UNDO_FEN_STRING)
    Chessboard vchessboard_ = *this;
    vchessboard_.makeMove( movement );

    // undo the switch makeMove did, so that we can really test if the king is threatened
    vchessboard_.switchTurn();

    if (!vchessboard_.isInCheck())
        variations.push_back( variation );
#else
    variation.chessboard_ = *this;
    variation.chessboard_.makeMove( movement );
    variation.chessboard_.switchTurn();
    if (!variation.chessboard_.isInCheck())
        variations.push_back( variation );
#endif




    /*int kingPosition = vchessboard_.findKing();

    // only append it if the king is not threatened
    if ( !vchessboard_.isKingThreatened( kingPosition ) )
    {
        variations.push_back( variation );
    }*/
}

bool Chessboard::isInCheck() const
{MEASURE
    int kingPosition = findKing();

    return isPieceThreatened( kingPosition );
}

int Chessboard::findKing() const
{MEASURE
    if ( turn_ == BLACK_TURN )
        for (int i = 0; i < 64; ++i)
        {
            if ( data_[i] == BLACK_KING )
                return i;
        }

    else //if ( turn_ == WHITE_TURN )
        for (int i = 0; i < 64; ++i)
        {
            if ( data_[i] == WHITE_KING )
                return i;
        }
    
    etlog("Error: king not found");
    return 0;
}

bool Chessboard::isPieceThreatened(int square) const
{MEASURE
    int x_coord = square % 8;
    int y_coord = square / 8;

    // threaten by knight?
    int x[8];
    int y[8];

    // position 1
    x[0] = x_coord-1;
    y[0] = y_coord-2;

    // position 2
    x[1] = x_coord+1;
    y[1] = y_coord-2;

    // position 3
    x[2] = x_coord-2;
    y[2] = y_coord-1;

    // position 4
    x[3] = x_coord+2;
    y[3] = y_coord-1;

    // position 5
    x[4] = x_coord-2;
    y[4] = y_coord+1;

    // position 6
    x[5] = x_coord+2;
    y[5] = y_coord+1;

    // position 7
    x[6] = x_coord-1;
    y[6] = y_coord+2;

    // position 8
    x[7] = x_coord+1;
    y[7] = y_coord+2;

    if ( turn_ == WHITE_TURN )
    {
        for (int i = 0; i < 8; ++i)
        {
            if( validCoordinates(x[i], y[i]) && data_[x[i] + y[i]*8] == BLACK_KNIGHT )
                return true;
        }

    }
    else if (turn_ == BLACK_TURN)
    {
        for (int i = 0; i < 8; ++i)
            if( validCoordinates(x[i], y[i]) && data_[x[i] + y[i]*8] == WHITE_KNIGHT )
                return true;
    }

    // threatened by rook or queen
    int x_target;
    int y_target;

    auto traverse = [&](auto lambda)
    {
        while ( validCoordinates(x_target, y_target) )
        {
            int target_square = y_target*8 + x_target;

            if ( !isSquareOccupied( target_square ) )
                lambda();
            else
            {
                if ( turn_ == WHITE_TURN )
                {
                    if ( data_[target_square] == BLACK_ROOK ||  data_[target_square] == BLACK_QUEEN)
                        return true;
                }
                else if ( turn_ == BLACK_TURN )
                {
                    if ( data_[target_square] == WHITE_ROOK ||  data_[target_square] == WHITE_QUEEN)
                        return true;
                }
                return false;
            }
        }
        return false;
    };

    auto move_up =      [&] () {--y_target;};
    auto move_down =    [&] () {++y_target;};
    auto move_right =   [&] () {++x_target;};
    auto move_left =    [&] () {--x_target;};

    // up
    x_target = x_coord;
    y_target = y_coord-1;
    if (traverse(move_up) )
        return true;

    // down
    x_target = x_coord;
    y_target = y_coord+1;
    if (traverse(move_down) )
        return true;

    // right
    x_target = x_coord+1;
    y_target = y_coord;
    if (traverse(move_right) )
        return true;

    // left
    x_target = x_coord-1;
    y_target = y_coord;
    if (traverse(move_left) )
        return true;
    
    // threatened by bishop or queen
    auto traverseBishopQueen = [&](auto lambda)
    {
        while ( validCoordinates(x_target, y_target) )
        {
            int target_square = y_target*8 + x_target;

            if ( !isSquareOccupied( target_square ) )
                lambda();
            else
            {
                if ( turn_ == WHITE_TURN )
                {
                    if ( data_[target_square] == BLACK_BISHOP ||  data_[target_square] == BLACK_QUEEN)
                        return true;
                }
                else if ( turn_ == BLACK_TURN )
                {
                    if ( data_[target_square] == WHITE_BISHOP ||  data_[target_square] == WHITE_QUEEN)
                        return true;
                }
                break;
            }
        }
        return false;
    };

    auto move_up_left    = [&] () {--y_target; --x_target;};
    auto move_up_right   = [&] () {--y_target; ++x_target;};
    auto move_down_left  = [&] () {++y_target; --x_target;};
    auto move_down_right = [&] () {++y_target; ++x_target;};

    // up left
    x_target = x_coord-1;
    y_target = y_coord-1;
    if ( traverseBishopQueen(move_up_left) )
        return true;

    // up right
    x_target = x_coord+1;
    y_target = y_coord-1;
    if ( traverseBishopQueen(move_up_right) )
        return true;

    // down left
    x_target = x_coord-1;
    y_target = y_coord+1;
    if ( traverseBishopQueen(move_down_left) )
        return true;

    // down left
    x_target = x_coord+1;
    y_target = y_coord+1;
    if ( traverseBishopQueen(move_down_right) )
        return true;

    // threatened by pawn FIXME
    if ( turn_ == WHITE_TURN )
    {
        if ( validCoordinates(x_coord-1, y_coord-1) && data_[x_coord-1 + (y_coord-1)*8] == BLACK_PAWN )
            return true;
        if ( validCoordinates(x_coord+1, y_coord-1) && data_[x_coord+1 + (y_coord-1)*8] == BLACK_PAWN )
            return true;
    }
    else if ( turn_ == BLACK_TURN )
    {
        if ( validCoordinates(x_coord-1, y_coord+1) && data_[x_coord-1 + (y_coord+1)*8] == WHITE_PAWN )
            return true;
        if ( validCoordinates(x_coord+1, y_coord+1) && data_[x_coord+1 + (y_coord+1)*8] == WHITE_PAWN )
            return true;
    }

    return false;
}

Chessboard::Piece Chessboard::makeMove( Movement& m )
{MEASURE
#if (UNDO_FEN_STRING)
    pastPositions_.push_back( exportFen() );
#else
    pastPositions_.push_back( *this );  
#endif

    Piece piece = EMPTY_SQUARE;

    if ( m.type() == Movement::Type::EnPassantCapture )
    {
        // return captured piece or EMPTY_SQUARE if movement normal
        //piece = data_[m.to().getValue()];
        if ( turn_ == WHITE_TURN )
        {
            piece = data_[m.to().getValue()+8];
            data_[m.to().getValue()+8] = EMPTY_SQUARE;
        }
        else if ( turn_ == BLACK_TURN )
        {
            piece = data_[m.to().getValue()-8];
            data_[m.to().getValue()-8] = EMPTY_SQUARE;
        }
        data_[ m.to().getValue() ] = data_[ m.from().getValue() ];
    }

    // Normal move: piece move, piece capture, advanced pawn
    else if ( m.type() == Movement::Type::Normal )
    {
        // return captured piece or EMPTY_SQUARE if movement normal
        piece = data_[m.to().getValue()];
        data_[ m.to().getValue() ] = data_[ m.from().getValue() ];
    }

    // pawn promotion
    else if ( m.type() == Movement::Type::Promotion )
    {
        // return captured piece or EMPTY_SQUARE if movement promotion
        piece = data_[m.to().getValue()];
        data_[ m.to().getValue() ] = m.info();
    }

    // initial pawn move of two squares
    else if ( m.type() == Movement::Type::DoublePawnStep )
    {
        data_[ m.to().getValue() ] = data_[ m.from().getValue() ];
        std::string to = m.to().getStr();

        // set en passant values, if pawn moved two squares
        enpassant_[0] = to[0];
        if ( turn_ == BLACK_TURN )
            enpassant_[1] = to[1] + 1;
        if ( turn_ == WHITE_TURN )
            enpassant_[1] = to[1] - 1;
    }

    // reset en passant values, if not double move of pawn
    if ( m.type() != Movement::Type::DoublePawnStep )
    {
        enpassant_[0] = '-';
        enpassant_[1] = 0;
    }

    data_[m.from().getValue()] = EMPTY_SQUARE;

    switchTurn();
    
    ++fullCount_;
    return piece;
}

void Chessboard::switchTurn()
{MEASURE
    if (turn_ == BLACK_TURN)
        turn_ = WHITE_TURN;
    else if (turn_ == WHITE_TURN)
        turn_ = BLACK_TURN;
}

/**
 * It is not easy to keep track of past en passant captures, cause we have
 * to restore the enpassant_ values, and if two pawns advance two squares in a row
 * the first one is lost. So an array would be needed...
 * I decided to use a vector.
 */
void Chessboard::undoMove( /*Movement& m, Piece piece*/ )
{MEASURE
#if (UNDO_FEN_STRING)
    importFen( pastPositions_.back() );
#else
    this->operator=( pastPositions_.back() );
#endif
    pastPositions_.pop_back();

    /*if ( m.type() == Movement::Type::Normal )
    {
        data_[m.from().getValue()] = data_[m.to().getValue()];
        data_[m.to().getValue()] = piece;
    }
    else if ( m.type() == Movement::Type::Promotion )
    {
        data_[m.from().getValue()];
        data_[m.to().getValue()] = piece;
    }

    if ( m.type() == Movement::Type::DoublePawnStep )
    {
        std::string to = m.to().getStr();

        enpassant_[0] = to[0];

        if ( turn_ == BLACK_TURN )
            enpassant_[1] = to[1] + 1;
        if ( turn_ == WHITE_TURN )
            enpassant_[1] = to[1] - 1;

        //std::cout<<"adding enpassant "<<enpassant_<<std::endl;
    }

    if ( m.type() != Movement::Type::DoublePawnStep )
    {
        //std::cout<<"removing enpassant "<<std::endl;
        enpassant_[0] = '-';
        enpassant_[1] = 0;
    }

    --fullCount_;*/
}

void Chessboard::findVariations( Variations& variations ) const
{MEASURE
    for (int i = 0; i < 64; ++i )
    {
        if ( turn_ == WHITE_TURN )
        {
            if ( data_[i] == WHITE_PAWN )
                findPawnVariations(variations, i);
            if ( data_[i] == WHITE_ROOK )
                findRookVariations(variations, i);
            if ( data_[i] == WHITE_KNIGHT )
                findKnightVariations(variations, i);
            if ( data_[i] == WHITE_BISHOP )
                findBishopVariations(variations, i);
            if ( data_[i] == WHITE_QUEEN )
                findQueenVariations(variations, i);
            if ( data_[i] == WHITE_KING )
                findKingVariations(variations, i);
        }

        if ( turn_ == BLACK_TURN )
        {
            if ( data_[i] == BLACK_PAWN )
                findPawnVariations(variations, i);
            else if ( data_[i] == BLACK_ROOK )
                findRookVariations(variations, i);
            else if ( data_[i] == BLACK_KNIGHT )
                findKnightVariations(variations, i);
            else if ( data_[i] == BLACK_BISHOP )
                findBishopVariations(variations, i);
            else if ( data_[i] == BLACK_QUEEN )
                findQueenVariations(variations, i);
            else if ( data_[i] == BLACK_KING )
                findKingVariations(variations, i);
        }
    }
}

int Chessboard::getEnPassantSquare() const
{MEASURE
    return Position::char2int(enpassant_);
}

void Chessboard::findPawnVariations(Variations& variations, int square) const
{MEASURE
    int x_coord = square % 8;
    int y_coord = square / 8;

    if ( turn_ == WHITE_TURN )
    {
        // if enpassant is an option
        if ( enpassant_[0] != '-' )
        {
            int enpassant_square = Position::char2int(enpassant_);
            if ( (x_coord > 0 && square - 9 == enpassant_square) || 
                 (x_coord < 7 && square - 7 == enpassant_square) )
            {
                appendVariation(variations, square, enpassant_square, Movement::Type::EnPassantCapture);
            }
        }

        // is it on a promoting rank? 
        bool promotingRank = false;
        if (square >= 8 && square <= 15)
            promotingRank = true;

        // default movement: one square
        if ( !isSquareOccupied(square - 8) && !promotingRank )
            appendVariation(variations, square, square - 8);

        // if first movement of the pawn, it could also move two squares
        if( square >= 48 && square <= 55 )
        {
            if ( !isSquareOccupied(square - 16) && !isSquareOccupied(square -8 ) )
                appendVariation(variations, square, square - 16, Movement::Type::DoublePawnStep);
        }

        // capture left
        if (x_coord != 0)
            if ( isSquareBlack(square-9) )
            {
                if (!promotingRank)
                    appendVariation(variations, square, square - 9);
                else
                    appendPromotion(variations, square, square - 9, Player::WHITE);
            }
        
        // capture right
        if (x_coord != 7)
            if ( isSquareBlack(square-7) )
            {
                if (!promotingRank)
                    appendVariation(variations, square, square - 7);
                else
                    appendPromotion(variations, square, square - 7, Player::WHITE);
            }

        // last step to final rank
        if ( promotingRank )
            if ( !isSquareOccupied(square - 8))
                appendPromotion(variations, square, square - 8, Player::WHITE);
    }
    else if (turn_ == BLACK_TURN)
    {
        // if enpassant is an option
        if ( enpassant_[0] != '-' )
        {
            int enpassant_square = Position::char2int(enpassant_);
            if ( (x_coord < 7 && square + 9 == enpassant_square ) || 
                 (x_coord > 0 && square + 7 == enpassant_square) )
                 {
                    appendVariation(variations, square, enpassant_square, Movement::Type::EnPassantCapture);
                 }
        }

        // is it on a promoting rank?
        bool promotingRank = false;
        if (square >= 48 && square <= 55)
            promotingRank = true;

        // default movement: one square
        if ( !isSquareOccupied(square + 8) && !promotingRank )
            appendVariation(variations, square, square + 8);

        // if first movement of the pawn, it could also move two squares
        if( square >= 8 && square <= 15 )
        {
            if ( !isSquareOccupied(square + 16) && !isSquareOccupied(square + 8))
                appendVariation(variations, square, square + 16, Movement::Type::DoublePawnStep);
        }
        
        // capture left
        if(x_coord != 7)
            if ( isSquareWhite(square+9) )
            {
                if (!promotingRank)
                    appendVariation(variations, square, square + 9);
                else
                    appendPromotion(variations, square, square + 9, Player::BLACK);
            }

        // capture right
        if (x_coord != 0)
            if ( isSquareWhite(square+7) )
            {
                if (!promotingRank)
                    appendVariation(variations, square, square + 7);
                else
                    appendPromotion(variations, square, square + 7, Player::BLACK);
            }

        // last step to final rank
        if ( promotingRank )
            if ( !isSquareOccupied(square + 8))
                appendPromotion(variations, square, square + 8, Player::BLACK);
    }
}

bool Chessboard::validCoordinates(int x, int y) const
{MEASURE
    if ( x < 0 || x > 7 || y < 0 || y > 7 )
        return false;

    return true;
}

void Chessboard::findKnightVariations(Variations& variations, int square ) const
{MEASURE
    int x_coord = square % 8;
    int y_coord = square / 8;

    int x[8];
    int y[8];

    // position 1
    x[0] = x_coord-1;
    y[0] = y_coord-2;

    // position 2
    x[1] = x_coord+1;
    y[1] = y_coord-2;

    // position 3
    x[2] = x_coord-2;
    y[2] = y_coord-1;

    // position 4
    x[3] = x_coord+2;
    y[3] = y_coord-1;

    // position 5
    x[4] = x_coord-2;
    y[4] = y_coord+1;

    // position 6
    x[5] = x_coord+2;
    y[5] = y_coord+1;

    // position 7
    x[6] = x_coord-1;
    y[6] = y_coord+2;

    // position 8
    x[7] = x_coord+1;
    y[7] = y_coord+2;

    if ( turn_ == WHITE_TURN )
    {
        for (int i = 0; i < 8; ++i)
        {
            int dest_square = x[i] + y[i]*8;
            //if( validCoordinates(x[i], y[i]) && (!isSquareOccupied(x[i] + y[i]*8) || isSquareBlack(x[i] + y[i]*8)) )
            if( validCoordinates(x[i], y[i]) && !isSquareWhite(dest_square) )
                appendVariation(variations, square, dest_square);
        }

    }
    else if (turn_ == BLACK_TURN)
    {
        for (int i = 0; i < 8; ++i)
        {
            int dest_square = x[i] + y[i]*8;
            //if( validCoordinates(x[i], y[i]) && (!isSquareOccupied(x[i] + y[i]*8) || isSquareWhite(x[i] + y[i]*8)) )
            if( validCoordinates(x[i], y[i]) && !isSquareBlack(dest_square) )
                appendVariation(variations, square, dest_square);
        }
    }
}

void Chessboard::findRookVariations(Variations& variations, int square ) const
{MEASURE
    int x_coord = square % 8;
    int y_coord = square / 8;

    int x_target;
    int y_target;

    auto traverse = [&](auto lambda)
    {
        while ( validCoordinates(x_target, y_target) )
        {
            int target_square = y_target*8 + x_target;

            if ( !isSquareOccupied( target_square ) )
            {
                appendVariation( variations, square, target_square );
                lambda();
            }
            else
            {
                if ( turn_ == WHITE_TURN )
                {
                    if ( isSquareBlack( target_square ) )
                        appendVariation( variations, square,target_square );
                }
                else if ( turn_ == BLACK_TURN )
                {
                    if ( isSquareWhite( target_square ) )
                        appendVariation( variations, square, target_square );
                }
                break;
            }
        }
    };
    
    auto move_up =      [&] () {--y_target;};
    auto move_down =    [&] () {++y_target;};
    auto move_right =   [&] () {++x_target;};
    auto move_left =    [&] () {--x_target;};

    // up
    x_target = x_coord;
    y_target = y_coord-1;
    traverse(move_up);

    // down
    x_target = x_coord;
    y_target = y_coord+1;
    traverse(move_down);

    // right
    x_target = x_coord+1;
    y_target = y_coord;
    traverse(move_right);

    // left
    x_target = x_coord-1;
    y_target = y_coord;
    traverse(move_left);
}

void Chessboard::findBishopVariations(Variations& variations, int square ) const 
{MEASURE
    int x_coord = square % 8;
    int y_coord = square / 8;

    int x_target;
    int y_target;

    auto traverse = [&](auto lambda)
    {
        while ( validCoordinates(x_target, y_target) )
        {
            int target_square = y_target*8 + x_target;

            if ( !isSquareOccupied( target_square ) )
            {
                appendVariation( variations, square, target_square );
                lambda();
            }
            else
            {
                if ( turn_ == WHITE_TURN )
                {
                    if ( isSquareBlack( target_square ) )
                        appendVariation( variations, square,target_square );
                }
                else if ( turn_ == BLACK_TURN )
                {
                    if ( isSquareWhite( target_square ) )
                        appendVariation( variations, square, target_square );
                }
                break;
            }
        }
    };
    
    auto move_up_left    = [&] () {--y_target; --x_target;};
    auto move_up_right   = [&] () {--y_target; ++x_target;};
    auto move_down_left  = [&] () {++y_target; --x_target;};
    auto move_down_right = [&] () {++y_target; ++x_target;};

    // up left
    x_target = x_coord-1;
    y_target = y_coord-1;
    traverse(move_up_left);

    // up right
    x_target = x_coord+1;
    y_target = y_coord-1;
    traverse(move_up_right);

    // down left
    x_target = x_coord-1;
    y_target = y_coord+1;
    traverse(move_down_left);

    // left
    x_target = x_coord+1;
    y_target = y_coord+1;
    traverse(move_down_right);
}

void Chessboard::findQueenVariations(Variations& variations, int square ) const
{MEASURE
    findRookVariations( variations, square );
    findBishopVariations( variations, square );
}

void Chessboard::findKingVariations(Variations& variations, int square ) const
{MEASURE
    int x_coord = square % 8;
    int y_coord = square / 8;

    int x[8];
    int y[8];

    // position 0
    x[0] = x_coord - 1;
    y[0] = y_coord - 1;

    // position 1
    x[1] = x_coord;
    y[1] = y_coord - 1;

    // position 2
    x[2] = x_coord + 1;
    y[2] = y_coord - 1;

    // position 3
    x[3] = x_coord - 1;
    y[3] = y_coord;

    // position 4
    x[4] = x_coord + 1;
    y[4] = y_coord;

    // position 5
    x[5] = x_coord - 1;
    y[5] = y_coord + 1;

    // position 6
    x[6] = x_coord;
    y[6] = y_coord + 1;

    // position 7
    x[7] = x_coord + 1;
    y[7] = y_coord + 1;

    auto checkPosition = [this, &square, &variations](auto x, auto y)
    {
        if ( validCoordinates(x, y) )
        {
            int target_square = y*8 + x;

            if ( turn_ == WHITE_TURN )
            {
                if ( isSquareBlack(target_square) || !isSquareOccupied(target_square) )
                    appendVariation( variations, square, target_square );
            }
            else if ( turn_ == BLACK_TURN )
            {
                if ( isSquareWhite(target_square) || !isSquareOccupied(target_square) )
                    appendVariation( variations, square, target_square );
            }
        }
    };

    for ( int i = 0; i < 8; ++i )
        checkPosition( x[i], y[i] );

    // castling
    if ( turn_ == WHITE_TURN )
    {
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_KING))
        {
            if ( !isSquareOccupied(61) && !isSquareOccupied(62) && !isPieceThreatened(61) && !isPieceThreatened(62) )
                appendVariation( variations, 60, 62 );
        }
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_QUEEN))
        {
            if ( !isSquareOccupied(58) && !isSquareOccupied(59) && !isPieceThreatened(58) && !isPieceThreatened(59) )
                appendVariation( variations, 60, 58 );
        }
    }
    else
    {
        if (castling_ & static_cast<uint8_t>(CASTLING::BLACK_KING) )
        {
            if ( !isSquareOccupied(5) && !isSquareOccupied(6) && !isPieceThreatened(5) && !isPieceThreatened(6) )
                appendVariation( variations, 4, 6 );
        }
        if (castling_ & static_cast<uint8_t>(CASTLING::BLACK_QUEEN) )
        {
            if ( !isSquareOccupied(2) && !isSquareOccupied(3) && !isPieceThreatened(2) && !isPieceThreatened(3) )
                appendVariation( variations, 4, 2 );
        }
   } 
}

bool Chessboard::isSquareOccupied( int square ) const
{MEASURE
    return data_[square] != EMPTY_SQUARE;
}

bool Chessboard::isSquareWhite( int square ) const
{MEASURE
    /*if ( data_[square] == WHITE_PAWN || 
         data_[square] == WHITE_ROOK || 
         data_[square] == WHITE_KNIGHT || 
         data_[square] == WHITE_BISHOP || 
         data_[square] == WHITE_QUEEN || 
         data_[square] == WHITE_KING )
         return true;*/
         return data_[square] >= 65 && data_[square] <=90;

    //return false;
}

bool Chessboard::isSquareBlack( int square ) const
{MEASURE
    /*if ( data_[square] == BLACK_PAWN || 
         data_[square] == BLACK_ROOK || 
         data_[square] == BLACK_KNIGHT || 
         data_[square] == BLACK_BISHOP || 
         data_[square] == BLACK_QUEEN || 
         data_[square] == BLACK_KING )
         return true;*/
    return data_[square] >= 97 && data_[square] <=122;

    //return false;
}

bool Chessboard::isWhiteTurn() const
{MEASURE
    return turn_ == WHITE_TURN;
}