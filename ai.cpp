#include "ai.h"
#include "globals.h"
#include <limits>

const int32_t INITIAL_DEPTH = 6;
const int32_t PLUS_INF = std::numeric_limits<int32_t>::max();
const int32_t MINUS_INF = std::numeric_limits<int32_t>::min()+1;

int32_t AIEngine::negamax_inner(Game& game, const Player& player, int32_t depth, int32_t alpha, int32_t beta)
{
    MoveVec moves;
    int32_t bestValue = MINUS_INF;
    Game gameStateCopy = game;
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
            gameStateCopy.restoreState();
            gameStateCopy.m_playerToMove = player;
            if(-childNodeRetValue > bestValue)
            {
                bestValue = -childNodeRetValue;
                alpha = std::max(alpha, -childNodeRetValue);
                if(alpha >= beta)
                    break;
            }
        }
    }
    return bestValue;
}

struct Move AIEngine::negamax(Game& game, const Player& player, int32_t depth, int32_t alpha, int32_t beta)
{
    MoveVec moves;
    int32_t bestValue = MINUS_INF;
    Game gameStateCopy = game;
    struct Move bestMove;
    bestMove.type = MoveType::NoMove;
    if(depth <= 0 || (moves=std::move(g_MoveGenerator.generateAllMoves(gameStateCopy, player))).size() == 0)
    {
        return bestMove;
    }
    /*#ifdef DEBUG_LOG
    std::cout << "# of total moves for player " << player << ": " << moves.size() << std::endl;
    #endif*/
    bestMove = moves[0];
    for(const auto& move : moves)
    {
        if(gameStateCopy.makeMove(move.fromPos, move.toPos, false))
        {
            auto childNodeRetValue = negamax_inner(gameStateCopy, getOtherPlayer(player), depth-1, -beta, -alpha);
            gameStateCopy.restoreState();
            gameStateCopy.m_playerToMove = player;
            if(-childNodeRetValue > bestValue)
            {
                bestValue = -childNodeRetValue;
                if(depth == INITIAL_DEPTH)
                    bestMove  = move;
                alpha = std::max(alpha, -childNodeRetValue);
                if(alpha >= beta)
                    break;
            }
        }
    }
    return bestMove;
}

void AIEngine::makeMove(Game& game, const Player& player)
{
    auto ret = negamax (game, player, INITIAL_DEPTH,
                        MINUS_INF,
                        PLUS_INF);
    if(!game.makeMove(ret.fromPos, ret.toPos))
    {
        game.giveUp(player);
    }
}

int64_t AIEngine::evaluateBoard(const Game& game)
{
    int64_t value = 0;

    for(const auto& tile : game.m_currentBoardState)
    {
        if(tile.type != Empty)
        {
            value += staticPieceValue[tile.type]*playerColor[tile.owner];
        }
    }
    return value;
}
