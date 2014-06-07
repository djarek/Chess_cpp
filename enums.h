#ifndef ENUMS_H_INCLUDED
#define ENUMS_H_INCLUDED
#include <cstdint>
struct Position
{
    Position(uint8_t x, uint8_t y)
        :x(x)
        ,y(y) {}
    Position()
        :x(0)
        ,y(0) {}
    uint8_t x, y;
    uint8_t getVectorIndex() const {return 8*y+x;}
};
bool operator!=(const Position& first, const Position& second);

enum PieceType
{
    Empty  = -1,
    Pawn   = 0,
    Rook   = 1,
    Knight = 2,
    Bishop = 3,
    Queen  = 4,
    King   = 5
};

enum Player
{
    None  = -1,
    White = 0,
    Black = 1
};

enum CheckType
{
    NoCheck = 0,
    SingleCheck = 1,
    DoubleCheck = 2
};

enum MoveType
{
    NoMove    = 0,
    Move      = 1,
    PawnRush  = 2,
    Capture   = 3,
    EnPassant = 4,
    KingSideCastling  = 5,
    QueenSideCastling  = 6
};
#endif // ENUMS_H_INCLUDED
