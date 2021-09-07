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

    std::string getAuthor();
    std::string getName();

    void loadFem(std::string fem);

    std::string findBestMove(/* time limit?, other options?*/);
    void cancelFind();


};