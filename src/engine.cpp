#include "engine.h"
#include "variation.h"
#include "log.h"

const std::string Engine::author = "FMaj7";
const std::string Engine::name = "xadrez 1.0";

Engine::Engine()
{

}

typedef struct LINE {
    int cmove;              // Number of moves in the line.
    Movement argmove[20];  // The line.
}   LINE;

float alphaBetaMin(float alpha, float beta, float depthleft, Chessboard& chessboard, LINE * pline);

float alphaBetaMax(float alpha, float beta, float depthleft, Chessboard& chessboard, LINE * pline)
{
    LINE line;

    if (depthleft == 0)
    {
        pline->cmove = 0;
        return chessboard.evaluation();
    }

    Variations variations;
    chessboard.findVariations(variations);
    for (Variation v : variations)
    {
        chessboard.makeMove(v.movement_);
        float score = alphaBetaMin(alpha, beta, depthleft - 1, chessboard, &line);
        chessboard.undoMove();
        if (score >= beta)
            return beta;
        if (score > alpha)
        {
            //std::cout<<"score > alpha: "<<v.movement_.str()<<std::endl;
            alpha = score;
            pline->argmove[0] = v.movement_;
            memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(Movement));
            pline->cmove = line.cmove + 1;
        }
    }
    return alpha;
}

float alphaBetaMin(float alpha, float beta, float depthleft, Chessboard& chessboard, LINE * pline)
{
    LINE line;
    //std::cout<<"alphaBetaMin("<<alpha<<", "<<beta<<", "<<depthleft<<")"<<std::endl;
    if (depthleft == 0)
    {
        pline->cmove = 0;
        return chessboard.evaluation();
    }

    Variations variations;
    chessboard.findVariations(variations);
    for (Variation v : variations)
    {
        chessboard.makeMove(v.movement_);
        int score = alphaBetaMax(alpha, beta, depthleft - 1, chessboard, &line);
        chessboard.undoMove();
        if (score <= alpha)
            return alpha;
        if (score < beta)
        {
            //std::cout<<"score < beta: "<<v.movement_.str()<<std::endl;
            beta = score;
            pline->argmove[0] = v.movement_;
            memcpy(pline->argmove + 1, line.argmove, line.cmove * sizeof(Movement));
            pline->cmove = line.cmove + 1;
        }
    }
    return beta;
}

float Engine::runAB(int depth)
{
    LINE line;
    float ab;
    if (chessboard_.isWhiteTurn())
        ab = alphaBetaMax(-1000, 1000, depth, chessboard_, &line);
    else
        ab = alphaBetaMin(-1000, 1000, depth, chessboard_, &line);

    std::cout<<"Move found:"<<std::endl;
    for (int i = 0; i < depth; ++i)
    {
        std::cout<<line.argmove[i].str()<<std::endl;
    }
}

std::string Engine::findBestMove( uint32_t seconds )
{
    /*seconds = 0;

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
    return v[chosen].movement_.str();*/

    LINE line;
    float ab;
    int depth = 4;
    
    if (chessboard_.isWhiteTurn())
        ab = alphaBetaMax(-1000, 1000, depth, chessboard_, &line);
    else
        ab = alphaBetaMin(-1000, 1000, depth, chessboard_, &line);

    return line.argmove[0].str();
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
