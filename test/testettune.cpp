#include "util/ettune.h"

#include <thread>
#include <chrono>
#include <catch2/catch_amalgamated.hpp>

void sleep01()
{MEASURE
    std::this_thread::sleep_for (std::chrono::milliseconds(100));
}


void sleep1()
{MEASURE
    std::this_thread::sleep_for (std::chrono::seconds(1));
}

void sleep2()
{MEASURE
    sleep1();
    sleep1();
}

void sleep3()
{MEASURE
    sleep1();
    sleep2();
}

void sleepRec(int depth)
{MEASURE
    sleep1();
    if (depth > 0)
        sleepRec(depth - 1);
}

void mainn()
{MEASURE
    //for (int i = 0; i < 2; i++)
        //sleep01();
          
    /*sleep1();
    sleep2();
    sleep3();*/
    sleepRec(3);
}

TEST_CASE("Test EtTune")
{
    mainn();
}