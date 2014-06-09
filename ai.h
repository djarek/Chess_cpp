#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "moves.h"

class AIEngine
{
public:
    struct Move getMove(Game& game, const Player& player);
private:

};
#endif // AI_H_INCLUDED
