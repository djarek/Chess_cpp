#ifndef BITBOARD_H_INCLUDED
#define BITBOARD_H_INCLUDED
#include <string>

class BitBoard
{
public:
    BitBoard(uint64_t board);
    BitBoard(const std::string board_string);
    BitBoard();
    ~BitBoard(){}
    bool isMovePossible(const BitBoard& destination) const
    {
        return m_board xor destination.m_board;
    }
    uint64_t& getBoard() {return m_board;}
    uint64_t operator<<(uint8_t n);
    uint64_t operator>>(uint8_t n);
    uint64_t operator^(const uint64_t& other)const {return m_board ^ other;}
    uint64_t operator^(const BitBoard& other)const {return m_board ^ other.m_board;}
    uint64_t safeLeftShift();
    uint64_t safeRightShift();
    uint64_t operator[](uint8_t i){return m_board & ((uint64_t)1 << i);}
    void setBitTrue(uint8_t x, uint8_t y)
    {
        m_board |= bitMasks[x][y];
    }
    void setBitFalse(uint8_t x, uint8_t y)
    {
        m_board &= ~bitMasks[x][y];
    }

    static uint64_t bitMasks[8][8];
    BitBoard operator&(const BitBoard& right) const
    {
        return m_board & right.m_board;
    }

    BitBoard operator|(const BitBoard& right) const
    {
        return m_board | right.m_board;
    }
private:
    uint64_t m_board;

};
void setMasks();
void printBitBoard(BitBoard& bb);

#endif // BITBOARD_H_INCLUDED

