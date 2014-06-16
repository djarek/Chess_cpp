#include "transposition_table.h"
#include "game.h"
#include <random>
#include <limits>

BitBoard TTable::randomBitStrings[2][6] = {{0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0}};

void TTable::init()
{
    std::random_device gen;
    std::uniform_int_distribution<uint64_t> losujWage(0, std::numeric_limits<uint64_t>::max());
    for(uint8_t i = 0; i < 2; ++i)
    {
        for(uint8_t j = 0; j < 6; ++j)
        {
            TTable::randomBitStrings[i][j] = losujWage(gen);
        }
    }
}

uint64_t getZobristHash(const Game& game)
{
    uint64_t ret = 0;
    for(uint8_t player = Player::White; player <= Player::Black; ++player)
    {
        for(int8_t piece = PieceType::Pawn; piece <= PieceType::King; ++piece)
        {
            ret = ret ^ (game.m_bitState.getBitBoard(static_cast<Player>(player), static_cast<PieceType>(piece))
                      ^ TTable::randomBitStrings[player][piece]);
        }
    }
    return ret;
}

void TTable::storeState(const uint64_t& hash, const int32_t& value, const int32_t& depth, const uint8_t& flag)
{
    auto pos = m_hashTable.begin() + (hash % m_hashTable.size());
    *pos = Entry(hash^static_cast<uint64_t>(value), value, depth, flag);
}

Entry TTable::getState(const uint64_t& hash) const
{
    auto pos = m_hashTable.begin() + (hash % m_hashTable.size());
    return *pos;
}
