#include "bitboard.h"

uint64_t BitBoard::bitMasks[8][8];

const BitBoard AColumn
(
"10000000"
"10000000"
"10000000"
"10000000"
"10000000"
"10000000"
"10000000"
"10000000"
);

const BitBoard HColumn
(
"00000001"
"00000001"
"00000001"
"00000001"
"00000001"
"00000001"
"00000001"
"00000001"
);

BitBoard::BitBoard(uint64_t board)
    : m_board(board)
{

}

void setMasks()
{
    for(int i = 0; i < 8; ++i)
    {
        for(int j = 0; j < 8; ++j)
        {
            BitBoard::bitMasks[i][j] = (uint64_t)1 << (i+8*j);
        }
    }
}
BitBoard::BitBoard(const std::string board_string)
    : m_board((uint64_t)0)
{
    //std::cout << std::endl << board_string << std::endl;
    for(uint8_t i = 0; i < 64; ++i)
    {
        m_board = m_board << 1;
        if(board_string[i] == '1')
        {
            m_board |= (uint64_t)1;
        }
    }
}

BitBoard::BitBoard()
        : m_board((uint64_t)0)
{

}

uint64_t BitBoard::operator>>(uint8_t n)
{
    m_board = m_board << n;
    return m_board;
}

uint64_t BitBoard::operator<<(uint8_t n)
{
    m_board = m_board >> n;
    return m_board;
}

uint64_t BitBoard::safeRightShift()
{
    m_board &= ~AColumn.m_board;
    m_board = m_board << 1;
    return m_board;
}

uint64_t BitBoard::safeLeftShift()
{
    m_board &= ~HColumn.m_board;
    m_board = m_board >> 1;
    return m_board;
}

/*
void printBitBoard(BitBoard& bb)
{
    std::cout << std::endl;
    int j = 0;
    for(int i = 0; i < 64; ++i)
    {
        std::cout << (bool)(bb.getBoard() & (uint64_t) 1<< i);
        if(++j == 8){
            std::cout << std::endl;
            j=0;
        }
    }
    std::cout << std::endl;
}*/
