#include "engine.h"
#include "variation.h"
#include "log.h"

const std::string Engine::author = "FMaj7";
const std::string Engine::name = "xadrez 1.0";

Engine::Engine()
{

}

std::string Engine::findBestMove( uint32_t seconds )
{
    seconds = 0;

    Variations v;
    chessboard_.findVariations(v);

    int len = v.size();

    int randNum = rand()%(len);
    return v[randNum].movement_.str();
}

void Engine::cancelFind()
{

}

std::string Engine::getAuthor()
{
    return author;
}

std::string Engine::getName()
{
    return name;
}

void Engine::loadFen(std::string fen)
{
    chessboard_.importFen(fen);
}
