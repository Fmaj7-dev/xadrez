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