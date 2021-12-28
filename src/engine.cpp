#include "engine.h"
#include "variation.h"
#include "log.h"

const std::string Engine::author = "FMaj7";
const std::string Engine::name = "xadrez 1.0";

Engine::Engine()
{

}

float alphaBetaMin(float alpha, float beta, float depthleft, Chessboard& chessboard);

float alphaBetaMax(float alpha, float beta, float depthleft, Chessboard& chessboard)
{
    if (depthleft == 0)
        return chessboard.evaluation();

    Variations variations;
    chessboard.findVariations(variations);
    for (Variation v : variations)
    {
        chessboard.makeMove(v.movement_);
        float score = alphaBetaMin(alpha, beta, depthleft - 1, chessboard);
        chessboard.undoMove();
        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}

float alphaBetaMin(float alpha, float beta, float depthleft, Chessboard& chessboard)
{
    if (depthleft == 0)
        return chessboard.evaluation();

    Variations variations;
    chessboard.findVariations(variations);
    for (Variation v : variations)
    {
        chessboard.makeMove(v.movement_);
        int score = alphaBetaMax(alpha, beta, depthleft - 1, chessboard);
        chessboard.undoMove();
        if (score <= alpha)
            return alpha;
        if (score < beta)
            beta = score;
    }
    return beta;
}

float Engine::runAB()
{
    return alphaBetaMax(-100, 100, 2, chessboard_);
}

std::string Engine::findBestMove( uint32_t seconds )
{
    seconds = 0;

    Variations v;
    chessboard_.findVariations(v);

    float eval = 100;
    int chosen = 0;

    etlog("found "+std::to_string(v.size())+" variations");

    for( size_t i = 0; i < v.size(); ++i)
    {
        //etlog(v[i].movement_.str() + " -> " + v[i].chessboard_.exportFen());
        chessboard_.makeMove(v[i].movement_);
        float neval = chessboard_.evaluation();
        chessboard_.undoMove();

        // second level variations
        #if 0
        {
            Variations v2;
            v[i].chessboard_.findVariations(v2);

            for ( size_t j = 0; j < v2.size(); ++j )
            {
                etlog("\t"+v2[j].movement_.str() + " -> " + v2[j].chessboard_.exportFen());

                    // third level variations
                    #if 1
                    {
                        Variations v3;
                        v2[j].chessboard_.findVariations(v3);

                        for ( size_t k = 0; k < v3.size(); ++k )
                        {
                            etlog("\t\t"+v3[k].movement_.str() + " -> " + v3[k].chessboard_.exportFen());
                        }
                    }
                    #endif
            }
        }
        #endif

        if (neval < eval)
        {
            eval = neval;
            chosen = i;
        }
    }

    if (v.empty())
        return "0000";

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
