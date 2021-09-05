#include "catch.hpp"
#include "position.h"
#include <iostream>

TEST_CASE("Test Positions")
{
    Position a8(0);
    REQUIRE( a8.str() == "a8");

    Position a1(56);
    REQUIRE( a1.str() == "a1");

    Position h8(7);
    REQUIRE( h8.str() == "h8");

    Position h1(63);
    REQUIRE( h1.str() == "h1");

    Position e4(36);
    REQUIRE( e4.str() == "e4");
}