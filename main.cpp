#include <iostream>
#include <Game.h>
#include <Configuration.h>

int main()
{
    Configuration::initialize();

    Game g;

    g.run(25);

    Configuration::release();

    return 0;
}
