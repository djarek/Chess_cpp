#include "moves.h"
#include "game.h"
const uint8_t ONE_ROW_UP_DOWN = 8;
const uint8_t RANK7 = 0;
const uint8_t RANK6 = 1;
const uint8_t RANK5 = 2;
const uint8_t RANK4 = 3;
const uint8_t RANK3 = 4;
const uint8_t RANK2 = 5;
const uint8_t RANK1 = 6;
const uint8_t RANK0 = 7;

const BitBoard KNIGHT_MOVES //At (2,2)
{
((uint64_t)1 << 1)+
((uint64_t)1 << 3)+
((uint64_t)1 << 8)+
((uint64_t)1 << 12)+
((uint64_t)1 << 24)+
((uint64_t)1 << 33)+
((uint64_t)1 << 35)+
((uint64_t)1 << 28)
};


BitBoard generateKnightAttacksAt(int8_t x, int8_t y)
{
    BitBoard bb = KNIGHT_MOVES;
    if(y>2)
        bb >> ONE_ROW_UP_DOWN*(y-2);
    else if(y<2)
        bb << ONE_ROW_UP_DOWN*(2-y);

    if(x > 2)
    {
        while(x > 2)
        {
            bb.safeRightShift();
            --x;
        }
    }
    else if(x < 2)
    {
        while(2-x > 0)
        {
            bb.safeLeftShift();
            ++x;
        }
    }
    return bb;
}

const BitBoard KING_MOVES //At (2,2)
{

((uint64_t)1 << 9)+
((uint64_t)1 << 10)+
((uint64_t)1 << 11)+
((uint64_t)1 << 17)+
((uint64_t)1 << 19)+
((uint64_t)1 << 25)+
((uint64_t)1 << 26)+
((uint64_t)1 << 27)
};

BitBoard generateKingAttacksAt(int8_t x, int8_t y)
{
    BitBoard bb = KING_MOVES;
    if(y>2)
        bb >> ONE_ROW_UP_DOWN*(y-2);
    else if(y<2)
        bb << ONE_ROW_UP_DOWN*(2-y);

    if(x > 2)
    {
        while(x > 2)
        {
            bb.safeRightShift();
            --x;
        }
    }
    else if(x < 2)
    {
        while(2-x > 0)
        {
            bb.safeLeftShift();
            ++x;
        }
    }
    return bb;
}

const BitBoard BLACK_PAWN_ATTACKS[8]=
{
((uint64_t)1  << 17),
((uint64_t)1  << 16) + ((uint64_t)1  << 18),
((uint64_t)1  << 17) + ((uint64_t)1  << 19),
((uint64_t)1  << 18) + ((uint64_t)1  << 20),
((uint64_t)1  << 19) + ((uint64_t)1  << 21),
((uint64_t)1  << 20) + ((uint64_t)1  << 22),
((uint64_t)1  << 21) + ((uint64_t)1  << 23),
((uint64_t)1  << 22)
};

const BitBoard WHITE_PAWN_ATTACKS[8]=
{
((uint64_t)1  << 41),
((uint64_t)1  << 40) + ((uint64_t)1  << 42),
((uint64_t)1  << 41) + ((uint64_t)1  << 43),
((uint64_t)1  << 42) + ((uint64_t)1  << 44),
((uint64_t)1  << 43) + ((uint64_t)1  << 45),
((uint64_t)1  << 44) + ((uint64_t)1  << 46),
((uint64_t)1  << 45) + ((uint64_t)1  << 47),
((uint64_t)1  << 46)
};

BitBoard generatePawnAttacksAt(const uint8_t x, const uint8_t y, const Player attackedPlayer)
{
    BitBoard ret;
    if(attackedPlayer == Player::White)
    {
        ret = WHITE_PAWN_ATTACKS[x];
        if(y < RANK5)
        {
            return BitBoard(0);
        }
        else if(y < RANK1)
            ret = ret << ONE_ROW_UP_DOWN*(6-y);
        else if(y == RANK0)
            ret = ret >> ONE_ROW_UP_DOWN;
    }
    else
    {
        ret = BLACK_PAWN_ATTACKS[x];
        if(y > RANK2)
        {
            return BitBoard(0);
        }
        else if(y > RANK6)
            ret = ret >> ONE_ROW_UP_DOWN*(y-1);
        else if(y == RANK7)
            ret = ret << ONE_ROW_UP_DOWN;
    }
    return ret;
}

void MoveGenerator::init()
{
    for (uint8_t player = Player::White; player <= Player::Black; ++player)
    {
        for(uint8_t x = 0; x < 8; ++x)
        {
            for(uint8_t y = 0; y < 8; ++y)
            {
                PAWN_ATTACKS[player][x][y] = generatePawnAttacksAt(x, y, static_cast<Player>(player));
                KNIGHT_ATTACKS[x][y] = generateKnightAttacksAt(x, y);
                KING_ATTACKS[x][y] = generateKingAttacksAt(x,y);
            }
        }
    }

}

BitBoard MoveGenerator::getPawnAttackersAt(const uint8_t x, const uint8_t y, const Player attackedPlayer)
{
    return PAWN_ATTACKS[attackedPlayer][x][y];
}

BitBoard MoveGenerator::getKnightAttackersAt(const uint8_t x, const uint8_t y)
{
    return KNIGHT_ATTACKS[x][y];
}

BitBoard MoveGenerator::getKingAttackerAt(const uint8_t x, const uint8_t y)
{
    return KING_ATTACKS[x][y];
}



BitBoard BOTTOM_RIGHT_DIAGONAL(
((uint64_t)1  <<9*0)+
((uint64_t)1 << 9*1)+
((uint64_t)1 << 9*2)+
((uint64_t)1 << 9*3)+
((uint64_t)1 << 9*4)+
((uint64_t)1 << 9*5)+
((uint64_t)1 << 9*6)+
((uint64_t)1 << 9*7));

BitBoard DOWN_COLUMN(
((uint64_t)1  <<8*0)+
((uint64_t)1 << 8*1)+
((uint64_t)1 << 8*2)+
((uint64_t)1 << 8*3)+
((uint64_t)1 << 8*4)+
((uint64_t)1 << 8*5)+
((uint64_t)1 << 8*6)+
((uint64_t)1 << 8*7));

BitBoard RIGHT_ROW(
((uint64_t)1  <<1*0)+
((uint64_t)1 << 1*1)+
((uint64_t)1 << 1*2)+
((uint64_t)1 << 1*3)+
((uint64_t)1 << 1*4)+
((uint64_t)1 << 1*5)+
((uint64_t)1 << 1*6)+
((uint64_t)1 << 1*7));


BitBoard BOTTOM_LEFT_DIAGONAL(

((uint64_t)1 << 7*1)+
((uint64_t)1 << 7*2)+
((uint64_t)1 << 7*3)+
((uint64_t)1 << 7*4)+
((uint64_t)1 << 7*5)+
((uint64_t)1 << 7*6)+
((uint64_t)1 << 7*7)+
((uint64_t)1 << 7*8)
);

const int8_t knightDeltaX[] = {-2, -2, -1, -1, 1,  1,  2, 2};
const int8_t knightDeltaY[] = {-1,  1, -2,  2, 2, -2, -1, 1};

void MoveGenerator::generateKnightMoves(const Game& game, const Player& player, MoveVec& result) const
{
    std::vector<Position> knights;
    for(uint8_t x = 0; x < 8; ++x)
    {
        for(uint8_t y = 0; y < 8; ++y)
        {
            const auto& piece = game.getPiece(x,y);
            if(piece.type == PieceType::Knight && piece.owner == player)
                knights.push_back(Position(x, y));
        }
    }
    for(const auto& knightPos : knights)
    {
        int8_t newX, newY;
        for(uint8_t i = 0; i < 8; ++i)
        {
            newX = knightPos.x+knightDeltaX[i];
            newY = knightPos.y+knightDeltaY[i];
            if(newX < 8 && newX >= 0 && newY < 8 && newY >= 0)
            {
                auto& targetPiece = game.getPiece(newX, newY);
                if(targetPiece.owner != player)
                {
                    struct Move newMove;
                    newMove.fromPos = knightPos;
                    newMove.toPos = Position(newX, newY);
                    if(targetPiece.type != Empty)
                        newMove.type = MoveType::Capture;
                    else
                        newMove.type = MoveType::Move;
                    result.push_back(newMove);
                }
            }
        }
    }
}

/*
(-1, -1)(0, -1)(1, -1)
(-1, 0) (King) (1, 0)
(-1, 1) (0, 1) (1, 1)*/
const int8_t kingDeltaX[] = {0,  0, 1, 1,  1, -1, -1, -1};
const int8_t kingDeltaY[] = {1, -1, 0, 1, -1, -1,  0,  1};

void MoveGenerator::generateKingMoves(const Game& game, const Player& player, MoveVec& result) const
{
    Position kingPos = game.m_KingPos[player];
    int8_t newX, newY;
    for(uint8_t i = 0; i < 8; ++i)
    {
        newX = kingPos.x+kingDeltaX[i];
        newY = kingPos.y+kingDeltaY[i];
        if(newX < 8 && newX >= 0 && newY < 8 && newY >= 0)
        {
            auto& targetPiece = game.getPiece(newX, newY);
            if(targetPiece.owner != player)
            {
                struct Move newMove;
                newMove.fromPos = kingPos;
                newMove.toPos = Position(newX, newY);
                if(targetPiece.type != Empty)
                    newMove.type = MoveType::Capture;
                else
                    newMove.type = MoveType::Move;
                result.push_back(newMove);
            }
        }
    }
    //castling
    if(!game.getPiece(kingPos).hasMoved )
    {
        auto& kingSideRook = game.getPiece(7, kingPos.y);
        auto& queenSideRook = game.getPiece(0, kingPos.y);
        if(!kingSideRook.hasMoved && game.getPiece(kingPos.x+1, kingPos.y).type == Empty)
        {
            struct Move newMove;
            newMove.fromPos = kingPos;
            newMove.toPos   = Position(kingPos.x+2, kingPos.y);
            newMove.type    = MoveType::KingSideCastling;
            result.push_back(newMove);
        }
        if(!queenSideRook.hasMoved && game.getPiece(kingPos.x-1, kingPos.y).type == Empty)
        {
            struct Move newMove;
            newMove.fromPos = kingPos;
            newMove.toPos   = Position(kingPos.x-2, kingPos.y);
            newMove.type    = MoveType::QueenSideCastling;
            result.push_back(newMove);
        }
    }
}

void MoveGenerator::generatePawnMoves(const Game& game, const Player& player, MoveVec& result) const
{
    std::vector<Position> pawns;
    for(uint8_t x = 0; x < 8; ++x)
    {
        for(uint8_t y = 0; y < 8; ++y)
        {
            const auto& piece = game.getPiece(x,y);
            if((piece.type == PieceType::Pawn) && piece.owner == player)
                pawns.push_back(Position(x, y));
        }
    }
    int8_t colorModifier = 1;
    if(player == Player::White)
    {
        colorModifier = -1;
    }
    for(const auto& pawnPos : pawns)
    {
        struct Move newMove;
        newMove.fromPos = pawnPos;
        //1 tile pushes
        if(game.getPiece(pawnPos.x, pawnPos.y+colorModifier).type == PieceType::Empty)
        {
            newMove.toPos = Position(pawnPos.x, pawnPos.y+colorModifier);
            newMove.type  = MoveType::Move;
            result.push_back(newMove);
        }
        //normal pawn captures
        if(pawnPos.x-1 >= 0 && game.getPiece(pawnPos.x-1, pawnPos.y+colorModifier).type != PieceType::Empty)
        {
            newMove.toPos = Position(pawnPos.x-1, pawnPos.y+colorModifier);
            newMove.type  = MoveType::Capture;
            result.push_back(newMove);
        }
        if(pawnPos.x+1 < 8 && game.getPiece(pawnPos.x+1, pawnPos.y+colorModifier).type != PieceType::Empty)
        {
            newMove.toPos = Position(pawnPos.x+1, pawnPos.y+colorModifier);
            newMove.type  = MoveType::Capture;
            result.push_back(newMove);
        }

        //pawn rush
        if(!game.getPiece(pawnPos).hasMoved
           && game.getPiece(pawnPos.x, pawnPos.y+colorModifier).type == PieceType::Empty
           && game.getPiece(pawnPos.x, pawnPos.y+2*colorModifier).type == PieceType::Empty)
        {
            newMove.toPos = Position(pawnPos.x, pawnPos.y+2*colorModifier);
            newMove.type  = MoveType::PawnRush;
            result.push_back(newMove);
        }

        //EnPassant capture
        if(game.m_lastPawnRush.y == pawnPos.y && std::abs(game.m_lastPawnRush.x-pawnPos.x) == 1)
        {
            newMove.toPos = Position(game.m_lastPawnRush.x, game.m_lastPawnRush.y + colorModifier);
            newMove.type  = MoveType::EnPassant;
            result.push_back(newMove);
        }
    }
}

void MoveGenerator::generateRookSliderMoves(const Game& game, const Player& player, MoveVec& result) const
{
    std::vector<Position> rooks;
    for(uint8_t x = 0; x < 8; ++x)
    {
        for(uint8_t y = 0; y < 8; ++y)
        {
            const auto& piece = game.getPiece(x,y);
            if((piece.type == PieceType::Rook || piece.type == PieceType::Queen) && piece.owner == player)
                rooks.push_back(Position(x, y));
        }
    }
    for(const auto& rookPos : rooks)
    {
        for(int8_t y = rookPos.y-1; y >= 0; --y)
        {
            struct Move newMove;
            newMove.fromPos = rookPos;
            newMove.toPos.x = rookPos.x;
            newMove.toPos.y = y;
            const auto& piece = game.getPiece(rookPos.x, y);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }

        for(int8_t y = rookPos.y-1; y >= 0; --y)
        {
            struct Move newMove;
            newMove.fromPos = rookPos;
            newMove.toPos.x = rookPos.x;
            newMove.toPos.y = y;
            const auto& piece = game.getPiece(rookPos.x, y);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }

        for(int8_t x = rookPos.x-1; x >= 0; --x)
        {
            struct Move newMove;
            newMove.fromPos = rookPos;
            newMove.toPos.x = x;
            newMove.toPos.y = rookPos.y;
            const auto& piece = game.getPiece(x, rookPos.y);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }

        for(int8_t x = rookPos.x+1; x < 8; ++x)
        {
            struct Move newMove;
            newMove.fromPos = rookPos;
            newMove.toPos.x = x;
            newMove.toPos.y = rookPos.y;
            const auto& piece = game.getPiece(x, rookPos.y);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }
    }
}

void MoveGenerator::generateBishopSliderMoves(const Game& game, const Player& player, MoveVec& result) const
{
    std::vector<Position> bishops;
    for(uint8_t x = 0; x < 8; ++x)
    {
        for(uint8_t y = 0; y < 8; ++y)
        {
            const auto& piece = game.getPiece(x,y);
            if((piece.type == PieceType::Bishop || piece.type == PieceType::Queen) && piece.owner == player)
                bishops.push_back(Position(x, y));
        }
    }
    for(const auto& bishopPos : bishops)
    {
        int8_t moveNum = numberOfMovesMainDiagonalUp[bishopPos.x][bishopPos.y];
        for(int8_t i = 1; i <= moveNum; ++i)
        {
            struct Move newMove;
            newMove.fromPos = bishopPos;
            newMove.toPos.x = bishopPos.x-i;
            newMove.toPos.y = bishopPos.y-i;
            const auto& piece = game.getPiece(bishopPos.x-i, bishopPos.y-i);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }

        moveNum = numberOfMovesMainDiagonalDown[bishopPos.x][bishopPos.y];
        for(int8_t i = 1; i <= moveNum; ++i)
        {
            struct Move newMove;
            newMove.fromPos = bishopPos;
            newMove.toPos.x = bishopPos.x+i;
            newMove.toPos.y = bishopPos.y+i;
            const auto& piece = game.getPiece(bishopPos.x+i, bishopPos.y+i);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }

        moveNum = numberOfMovesAntiDiagonalUp[bishopPos.x][bishopPos.y];
        for(int8_t i = 1; i <= moveNum; ++i)
        {
            struct Move newMove;
            newMove.fromPos = bishopPos;
            newMove.toPos.x = bishopPos.x+i;
            newMove.toPos.y = bishopPos.y-i;
            const auto& piece = game.getPiece(bishopPos.x+i, bishopPos.y-i);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }

        moveNum = numberOfMovesAntiDiagonalDown[bishopPos.x][bishopPos.y];
        for(int8_t i = 1; i <= moveNum; ++i)
        {
            struct Move newMove;
            newMove.fromPos = bishopPos;
            newMove.toPos.x = bishopPos.x-i;
            newMove.toPos.y = bishopPos.y+i;
            const auto& piece = game.getPiece(bishopPos.x-i, bishopPos.y+i);
            if(piece.type != PieceType::Empty)
            {
                if(piece.owner != player)
                {
                    newMove.type = Capture;
                    result.push_back(newMove);
                }
                break;
            }
            newMove.type = Move;
            result.push_back(newMove);
        }
    }
}

MoveVec MoveGenerator::generateAllMoves(const Game& game, const Player& player) const
{
    MoveVec result;
    generatePawnMoves(game, player, result);
    generateKingMoves(game, player, result);
    generateKnightMoves(game, player, result);
    generateBishopSliderMoves(game, player, result);
    generateRookSliderMoves(game, player, result);
    return std::move(result);
}
