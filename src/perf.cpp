#include "chessboard.h"
#include "variation.h"

#include <iostream>
#include <chrono>

void perf( Chessboard& cb, int depth, int& total_moves )
{
  Variations v;
  cb.findVariations(v);

  if (depth == 1)
  {
    total_moves += v.size();

    // print moves
    for (auto& v : v)
    {
      cb.makeMove( v.movement_ );
      //std::cout << v.movement_.str()<< " " <<cb.exportFen() << std::endl;    
      cb.undoMove();
    }

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

int main()
{
  auto start = std::chrono::steady_clock::now();

  // Create a chessboard
  Chessboard cb;
  cb.initDefault();
  //cb.importFen("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1");

  int total_moves = 0;

  perf( cb, 6, total_moves );

  auto end = std::chrono::steady_clock::now();

  std::cout<<"Total moves: "<<total_moves<<std::endl;

  std::cout << "Elapsed time in milliseconds: "
      << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
      << " ms" << std::endl;
}
