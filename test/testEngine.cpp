#include "catch.hpp"
#include "engine.h"
#include <iostream>

TEST_CASE("Test Engine")
{
    Engine e;

    // fails with null move
    e.loadFen("2Q5/1k1Q4/8/6p1/1r6/6PP/2K5/8 b - - 0 68");
    std::string value = e.findBestMove(10);

    std::cout<<"value: "<<value<<std::endl;
}