#ifndef UCI_H
#define UCI_H

#include "engine.h"

/**
 * Universal Chess Interface
 */
class UCI
{
public:
    UCI();

    int run();

private:
    Engine engine_;
};

#endif