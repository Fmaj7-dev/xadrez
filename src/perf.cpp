#include "chessboard.h"
#include "variation.h"
#include "util/ettune.h"

#include <iostream>
#include <chrono>

void perf( Chessboard& cb, int depth, uint64_t& total_moves )
{MEASURE
  Variations v;
  v.reserve(15);

  cb.findVariations(v);

  if (depth == 1)
  {
    total_moves += v.size();

    /*for (auto& v : v)
    {
      std::cout<<cb.exportFen()<<" ->";
      cb.makeMove(v.movement_);
      std::cout<<v.movement_.str()<<" -> "<<cb.exportFen()<<std::endl;
      cb.undoMove();
    }*/
    return;
  }

  for (auto& v : v)
  {
    cb.makeMove( v.movement_ );
  
    if (depth > 0)  
      perf( cb, depth - 1, total_moves);
      //perf( v.chessboard_, depth - 1, total_moves);

    cb.undoMove();
  }
}

int main(int charc, char** argv)
{MEASURE

  int depth = 5;

  if (charc > 1)
    depth = std::stoi(argv[1]);

  auto start = std::chrono::steady_clock::now();

  // Create a chessboard
  Chessboard cb;
  cb.initDefault();

  // from https://www.chessprogramming.org/Perft_Results

  //position 2
  //cb.importFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
  
  //position 3
  //cb.importFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

  // position 4
  //cb.importFen("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");

  // position 5
  //cb.importFen("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ");

  // position 6
  //cb.importFen("r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 ");

  uint64_t total_moves = 0;

  perf( cb, depth, total_moves );

  auto end = std::chrono::steady_clock::now();

  std::cout<<"Total moves: "<<total_moves<<std::endl;

  std::cout << "Elapsed time in milliseconds: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
      << " ms" << std::endl;
}
