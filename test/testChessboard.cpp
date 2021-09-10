#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "chessboard.h"
#include "variation.h"
#include "log.h"

#include <iostream>
#include <fstream>

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

    for( size_t i = 0; i < v.size(); ++i)
    {
        etlog(v[i].movement_.str() + " -> " + v[i].chessboard_.exportFen());
    }
}