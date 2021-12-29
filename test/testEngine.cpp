#include "catch.hpp"
#include "engine.h"
#include <iostream>

TEST_CASE("Test Engine")
{
    Engine e;
    //e.loadFen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    e.loadFen("r2k1bnr/ppppp3/5pQ1/3q1np1/6bp/8/PPPPPPPP/RNB1KBNR w KQ - 0 1");
    float value = e.runAB(4);

    std::cout<<"value: "<<value<<std::endl;
}