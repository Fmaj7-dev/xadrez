#include "catch.hpp"
#include "movement.h"
#include <iostream>

TEST_CASE("Test Movement")
{
    Movement e2e4(52, 36);
    REQUIRE( e2e4.str() == "e2e4");
}