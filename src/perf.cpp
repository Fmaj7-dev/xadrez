#include "chessboard.h"
#include "variation.h"

#include <iostream>
#include <chrono>

int main()
{
  auto start = std::chrono::steady_clock::now();

  Chessboard cb;
  cb.initDefault();

  Variations v;
  cb.findVariations(v);
}
