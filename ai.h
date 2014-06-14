#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "moves.h"
#include "game.h"

const int8_t playerColor[2]={1, -1};
//Static values for Pawn, Rook, Knight, Bishop, Queen, King
const int64_t staticPieceValue[6]={100, 500, 300, 300, 900, 20000};
class AIEngine
{
public:
    void init(){}
    void makeMove(Game& game, const Player& player);
    static int64_t evaluateBoard(const Game& game);
    struct Move negamax(Game& game, const Player& player, int32_t depth, int32_t alpha, int32_t beta);
    int32_t negamax_inner(Game& game, const Player& player, int32_t depth, int32_t alpha, int32_t beta);
private:
};
#endif // AI_H_INCLUDED
