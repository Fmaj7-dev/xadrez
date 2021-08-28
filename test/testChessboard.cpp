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
