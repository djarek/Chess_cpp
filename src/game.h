#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <string>
#include "bitboard.h"
#include "moves.h"
#include "enums.h"
#include "ai.h"
#include <iostream>
#include <mutex>

uint8_t getVectorIndex(const uint8_t x, const uint8_t y);

const std::string PIECE_NAMES[]={"pawn", "rook", "knight", "bishop", "queen", "king"};
const std::string PLAYER_NAMES[]={"White", "Black"};
struct Piece
{
    Piece()
        :type(PieceType::Empty)
        ,owner(Player::None) {}
    Piece(PieceType t, Player p)
        :type(t)
        ,owner(p) {}
    PieceType type;
    Player owner;
    bool hasMoved = false;
};

struct GameBitState
{
    BitBoard boards[2][6];
    BitBoard& getBitBoard(const Piece piece)
    {
        return boards[piece.owner][piece.type];
    }
    const BitBoard& getBitBoard(const Piece piece) const
    {
        return boards[piece.owner][piece.type];
    }
    BitBoard& getBitBoard(const Player owner, const PieceType type)
    {
        return boards[owner][type];
    }

    const BitBoard& getBitBoard(const Player owner, const PieceType type) const
    {
        return boards[owner][type];
    }
};

enum State
{
    whiteWon   = 0,
    blackWon   = 1,
    draw       = 2,
    inProgress = 3,
    whiteGaveUp= 4,
    blackGaveUp= 5
};

Player getOtherPlayer(const Player otherPlayer);
class AIEngine;
extern AIEngine AIPlayers[2];

class Game
{
public:
    friend class MoveGenerator;
    friend class AIEngine;
    friend uint64_t getZobristHash(const Game& game);
    Game();
    Game(const Game& other);
    ~Game();
    void resetGame();
    bool isMoveLegal(const Position& source, const Position& destination, MoveType& move);
    bool makeMove(const Position& source, const Position& destination, const bool mateCheck = true);
    bool spawnPiece(const PieceType type, const Position pos, const Player owner);
    void giveUp(const Player& player) {m_gameStatus = (player == Player::White) ? whiteGaveUp: blackGaveUp; std::cout << "Player" << PLAYER_NAMES[player] << "gave up." << std::endl;}
    CheckType isKingChecked(const Position& kingPos, const Player owner);
    Position findPieceAtColumnRay(const Position& pos, bool up) const;
    Piece& getPiece(const uint8_t x, const uint8_t y)
    {
        #ifdef DEBUG_LOG
        if (x > 8 || y > 8)
        {
            std::cout << "GetPiece() at "<< (uint32_t)x << " " << (uint32_t)y << std::endl;
            throw;
        }
        #endif
        return m_currentBoardState[getVectorIndex(x, y)];
    }

    const Piece& getPiece(const uint8_t x, const uint8_t y) const
    {
        #ifdef DEBUG_LOG
        if (x > 8 || y > 8)
        {
            std::cout << "GetPiece() at "<< (uint32_t)x << " " << (uint32_t)y << std::endl;
            throw;
        }
        #endif
        return m_currentBoardState[getVectorIndex(x, y)];

    }

    Piece& getPiece(const Position pos)
    {
        #ifdef DEBUG
        if (pos.x > 8 || pos.y > 8)
        {
            std::cout << "GetPiece() at "<< (uint32_t)pos.x << " " << (uint32_t)pos.y << std::endl;
            throw;
        }
        #endif
        return m_currentBoardState[getVectorIndex(pos.x, pos.y)];
    }

    const Piece& getPiece(const Position pos) const
    {
        #ifdef DEBUG
        if (pos.x > 8 || pos.y > 8)
        {
            std::cout << "GetPiece() at "<< (uint32_t)pos.x << " " << (uint32_t)pos.y << std::endl;
            throw;
        }
        #endif
        return m_currentBoardState[getVectorIndex(pos.x, pos.y)];
    }

    Player whoMoves() const {return m_playerToMove;}
    Position findPieceAtRowRay(const Position& pos, const bool left) const ;
    Position findPieceAtMainDiagonalRay(const Position& pos, const bool up) const;
    Position findPieceAtAntiDiagonalRay(const Position& pos, const bool up) const;
    void backupState();
    void restoreState();
    void clearState();
    void copyState(Piece* source, Piece* destination){std::copy(source, source+64, destination);}
    bool isMated(const CheckType check, const Player player);
    State getGameStatus() const {return m_gameStatus;}
    Game& operator=(const Game& other);
    bool makeMove(const struct Move move);
    bool isCurrentPlayerAI(){return m_players[m_playerToMove] == AI;}
private:
    Piece m_previousBoardState[64];
    Position m_previouslastPawnRush;
    GameBitState m_previousbitState;
    Position m_previousKingPos[2];
    State m_previousgameStatus;

    Piece m_currentBoardState[64];
    Position m_lastPawnRush;
    GameBitState m_bitState;
    Position m_KingPos[2];

    State m_gameStatus;

    Player m_playerToMove = Player::White;

    PlayerType m_players[2] = {AI, AI};
};


#endif // GAME_H_INCLUDED
