#include "engine.h"
#include "variation.h"
#include "util/log.h"

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
#if (UNDO_FEN_STRING)        
        chessboard.makeMove(v.movement_);
        float score = alphaBetaMin(alpha, beta, depthleft - 1, chessboard, &line);
        chessboard.undoMove();
#else
        float score = alphaBetaMin(alpha, beta, depthleft - 1, v.chessboard_, &line);
#endif
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
#if (UNDO_FEN_STRING)
        chessboard.makeMove(v.movement_);
        float score = alphaBetaMax(alpha, beta, depthleft - 1, chessboard, &line);
        chessboard.undoMove();
#else
        float score = alphaBetaMax(alpha, beta, depthleft - 1, v.chessboard_, &line);
#endif
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

/*float Engine::runAB(int depth)
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
}*/

std::string Engine::findBestMove( uint32_t seconds )
{
    LINE line;
    float ab;
    int depth = 6;

    if (chessboard_.isWhiteTurn())
        ab = alphaBetaMax(-10000, 10000, depth, chessboard_, &line);
    else
        ab = alphaBetaMin(-10000, 10000, depth, chessboard_, &line);

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
