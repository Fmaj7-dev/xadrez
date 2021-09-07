#include "chessboard.h"

#include <string>

class Engine
{
public:
    Engine();
    void init(/* color */);

    void loadFem(std::string fem);

    std::string findBestMove(/* time limit?, other options?*/);
    void cancelFind();


};