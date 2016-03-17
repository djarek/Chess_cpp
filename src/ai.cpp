#include "ai.h"
#include "globals.h"
#include <limits>
#include <future>
#include <thread>

const int32_t INITIAL_DEPTH = 5;
const int32_t PLUS_INF = std::numeric_limits<int32_t>::max();
const int32_t MINUS_INF = std::numeric_limits<int32_t>::min()+1;

int32_t AIEngine::negamax_inner(Game gameStateCopy, const Player& player, int32_t depth, int32_t alpha, int32_t beta)
{
    if (m_Exit)
        return 0;
    auto alphaOrig = alpha;
    auto hash = getZobristHash(gameStateCopy);
    auto entry = m_hashTable.getState(hash);
    if(hash == (entry.hash^entry.value) && entry.flag < INVALID && entry.depth >= depth)
    {
        if(entry.flag == EXACT)
            return entry.value;
        else if(entry.flag == LOWERBOUND)
            alpha = std::max(alpha, entry.value);
        else if(entry.flag == UPPERBOUND)
            beta = std::min(beta, entry.value);
    }
    MoveVec moves;
    int32_t bestValue = MINUS_INF;
    uint64_t bestHash = 0;
    if(depth <= 0 || (moves=std::move(g_MoveGenerator.generateAllMoves(gameStateCopy, player))).size() == 0)
    {
        return playerColor[player]*evaluateBoard(gameStateCopy);
    }
    /*#ifdef DEBUG_LOG
    std::cout << "# of total moves for player " << player << ": " << moves.size() << std::endl;
    #endif*/
    for(const auto& move : moves)
    {
        if(gameStateCopy.makeMove(move.fromPos, move.toPos, false))
        {
            auto childNodeRetValue = negamax_inner(gameStateCopy, getOtherPlayer(player), depth-1, -beta, -alpha);
            if(-childNodeRetValue > bestValue)
            {
                bestValue = -childNodeRetValue;
                bestHash = getZobristHash(gameStateCopy);
                alpha = std::max(alpha, -childNodeRetValue);
                if(alpha >= beta)
                    break;
            }
            gameStateCopy.restoreState();
            gameStateCopy.m_playerToMove = player;
        }
    }
    if(bestValue <= alphaOrig)
        entry.flag = UPPERBOUND;
    else if(bestValue >= beta)
        entry.flag = LOWERBOUND;
    else
        entry.flag = EXACT;
    m_hashTable.storeState(bestHash, bestValue, depth, entry.flag);
    return bestValue;
}

struct Move AIEngine::negamax(const Game& game, const Player& player, int32_t depth, int32_t alpha, int32_t beta)
{
    MoveVec moves;
    int32_t bestValue = MINUS_INF;
    g_Game_mutex.lock();
    Game gameStateCopy = game;
    g_Game_mutex.unlock();
    struct Move bestMove;
    bestMove.type = MoveType::NoMove;
    if(depth <= 0 || (moves=std::move(g_MoveGenerator.generateAllMoves(gameStateCopy, player))).size() == 0)
    {
        return bestMove;
    }
    #ifdef DEBUG
    std::cout << "# of total moves for player " << player << ": " << moves.size() << std::endl;
    #endif
    bestMove = moves[0];
    struct Move lastLegalMove;
    std::vector<std::pair<struct Move, std::future<int32_t>>> results;
    for(const auto& move : moves)
    {
        if(gameStateCopy.makeMove(move.fromPos, move.toPos, false))
        {
            results.push_back(
                              std::make_pair(move, std::async(std::launch::async,
                                             std::bind(&AIEngine::negamax_inner, this,
                                                       gameStateCopy, getOtherPlayer(player),
                                                       depth-1, -beta, -alpha))));
            //auto childNodeRetValue = negamax_inner(gameStateCopy, getOtherPlayer(player), depth-1, -beta, -alpha);
            gameStateCopy.restoreState();
            gameStateCopy.m_playerToMove = player;
        }
    }
    for(auto& result : results)
    {
        auto childNodeRetValue = result.second.get();
        std::cout << player <<" player's value " << childNodeRetValue << std::endl;
        if(-childNodeRetValue > bestValue)
        {
            bestValue = -childNodeRetValue;
            if(depth == INITIAL_DEPTH)
                bestMove  = result.first;
            alpha = std::max(alpha, -childNodeRetValue);
        }
    }
    return bestMove;
}
void AIEngine::dispatcherLoop()
{
    while (!m_Exit)
    {
        if(game_Ptr)
        {
            makeMove(*game_Ptr, playerToMove);
            game_Ptr = nullptr;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}
void AIEngine::makeMove(Game& game, const Player& player)
{
    auto ret = negamax(game, player, INITIAL_DEPTH,
                        MINUS_INF,
                        PLUS_INF);
    std::lock_guard<std::mutex> lock(g_Game_mutex);
    if(!game.makeMove(ret.fromPos, ret.toPos))
    {
        game.giveUp(player);
    }
}

void AIEngine::getMove(Game& game, const Player& player)
{
    playerToMove = player;
    game_Ptr = &game;
}

const int32_t POSITION_MODIFIER[6][8][8]=
{
{
//Pawns
{ 0,  0,  0,  0,  0,  0,  0,  0},
{50, 50, 50, 50, 50, 50, 50, 50},
{10, 10, 20, 30, 30, 20, 10, 10},
{ 5,  5, 10, 25, 25, 10,  5,  5},
{ 0,  0,  0, 20, 20,  0,  0,  0},
{ 5, -5,-10,  0,  0,-10, -5,  5},
{ 5, 10, 10,-20,-20, 10, 10,  5},
{ 0,  0,  0,  0,  0,  0,  0,  0}
},

{//Rooks
 {0,  0,  0,  0,  0,  0,  0,  0},
 {5, 10, 10, 10, 10, 10, 10,  5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 {-5,  0,  0,  0,  0,  0,  0, -5},
 { 0,  0,  0,  5,  5,  0,  0,  0}
},
//Bishops
{
{-20,-10,-10,-10,-10,-10,-10,-20},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-10,  0,  5, 10, 10,  5,  0,-10},
{-10,  5,  5, 10, 10,  5,  5,-10},
{-10,  0, 10, 10, 10, 10,  0,-10},
{-10, 10, 10, 10, 10, 10, 10,-10},
{-10,  5,  0,  0,  0,  0,  5,-10},
{-20,-10,-10,-10,-10,-10,-10,-20}
},
//Knights
{
{-50,-40,-30,-30,-30,-30,-40,-50},
{-40,-20,  0,  0,  0,  0,-20,-40},
{-30,  0, 10, 15, 15, 10,  0,-30},
{-30,  5, 15, 20, 20, 15,  5,-30},
{-30,  0, 15, 20, 20, 15,  0,-30},
{-30,  5, 10, 15, 15, 10,  5,-30},
{-40,-20,  0,  5,  5,  0,-20,-40},
{-50,-40,-30,-30,-30,-30,-40,-50}
},
//Queens
{
{-20,-10,-10, -5, -5,-10,-10,-20},
{-10,  0,  0,  0,  0,  0,  0,-10},
{-10,  0,  5,  5,  5,  5,  0,-10},
{-5,  0,  5,  5,  5,  5,  0, -5},
{0,  0,  5,  5,  5,  5,  0, -5},
{-10,  5,  5,  5,  5,  5,  0,-10},
{-10,  0,  5,  0,  0,  0,  0,-10},
{-20,-10,-10, -5, -5,-10,-10,-20}
},
//Kings
{
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-30,-40,-40,-50,-50,-40,-40,-30},
{-20,-30,-30,-40,-40,-30,-30,-20},
{-10,-20,-20,-20,-20,-20,-20,-10},
{ 20, 20,  0,  0,  0,  0, 20, 20},
{ 20, 30, 10,  0,  0, 10, 30, 20}
}

};

int64_t AIEngine::evaluateBoard(const Game& game)
{
    int64_t value = 0;
    int32_t i = 0;
    for(const auto& tile : game.m_currentBoardState)
    {
        if(tile.type != Empty)
        {

            value += playerColor[tile.owner]*staticPieceValue[tile.type];
            if(tile.owner == White)
                value += playerColor[tile.owner]*POSITION_MODIFIER[tile.type][i/8][i%8];
            else
                value += playerColor[tile.owner]*POSITION_MODIFIER[tile.type][7-(i/8)][i%8];
        }
        ++i;
    }
    return value;
}
