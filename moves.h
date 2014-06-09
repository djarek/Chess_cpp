#ifndef MOVES_H_INCLUDED
#define MOVES_H_INCLUDED
#include "enums.h"
#include "bitboard.h"
#include <vector>

class Game;

void test();
BitBoard getPawnAttackersAt(const uint8_t x, const uint8_t y, const Player attackedPlayer);
BitBoard getKingAttackerAt(const uint8_t x, const uint8_t y);
BitBoard getKnightAttackersAt(const uint8_t x, const uint8_t y);
BitBoard getBottomRightPossibleMoves(uint8_t x, uint8_t y, uint8_t n = 8);
BitBoard getBottomLeftPossibleMoves(uint8_t x, uint8_t y, uint8_t n = 8);

const uint16_t fromPosMask = 7;
const uint16_t toPosMask = 56;

struct Move
{
    Move(){}
    Move(const Position fromPos, const Position toPos, const MoveType type)
        :fromPos(fromPos)
        ,toPos(toPos)
        ,type(type)
    {}
    Position fromPos, toPos;
    MoveType type;
};

const uint8_t numberOfMovesMainDiagonalUp[8][8]=
{
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 1, 1, 1, 1, 1, 1},
{0, 1, 2, 2, 2, 2, 2, 2},
{0, 1, 2, 3, 3, 3, 3, 3},
{0, 1, 2, 3, 4, 4, 4, 4},
{0, 1, 2, 3, 4, 5, 5, 5},
{0, 1, 2, 3, 4, 5, 6, 6},
{0, 1, 2, 3, 4, 5, 6, 7},

};

const uint8_t numberOfMovesMainDiagonalDown[8][8]=
{
{7, 6, 5, 4, 3, 2, 1, 0},
{6, 6, 5, 4, 3, 2, 1, 0},
{5, 5, 5, 4, 3, 2, 1, 0},
{4, 4, 4, 4, 3, 2, 1, 0},
{3, 3, 3, 3, 3, 2, 1, 0},
{2, 2, 2, 2, 2, 2, 1, 0},
{1, 1, 1, 1, 1, 1, 1, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
};

//# of moves on the anitdiagonal, transposition of the "Up" array is equal to the "Down" one
const uint8_t numberOfMovesAntiDiagonalUp[8][8]=
{
{0, 1, 2, 3, 4, 5, 6, 7},
{0, 1, 2, 3, 4, 5, 6, 6},
{0, 1, 2, 3, 4, 5, 5, 5},
{0, 1, 2, 3, 4, 4, 4, 4},
{0, 1, 2, 3, 3, 3, 3, 3},
{0, 1, 2, 2, 2, 2, 2, 2},
{0, 1, 1, 1, 1, 1, 1, 1},
{0, 0, 0, 0, 0, 0, 0, 0}
};

const uint8_t numberOfMovesAntiDiagonalDown[8][8]=
{
{0, 0, 0, 0, 0, 0, 0, 0},
{1, 1, 1, 1, 1, 1, 1, 0},
{2, 2, 2, 2, 2, 2, 1, 0},
{3, 3, 3, 3, 3, 2, 1, 0},
{4, 4, 4, 4, 3, 2, 1, 0},
{5, 5, 5, 4, 3, 2, 1, 0},
{6, 6, 5, 4, 3, 2, 1, 0},
{7, 6, 5, 4, 3, 2, 1, 0},
};

typedef std::vector<struct Move> MoveVec;
class MoveGenerator
{
public:
    void init();
    void generateKingMoves(const Game& game, const Player& player, MoveVec& result) const;
    void generateKnightMoves(const Game& game, const Player& player, MoveVec& result) const;
    void generateRookSliderMoves(const Game& game, const Player& player, MoveVec& result) const;
    void generateBishopSliderMoves(const Game& game, const Player& player, MoveVec& result) const;
    void generatePawnMoves(const Game& game, const Player& player, MoveVec& result) const;
    MoveVec generateAllMoves(const Game& game, const Player& player) const;
    BitBoard getKnightAttackersAt(const uint8_t x, const uint8_t y);
    BitBoard getPawnAttackersAt(const uint8_t x, const uint8_t y, const Player attackedPlayer);
    BitBoard getKingAttackerAt(const uint8_t x, const uint8_t y);
private:
    BitBoard PAWN_ATTACKS[2][8][8];
    BitBoard KNIGHT_ATTACKS[8][8];
    BitBoard KING_ATTACKS[8][8];
};

#endif // MOVES_H_INCLUDED
