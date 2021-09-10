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

    float eval = 100;
    int chosen = 0;

    for( size_t i = 0; i < v.size(); ++i)
    {
        etlog(v[i].movement_.str() + " -> " + v[i].chessboard_.exportFen());
        float neval = v[i].chessboard_.evaluation();

        if (neval < eval)
        {
            eval = neval;
            chosen = i;
        }
    }

    etlog("Best move: " + v[chosen].movement_.str()+" eval: "+std::to_string(eval));

    return v[chosen].movement_.str();
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
