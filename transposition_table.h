#ifndef TRANSPOSITION_TABLE_H_INCLUDED
#define TRANSPOSITION_TABLE_H_INCLUDED

#include "bitboard.h"
#include <vector>

class Game;
const uint8_t EXACT = 0;
const uint8_t LOWERBOUND = 1;
const uint8_t UPPERBOUND = 2;
const uint8_t INVALID = 3;

struct Entry
{
    Entry()
        :hash(0), depth(0), flag(INVALID){}
    Entry(const uint64_t hash, const int32_t value, const int32_t depth, const uint8_t flag)
        :value(value), depth(depth), flag(flag){this->hash = hash;}
    uint64_t hash;
    int32_t value;
    int32_t depth;
    uint8_t flag;
};


typedef std::vector<Entry> HashVec;

class TTable
{
public:
    TTable(){m_hashTable.resize(1000000, Entry());}
    ~TTable(){}
    static void init();
    size_t size(){return m_hashTable.size();}
    static BitBoard randomBitStrings[2][6];
    void storeState(const uint64_t& hash, const int32_t& value, const int32_t& depth, const uint8_t& flag);
    Entry getState(const uint64_t& hash) const;
private:
    HashVec m_hashTable;
};

uint64_t getZobristHash(const Game& game);

#endif // TRANSPOSITION_TABLE_H_INCLUDED
