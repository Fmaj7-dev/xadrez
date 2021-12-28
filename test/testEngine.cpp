#include "catch.hpp"
#include "engine.h"
#include <iostream>

TEST_CASE("Test Engine")
{
    Engine e;
    e.loadFen("");
    float value = e.runAB();

    std::cout<<"value: "<<value<<std::endl;
}