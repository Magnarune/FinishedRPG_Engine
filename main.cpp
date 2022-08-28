#include "Engine.h"
int main()
{
    RPG_Engine Game;
    if (Game.Construct(256, 240, 4, 4))
    Game.Start();
    return 0;
}