#ifndef ENGINE_H
#define ENGINE_H

#include "chessboard.h"

#include <string>

class Engine
{
    // strings
    static const std::string author;
    static const std::string name;

public:
    Engine();
    void init(/* color */);

    // engine info
    std::string getAuthor();
    std::string getName();

    // load fem to chessboard
    void loadFem(std::string fem);

    std::string findBestMove( uint32_t seconds );
    void cancelFind();

    Chessboard chessboard_;
};

#endif
