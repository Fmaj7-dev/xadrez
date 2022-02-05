#include "engine.h"
#include "variation.h"
#include "catch.hpp"

#include <iostream>
#include <vector>

TEST_CASE("Test Engine")
{
    // fail with this:
    //1k3b2/p1p3r1/2p2p2/3pp3/3P1pP1/2N2P2/P1P2KP1/8 b - - 3 30
    
    // null move with this:
    //2r5/4ppk1/p1P5/8/P2p4/8/q7/4K3 b - - 1 57

/*    Chessboard cb;
    cb.importFen("4k3/8/2r5/8/4B3/8/4PPPP/6KR b - - 0 1");
    Variations v;
    cb.findVariations(v);

    for (Variation v : v)
    {
        cb.makeMove(v.movement_);
        std::cout<<v.movement_.str()<<" -> "<<cb.evaluation()<<std::endl;

        cb.undoMove();
    }*/

    Engine engine;
    engine.loadFen("4k3/8/2r5/8/4B3/8/4PPPP/6KR b - - 0 1");

    std::string best = engine.findBestMove(3);

    std::cout<<"best: "<<best<<std::endl;
}