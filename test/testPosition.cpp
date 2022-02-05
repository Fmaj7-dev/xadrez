#include "position.h"
#include <iostream>
#include <catch2/catch_amalgamated.hpp>

TEST_CASE("Test Positions")
{
    Position a8(0);
    REQUIRE( a8.getStr() == "a8");

    Position a1(56);
    REQUIRE( a1.getStr() == "a1");

    Position h8(7);
    REQUIRE( h8.getStr() == "h8");

    Position h1(63);
    REQUIRE( h1.getStr() == "h1");

    Position e4(36);
    REQUIRE( e4.getStr() == "e4");

    REQUIRE (Position::char2int("a8") == 0);
    REQUIRE (Position::char2int("a1") == 56);
    REQUIRE (Position::char2int("h8") == 7);
    REQUIRE (Position::char2int("h1") == 63);
    REQUIRE (Position::char2int("e4") == 36);
}