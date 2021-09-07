#include "engine.h"

const std::string Engine::author = "FMaj7";
const std::string Engine::name = "xadrez 1.0";

Engine::Engine()
{

}

std::string Engine::findBestMove()
{
    return std::string("e7e5");
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