#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "chessboard.h"
#include "variation.h"
#include "util/log.h"

#include <iostream>
#include <fstream>

#define UNDO_FEN_STRING true

TEST_CASE("FEM default init")
{
    Chessboard cb;
    std::string default_init ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    cb.initDefault();

    std::string exported = cb.exportFen();

    REQUIRE( exported == default_init );
}

TEST_CASE("FEN custom import")
{
    Chessboard cb;
    std::string init ("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    cb.importFen(init);

    std::string exported = cb.exportFen();

    REQUIRE( exported == init );

    // test custom import
    std::string init2 ("1k6/7P/8/8/6p1/6P1/4q1PK/4r3 w - - 0 1");
    cb.importFen(init2);
    std::string exported2 = cb.exportFen();

    REQUIRE( exported2 == init2 );
}

TEST_CASE("evaluation")
{
    Chessboard cb;

    // test initial evaluation
    cb.initDefault();
    float eval = cb.evaluation();
    REQUIRE( eval == 0.0f );

    // test with one black rook missing
    std::string init ("1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"); 
    cb.importFen(init);
    eval = cb.evaluation();
    REQUIRE( eval == 5.0f );

    // test with one black rook missing
    std::string init2 ("1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1"); 
    cb.importFen(init2);
    eval = cb.evaluation();
    REQUIRE( eval == -4.0f );
}

TEST_CASE("variations")
{
    Chessboard cb;
    cb.initDefault();
    
    Variations v;
    cb.findVariations(v);

    REQUIRE( v.size() == 20 );

    // test king threatened filter
    Variations v2;
    cb.importFen("3ppp2/3pkp2/4pp2/8/1B6/K7/8/8 b - - 0 1");
    cb.findVariations(v2);

    REQUIRE( v2.size() == 1 );

    // test king threatened filter2
    Variations v3;
    cb.importFen("3ppp2/3pkp2/3p1b2/8/8/4R3/4K3/8 b - - 0 1");
    cb.findVariations(v3);

    REQUIRE( v3.size() == 1 );

    // test white promotion
    Variations v4;
    cb.importFen("1k6/7P/8/8/6p1/6P1/4q1PK/4r3 w - - 0 1");
    cb.findVariations(v4);
    REQUIRE( v4.size() == 4 );

    // test black promotion
    Variations v4_2;
    cb.importFen("3Q4/kp6/1p6/1P6/8/8/p6K/8 b - - 0 1");
    cb.findVariations(v4_2);
    REQUIRE( v4_2.size() == 4 );    

    // test white king castling. only 4 white moves are possible in this fem
    Variations v5;
    cb.importFen("3k4/8/8/8/b7/4p2p/4P2P/4K2R w K - 1 1");
    cb.findVariations(v5);
    REQUIRE( v5.size() == 4 );

    // test white queen castling, only 5 white moves are possible in this fem
    Variations v6;
    cb.importFen("3k4/8/8/8/8/p3p2b/P3P3/R3K3 w Q - 1 1");
    cb.findVariations(v6);
    REQUIRE( v6.size() == 5 );

    // test black king castling, only 4 black moves are possible in this fem
    Variations v7;
    cb.importFen("4k2r/4p2p/4P2P/B7/8/8/8/4K3 b k - 1 1");
    cb.findVariations(v7);
    REQUIRE( v7.size() == 4 );

    // test black queen castling, only 5 black moves are possible in this fem
    Variations v8;
    cb.importFen("r3k3/p3p3/P3P2B/8/8/8/8/4K3 b q - 1 1");
    cb.findVariations(v8);
    REQUIRE( v8.size() == 5 );

    // test enpassant white
    Variations v9;
    cb.importFen("rnbqkbnr/1ppppp1p/p7/6pP/8/8/PPPPPPP1/RNBQKBNR w KQkq g6 1 1");
    cb.findVariations(v9);
    REQUIRE( v9.size() == 23 );

    // test enpassant black
    Variations v10;
    cb.importFen("rnbqkbnr/ppppppp1/8/8/6Pp/8/PPPPPP1P/RNBQKBNR b KQkq g3 1 1");
    cb.findVariations(v10);
    REQUIRE( v10.size() == 23 );
}

TEST_CASE("movements")
{
# if (UNDO_FEN_STRING)
    Chessboard cb;
    cb.initDefault();

    // test normal advance pawn
    {
        std::string before = cb.exportFen();
        Movement m(52, 44);
        Chessboard::Piece piece = cb.makeMove(m);
        cb.undoMove();
        std::string after = cb.exportFen();

        REQUIRE( before == after );
    }

    // test double square advance
    {
        std::string before = cb.exportFen();
        Movement m(52, 36, Movement::Type::DoublePawnStep);
        Chessboard::Piece piece = cb.makeMove(m);
        cb.undoMove();
        std::string after = cb.exportFen();

        REQUIRE( before == after );
    }
#endif
}