#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <vector>
#include <string>
#include "bitboard.h"
#include "moves.h"
#include "enums.h"

uint8_t getVectorIndex(const uint8_t x, const uint8_t y);

const std::string PIECE_NAMES[]={"pawn", "rook", "knight", "bishop", "queen", "king"};

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
    BitBoard& getBitBoard(const Player owner, const PieceType type)
    {
        return boards[owner][type];
    }
};

Player getOtherPlayer(const Player otherPlayer);

class Game
{
public:
    friend class MoveGenerator;
    Game();
    ~Game();
    void resetGame();
    bool isMoveLegal(const Position& source, const Position& destination, MoveType& move);
    bool makeMove(Position& source, Position& destination);
    bool spawnPiece(const PieceType type, const Position pos, const Player owner);
    CheckType isKingChecked(const Position& kingPos, const Player owner);
    Position findPieceAtColumnRay(const Position& pos, bool up);
    Piece& getPiece(const uint8_t x, const uint8_t y)
    {
        #ifdef DEBUG
        if (x > 8 || y > 8)
            throw;
        #endif
        return m_currentBoardState[getVectorIndex(x, y)];

    }

    const Piece& getPiece(const uint8_t x, const uint8_t y) const
    {
        #ifdef DEBUG
        if (x > 8 || y > 8)
            throw;
        #endif
        return m_currentBoardState[getVectorIndex(x, y)];

    }

    Piece& getPiece(const Position pos)
    {
        #ifdef DEBUG
        if (pos.x > 8 || pos.y > 8)
            throw;
        #endif
        return m_currentBoardState[getVectorIndex(pos.x, pos.y)];
    }

    const Piece& getPiece(const Position pos) const
    {
        #ifdef DEBUG
        if (pos.x > 8 || pos.y > 8)
            throw;
        #endif
        return m_currentBoardState[getVectorIndex(pos.x, pos.y)];
    }

    Player whoMoves() const {return m_playerToMove;}
    Position findPieceAtRowRay(const Position& pos, bool left);
    Position findPieceAtMainDiagonalRay(const Position& pos, bool up);
    Position findPieceAtAntiDiagonalRay(const Position& pos, bool up);
    void backupState();
    void restoreState();
    void clearState();
    void copyState(Piece* source, Piece* destination){std::copy(source, source+63, destination);}
    bool isMated(CheckType check, Player player);
private:
    Piece m_previousBoardState[64];
    Position m_previouslastPawnRush;
    GameBitState m_previousbitState;
    Position m_previousKingPos[2];

    Piece m_currentBoardState[64];
    Position m_lastPawnRush;
    GameBitState m_bitState;
    Position m_KingPos[2];
    Player m_winner = Player::None;
    Player m_playerToMove = Player::White;
};


#endif // GAME_H_INCLUDED
