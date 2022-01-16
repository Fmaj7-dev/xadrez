#include "chessboard.h"
#include "variation.h"
#include "util/log.h"
#include "util/ettune.h"

#include <algorithm>
//#include <thread>
#include <boost/algorithm/string.hpp>

Chessboard::Chessboard()
{MEASURE
}

Chessboard::Chessboard(const Chessboard& rhs)
{MEASURE
    this->operator=(rhs);
}

const Chessboard& Chessboard::operator=(const Chessboard& rhs)
{MEASURE
    std::copy(std::begin(rhs.data_), std::end(rhs.data_), std::begin(data_));

    turn_ = rhs.turn_;
    castling_ = rhs.castling_;
    enpassant_[0] = rhs.enpassant_[0];
    enpassant_[1] = rhs.enpassant_[1];
    halfCount_ = rhs.halfCount_;
    fullCount_ = rhs.fullCount_;
    whiteKingPosition_ = rhs.whiteKingPosition_;
    blackKingPosition_ = rhs.blackKingPosition_;

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
    //libc++abi.dylib: terminating with uncaught exception of type std::invalid_argument: stoi: no conversion
    //pastPositions_.clear();
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
        {
            data_[insertPosition] = c;

            if (c == WHITE_KING)
                whiteKingPosition_ = insertPosition;
            else if (c == BLACK_KING)
                blackKingPosition_ = insertPosition;

            insertPosition++;
        }

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
    std::cout << "\n" << std::endl;
    std::cout << "  _______________" << std::endl;

    for ( int i = 0; i < 8; ++i )
    {
        std::cout << 8-i << "|";
        for ( int j = 0; j < 8; ++j )
        {
            if (data_[i*8+j] == EMPTY_SQUARE)
                std::cout << ". ";
            else
                std::cout << data_[i*8+j]<<" ";
        }
        std::cout << "|"<<std::endl;
    }
    std::cout<<"  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯"<<std::endl;
    std::cout<<"  a b c d e f g h"<<std::endl;
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

    Variation variation(movement);
    
#if (UNDO_FEN_STRING)
    // optimization: avoid creating each time
    static Chessboard vchessboard_;
    vchessboard_ = *this;

    vchessboard_.makeMove( movement, true );

    // undo the switch makeMove did, so that we can really test if the king is threatened
    vchessboard_.switchTurn();

    if (!vchessboard_.isInCheck())
        variations.push_back( variation );

    /*makeMove( movement, false);
    switchTurn();
    if (!isInCheck())
        variations.push_back( variation );
    undoMove();*/
#else
    variation.chessboard_ = *this;
    variation.chessboard_.makeMove( movement, true );
    variation.chessboard_.switchTurn();
    if (!variation.chessboard_.isInCheck())
        variations.push_back( variation );
#endif
}

bool Chessboard::isInCheck() const
{MEASURE
    int kingPosition = findKing();

    return isPieceThreatened( kingPosition );
}

int Chessboard::findKing() const
{MEASURE
    if ( turn_ == BLACK_TURN )
        return blackKingPosition_;
    else 
        return whiteKingPosition_;
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

    Piece pieceToSearch;
    if ( turn_ == WHITE_TURN )
        pieceToSearch = BLACK_KNIGHT;
    else
        pieceToSearch = WHITE_KNIGHT;

    for (int i = 0; i < 8; ++i)
        if( validCoordinates(x[i], y[i]) && data_[x[i] + y[i]*8] == pieceToSearch )
            return true;

    // threatened by rook or queen
    int x_target;
    int y_target;

    auto traverse = [&](auto lambda, Piece piece1, Piece piece2)
    {
        while ( validCoordinates(x_target, y_target) )
        {
            int target_square = y_target*8 + x_target;

            if ( !isSquareOccupied( target_square ) )
                lambda();
            else
            {
                if ( data_[target_square] == piece1 ||  data_[target_square] == piece2)
                    return true;

                return false;
            }
        }
        return false;
    };

    auto move_up =      [&] () {--y_target;};
    auto move_down =    [&] () {++y_target;};
    auto move_right =   [&] () {++x_target;};
    auto move_left =    [&] () {--x_target;};

    Piece pieceToSearch1;
    Piece pieceToSearch2;

    if (turn_ == WHITE_TURN)
    {
        pieceToSearch1 = BLACK_ROOK;
        pieceToSearch2 = BLACK_QUEEN;
    }
    else
    {
        pieceToSearch1 = WHITE_ROOK;
        pieceToSearch2 = WHITE_QUEEN;
    }


    // up
    x_target = x_coord;
    y_target = y_coord-1;
    if (traverse(move_up, pieceToSearch1, pieceToSearch2) )
        return true;

    // down
    x_target = x_coord;
    y_target = y_coord+1;
    if (traverse(move_down, pieceToSearch1, pieceToSearch2) )
        return true;

    // right
    x_target = x_coord+1;
    y_target = y_coord;
    if (traverse(move_right, pieceToSearch1, pieceToSearch2) )
        return true;

    // left
    x_target = x_coord-1;
    y_target = y_coord;
    if (traverse(move_left, pieceToSearch1, pieceToSearch2) )
        return true;
    
    // threatened by bishop or queen
    // FIXME make sure the captures are correct
    /*auto traverseBishopQueen = [&](auto lambda)
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
    };*/

    auto move_up_left    = [&] () {--y_target; --x_target;};
    auto move_up_right   = [&] () {--y_target; ++x_target;};
    auto move_down_left  = [&] () {++y_target; --x_target;};
    auto move_down_right = [&] () {++y_target; ++x_target;};

    if (turn_ == WHITE_TURN)
    {
        pieceToSearch1 = BLACK_BISHOP;
        pieceToSearch2 = BLACK_QUEEN;
    }
    else
    {
        pieceToSearch1 = WHITE_BISHOP;
        pieceToSearch2 = WHITE_QUEEN;
    }

    // up left
    x_target = x_coord-1;
    y_target = y_coord-1;
    if ( traverse(move_up_left, pieceToSearch1, pieceToSearch2) )
        return true;

    // up right
    x_target = x_coord+1;
    y_target = y_coord-1;
    if ( traverse(move_up_right, pieceToSearch1, pieceToSearch2) )
        return true;

    // down left
    x_target = x_coord-1;
    y_target = y_coord+1;
    if ( traverse(move_down_left, pieceToSearch1, pieceToSearch2) )
        return true;

    // down left
    x_target = x_coord+1;
    y_target = y_coord+1;
    if ( traverse(move_down_right, pieceToSearch1, pieceToSearch2) )
        return true;

    // threatened by pawn
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

    // FIXME: we are not checking minimum distance to the other king

    /*if (blackKingPosition_ - 9 == whiteKingPosition_ ||
        blackKingPosition_ - 8 == whiteKingPosition_ ||
        blackKingPosition_ - 7 == whiteKingPosition_ ||
        blackKingPosition_ - 1 == whiteKingPosition_ ||
        blackKingPosition_ + 1 == whiteKingPosition_ ||
        blackKingPosition_ + 7 == whiteKingPosition_ ||
        blackKingPosition_ + 8 == whiteKingPosition_ ||
        blackKingPosition_ + 9 == whiteKingPosition_)
        return true;*/

    // check king
    Piece pieceToCheck;
    if (turn_ == WHITE_TURN)
        pieceToCheck = BLACK_KING;
    else
        pieceToCheck = WHITE_KING;

    if ( (validCoordinates(x_coord-1, y_coord-1) && data_[x_coord-1 + (y_coord-1)*8] == pieceToCheck) ||
         (validCoordinates(x_coord-1, y_coord) && data_[x_coord-1 + (y_coord)*8] == pieceToCheck) ||
         (validCoordinates(x_coord-1, y_coord+1) && data_[x_coord-1 + (y_coord+1)*8] == pieceToCheck) ||
         (validCoordinates(x_coord+1, y_coord-1) && data_[x_coord+1 + (y_coord-1)*8] == pieceToCheck) ||
         (validCoordinates(x_coord+1, y_coord) && data_[x_coord+1 + (y_coord)*8] == pieceToCheck) ||
         (validCoordinates(x_coord+1, y_coord+1) && data_[x_coord+1 + (y_coord+1)*8] == pieceToCheck) ||
         (validCoordinates(x_coord, y_coord-1) && data_[x_coord + (y_coord-1)*8] == pieceToCheck) ||
         (validCoordinates(x_coord, y_coord+1) && data_[x_coord + (y_coord+1)*8] == pieceToCheck) )
        return true;

    return false;
}

Chessboard::Piece Chessboard::makeMove( Movement& m, bool ignoreExport )
{MEASURE

    if (!ignoreExport)
    {
        //pastPositions_.push_back( exportFen() );
        pastPositions_.push_back( *this );  
    }

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

        // make sure castling rights are lost if another piece is placed on rook squares
        // this is to avoid a corner case: if our rook is captured, but we promote another
        // pawn to a rook, and then move that rook to the square where the other rook was captured,
        // the king would be able to castle, but that is not valid
        if (m.to().getValue() == A1)
            castling_ &=  ~static_cast<uint8_t>(CASTLING::WHITE_QUEEN);
        else if (m.to().getValue() == H1)
            castling_ &=  ~static_cast<uint8_t>(CASTLING::WHITE_KING);
        else if (m.to().getValue() == A8)
            castling_ &=  ~static_cast<uint8_t>(CASTLING::BLACK_QUEEN);
        else if (m.to().getValue() == H8)
            castling_ &=  ~static_cast<uint8_t>(CASTLING::BLACK_KING);

        if (data_[ m.from().getValue() ] == WHITE_KING)
        {
            castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_KING);
            castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_QUEEN);
            whiteKingPosition_ = m.to().getValue();
        }
        else if (data_[ m.from().getValue() ] == BLACK_KING)
        {
            castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_KING);
            castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_QUEEN);
            blackKingPosition_ = m.to().getValue();
        }
        else if (data_[ m.from().getValue() ] == WHITE_ROOK)
        {
            if (m.from().getValue() == 56)
                castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_QUEEN);
            else if (m.from().getValue() == 63)
                castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_KING);
        }
        else if (data_[ m.from().getValue() ] == BLACK_ROOK)
        {
            if (m.from().getValue() == 0)
                castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_QUEEN);
            else if (m.from().getValue() == 7)
                castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_KING);
        }
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
    else if ( m.type() == Movement::Type::Castling )
    {
        if ( m.to().getValue() == 62 )
        {
            castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_KING);
            castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_QUEEN);

            whiteKingPosition_ = 62;

            data_[60] = EMPTY_SQUARE;
            data_[62] = WHITE_KING;
            data_[63] = EMPTY_SQUARE;
            data_[61] = WHITE_ROOK;
        }
        else if ( m.to().getValue() == 58 )
        {
            castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_KING);
            castling_ &= ~static_cast<uint8_t>(CASTLING::WHITE_QUEEN);

            whiteKingPosition_ = 58;

            data_[60] = EMPTY_SQUARE;
            data_[58] = WHITE_KING;
            data_[56] = EMPTY_SQUARE;
            data_[59] = WHITE_ROOK;
        }
        else if ( m.to().getValue() == 6 )
        {
            castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_KING);
            castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_QUEEN);

            blackKingPosition_ = 6;

            data_[4] = EMPTY_SQUARE;
            data_[6] = BLACK_KING;
            data_[7] = EMPTY_SQUARE;
            data_[5] = BLACK_ROOK;
        }
        else if ( m.to().getValue() == 2 )
        {
            castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_KING);
            castling_ &= ~static_cast<uint8_t>(CASTLING::BLACK_QUEEN);

            blackKingPosition_ = 2;

            data_[4] = EMPTY_SQUARE;
            data_[2] = BLACK_KING;
            data_[0] = EMPTY_SQUARE;
            data_[3] = BLACK_ROOK;
        }
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
    //importFen( pastPositions_.back() );
    this->operator=( pastPositions_.back() );
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

    /*std::thread pawn_thread;
    std::thread knight_thread;
    std::thread bishop_thread;
    std::thread rook_thread;
    std::thread queen_thread;
    std::thread king_thread;*/

    for (int i = 0; i < 64; ++i )
    {
        if ( turn_ == WHITE_TURN )
        {
            
            if ( data_[i] == WHITE_PAWN )
            {
                findPawnVariations(variations, i);
                //pawn_thread = std::thread([this, &variations, &i]() { this->findPawnVariations(variations, i); });
            }
            else if ( data_[i] == WHITE_ROOK )
                findRookVariations(variations, i);
                //rook_thread = std::thread([this, &variations, &i]() { this->findRookVariations(variations, i); });
            else if ( data_[i] == WHITE_KNIGHT )
                findKnightVariations(variations, i);
                //knight_thread = std::thread([this, &variations, &i]() { this->findKnightVariations(variations, i); });
            else if ( data_[i] == WHITE_BISHOP )
                findBishopVariations(variations, i);
                //bishop_thread = std::thread([this, &variations, &i]() { this->findBishopVariations(variations, i); });
            else if ( data_[i] == WHITE_QUEEN )
                findQueenVariations(variations, i);
                //queen_thread = std::thread([this, &variations, &i]() { this->findQueenVariations(variations, i); });
            else if ( data_[i] == WHITE_KING )
                findKingVariations(variations, i);
                //king_thread = std::thread([this, &variations, &i]() { this->findKingVariations(variations, i); });

            //if (pawn_thread.joinable()) pawn_thread.join();
            /*if (knight_thread.joinable()) knight_thread.join();
            if (bishop_thread.joinable()) bishop_thread.join();
            if (rook_thread.joinable()) rook_thread.join();
            if (queen_thread.joinable()) queen_thread.join();
            if (king_thread.joinable()) king_thread.join();*/
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
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_KING) && data_[H1] == WHITE_ROOK)
        {
            if ( !isSquareOccupied(F1) && !isSquareOccupied(G1) && 
                 !isPieceThreatened(E1) && !isPieceThreatened(F1) && !isPieceThreatened(G1))
                appendVariation( variations, 60, 62, Movement::Type::Castling );
        }
        if ( castling_ & static_cast<uint8_t>(CASTLING::WHITE_QUEEN) && data_[A1] == WHITE_ROOK)
        {
            if ( !isSquareOccupied(B1) && !isSquareOccupied(C1) && !isSquareOccupied(D1) && 
                 !isPieceThreatened(C1) && !isPieceThreatened(D1) &&  !isPieceThreatened(E1))
                appendVariation( variations, 60, 58, Movement::Type::Castling );
        }
    }
    else
    {
        if (castling_ & static_cast<uint8_t>(CASTLING::BLACK_KING) && data_[H8] == BLACK_ROOK)
        {
            if ( !isSquareOccupied(F8) && !isSquareOccupied(G8) && 
                 !isPieceThreatened(E8) && !isPieceThreatened(F8) && !isPieceThreatened(G8))
                appendVariation( variations, 4, 6, Movement::Type::Castling );
        }
        if (castling_ & static_cast<uint8_t>(CASTLING::BLACK_QUEEN) && data_ [A8] == BLACK_ROOK)
        {
            if ( !isSquareOccupied(B8) && !isSquareOccupied(C8) && !isSquareOccupied(D8) && 
                 !isPieceThreatened(C8) && !isPieceThreatened(D8) &&  !isPieceThreatened(E8))
                appendVariation( variations, 4, 2, Movement::Type::Castling );
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