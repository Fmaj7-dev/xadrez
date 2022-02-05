#include "movement.h"
#include <iostream>
#include <catch2/catch_amalgamated.hpp>

TEST_CASE("Test Movement")
{
    Movement e2e4(52, 36);
    REQUIRE( e2e4.str() == "e2e4");
}