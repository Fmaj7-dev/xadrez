#include "chessboard.h"
#include "variation.h"

#include <iostream>
#include <chrono>
#include <catch2/catch_amalgamated.hpp>

void perf( Chessboard& cb, int depth, uint64_t& total_moves )
{
  Variations v;
  v.reserve(15);

  cb.findVariations(v);

  if (depth == 1)
  {
    total_moves += v.size();
    return;
  }

  for (auto& v : v)
  {
    cb.makeMove( v.movement_ );
  
    if (depth > 0)  
      perf( cb, depth - 1, total_moves);

    cb.undoMove();
  }
}

  // from https://www.chessprogramming.org/Perft_Results
TEST_CASE("Test Performance")
{
    Chessboard cb;
    cb.initDefault();

    uint64_t total_moves;

    // position 1, depth 1
    total_moves = 0;
    perf( cb, 1, total_moves );
    REQUIRE(total_moves == 20);

    // position 1, depth 2
    total_moves = 0;
    perf( cb, 2, total_moves );
    REQUIRE(total_moves == 400);

    // position 1, depth 3
    total_moves = 0;
    perf( cb, 3, total_moves );
    REQUIRE(total_moves == 8902);

    // position 1, depth 4
    total_moves = 0;
    perf( cb, 4, total_moves );
    REQUIRE(total_moves == 197281);

    // position 1, depth 5
    total_moves = 0;
    perf( cb, 5, total_moves );
    REQUIRE(total_moves == 4865609);
}
