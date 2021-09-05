#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "chessboard.h"
#include <iostream>
#include <fstream>

TEST_CASE("FEM default init")
{
    Chessboard cb;
    std::string default_init ("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    cb.initDefault();

    std::string exported = cb.exportFem();

    REQUIRE( exported == default_init );
}

TEST_CASE("FEM custom import")
{
    Chessboard cb;
    std::string init ("rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2");
    cb.importFem(init);

    std::string exported = cb.exportFem();

    REQUIRE( exported == init );
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
    cb.importFem(init);
    eval = cb.evaluation();
    REQUIRE( eval == 5.0f );

    // test with one black rook missing
    std::string init2 ("1nbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNB1KBNR w KQkq - 0 1"); 
    cb.importFem(init2);
    eval = cb.evaluation();
    REQUIRE( eval == -4.0f );
}