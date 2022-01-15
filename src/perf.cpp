#include "chessboard.h"
#include "variation.h"
#include "util/ettune.h"

#include <iostream>
#include <chrono>

void perf( Chessboard& cb, int depth, uint32_t& total_moves )
{MEASURE
  Variations v;
  v.reserve(15);

  cb.findVariations(v);

  if (depth == 1)
  {
    total_moves += v.size();

    for (auto& v : v)
    {
      cb.makeMove(v.movement_);
      std::cout<<v.movement_.str()<<" "<<cb.exportFen()<<std::endl;
      cb.undoMove();
    }
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
  //cb.initDefault();
  cb.importFen("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");

  uint32_t total_moves = 0;

  perf( cb, depth, total_moves );

  auto end = std::chrono::steady_clock::now();

  std::cout<<"Total moves: "<<total_moves<<std::endl;

  std::cout << "Elapsed time in milliseconds: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
      << " ms" << std::endl;
}
