#include "ai.h"
#include "globals.h"
#include <limits>

const int32_t INITIAL_DEPTH = 6;
const int64_t PLUS_INF = std::numeric_limits<int32_t>::max()-1;
const int64_t MINUS_INF = std::numeric_limits<int32_t>::min();

std::pair<int64_t, struct Move> AIEngine::negamax(Game& game, const Player& player, int32_t depth, int64_t alpha, int64_t beta)
{
    MoveVec moves;
    int64_t bestValue = MINUS_INF;
    Game gameStateCopy = game;
    struct Move bestMove;
    if(depth <= 0 || (moves=std::move(g_MoveGenerator.generateAllMoves(gameStateCopy, player))).size() == 0)
    {
        return std::make_pair(playerColor[player]*evaluateBoard(gameStateCopy, player), bestMove);
    }
    /*#ifdef DEBUG_LOG
    std::cout << "# of total moves for player " << player << ": " << moves.size() << std::endl;
    #endif*/
    bestMove = moves[0];
    for(const auto& move : moves)
    {
        if(gameStateCopy.makeMove(move.fromPos, move.toPos, false))
        {
            auto childNodeRetValue = negamax(gameStateCopy, getOtherPlayer(player), depth-1, -beta, -alpha);
            gameStateCopy.restoreState();
            gameStateCopy.m_playerToMove = player;
            if(-childNodeRetValue.first > bestValue)
            {
                bestValue = -childNodeRetValue.first;
                if(depth == INITIAL_DEPTH)
                    bestMove  = move;
                alpha = std::max(alpha, -childNodeRetValue.first);
                if(alpha >= beta)
                    break;
            }
        }
    }
    return std::make_pair(bestValue, bestMove);
}

void AIEngine::makeMove(Game& game, const Player& player)
{
    auto ret = negamax (game, player, INITIAL_DEPTH,
                        MINUS_INF,
                        PLUS_INF);
    if(!game.makeMove(ret.second.fromPos, ret.second.toPos))
    {
        game.giveUp(player);
    }
}

int64_t AIEngine::evaluateBoard(const Game& game, const Player& player)
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
