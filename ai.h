#ifndef AI_H_INCLUDED
#define AI_H_INCLUDED

#include "moves.h"
#include "game.h"
#include "transposition_table.h"
#include <thread>
const int8_t playerColor[2]={1, -1};
//Static values for Pawn, Rook, Knight, Bishop, Queen, King
const int64_t staticPieceValue[6]={100, 500, 300, 300, 900, 20000};
class AIEngine
{
public:
    AIEngine(){}
    ~AIEngine(){m_Exit = true; if(m_Inited) m_dispatcher.join();}
    void init(){m_Inited = true; m_dispatcher=std::move(std::thread(std::bind(&AIEngine::dispatcherLoop, this)));}
    void makeMove(Game& game, const Player& player);
    void getMove(Game& game, const Player& player);
    static int64_t evaluateBoard(const Game& game);
    void dispatcherLoop();
    struct Move negamax(const Game& game, const Player& player, int32_t depth, int32_t alpha, int32_t beta);
    int32_t negamax_inner(Game gameStateCopy, const Player& player, int32_t depth, int32_t alpha, int32_t beta);
    bool m_Exit = false;
    bool m_Inited = false;
private:
    TTable m_hashTable;
    Game* game_Ptr = nullptr;
    Player playerToMove;
    std::thread m_dispatcher;
};
#endif // AI_H_INCLUDED
