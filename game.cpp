#include "game.h"
#include <iostream>
#include "globals.h"
bool operator!=(const Position& first, const Position& second)
{
    return first.x != second.x || second.y != first.y;
}

Game::Game()
{
    setMasks();
    resetGame();
}

Game::~Game()
{

}
void Game::clearState()
{
    for(int i = 0; i < 64; ++i)
        m_currentBoardState[i] = Piece();
}
void Game::resetGame()
{
    clearState();
    m_lastPawnRush = Position();
    m_bitState = GameBitState();
    //spawn pawns
    for(uint8_t i = 0; i < 8; ++i)
    {
        spawnPiece(PieceType::Pawn, Position(i, 1), Player::Black);
        spawnPiece(PieceType::Pawn, Position(i, 6), Player::White);
    }
    spawnPiece(PieceType::Rook, Position(0, 0), Player::Black);
    spawnPiece(PieceType::Rook, Position(7, 0), Player::Black);

    spawnPiece(PieceType::Rook, Position(0, 7), Player::White);
    spawnPiece(PieceType::Rook, Position(7, 7), Player::White);

    spawnPiece(PieceType::Knight, Position(1, 0), Player::Black);
    spawnPiece(PieceType::Knight, Position(6, 0), Player::Black);

    spawnPiece(PieceType::Knight, Position(1, 7), Player::White);
    spawnPiece(PieceType::Knight, Position(6, 7), Player::White);

    spawnPiece(PieceType::Bishop, Position(2, 0), Player::Black);
    spawnPiece(PieceType::Bishop, Position(5, 0), Player::Black);

    spawnPiece(PieceType::Bishop, Position(2, 7), Player::White);
    spawnPiece(PieceType::Bishop, Position(5, 7), Player::White);

    spawnPiece(PieceType::Queen, Position(3, 0), Player::Black);
    spawnPiece(PieceType::King, Position(4, 0), Player::Black);

    spawnPiece(PieceType::Queen, Position(3, 7), Player::White);
    spawnPiece(PieceType::King, Position(4, 7), Player::White);

    printBitBoard(m_bitState.getBitBoard(Player::Black, PieceType::Pawn));
    Position p(7, 6);
        auto ret = findPieceAtColumnRay(p, false);
        std::cout << "ret"<< (int32_t)ret.x << (int32_t)ret.y << std::endl;
}

bool Game::isMoveLegal(const Position& source, const Position& destination, MoveType& move)
{
    auto piece = getPiece(source);
    move = MoveType::Move;
    auto targetPiece = getPiece(destination);
    int8_t deltaX = destination.x - source.x;
    int8_t deltaY = destination.y - source.y;
    if(targetPiece.type != PieceType::Empty)
    {
        if(targetPiece.owner != piece.owner)
            move = MoveType::Capture;
        else
            return false;
    }
    int8_t playerSign = -1;
    if(piece.owner == Player::Black)
        playerSign = 1;
    switch(piece.type)
    {
        case PieceType::Pawn:
            if(deltaY == 1*playerSign)
            {
                if((deltaX == 1 || deltaX == -1))
                {
                    if(targetPiece.type != PieceType::Empty)
                        return true;
                    if(destination.x == m_lastPawnRush.x && source.y == m_lastPawnRush.y)
                    {
                        move = MoveType::EnPassant;
                        return true;
                    }
                    else
                        return false;
                }
                else if(deltaX == 0)
                    return true;
            }
            else if(deltaY == 2*playerSign && !piece.hasMoved && deltaX == 0 && getPiece(source.x, source.y+1*playerSign).type == PieceType::Empty)
            {
                m_lastPawnRush = destination;
                move = MoveType::PawnRush;
                return true;
            }
        break;
        case PieceType::Rook:
            if(deltaX == 0 && deltaY != 0)
            {
                auto maxY = std::max(source.y, destination.y);
                for(int8_t i = std::min(source.y, destination.y)+1; i < maxY; ++i)
                {
                    if(getPiece(source.x, i).type != PieceType::Empty )
                        return false;
                }
                return true;
            }
            else if(deltaX != 0 && deltaY == 0)
            {
                auto maxX = std::max(source.x, destination.x);
                for(int8_t i = std::min(source.x, destination.x)+1; i < maxX; ++i)
                {
                    if(getPiece(i, source.y).type != PieceType::Empty)
                        return false;
                }
                return true;
            }
            else
                return false;
        break;
        case PieceType::Bishop:
            if(deltaX/deltaY == 1)
            {
                for(int8_t i = 1; i < std::max(deltaX, deltaY); ++i)
                {
                    auto p = getPiece(source.x+i, source.y+i);

                    if(p.type != PieceType::Empty)
                        return false;
                    ++i;
                }
                return true;
            }
            else if(deltaX/deltaY == -1)
            {
                for(int8_t i = 1; i < std::min(deltaX, deltaY); --i)
                {
                    auto p = getPiece(source.x+i, source.y+i);

                    if(p.type != PieceType::Empty)
                        return false;
                    ++i;
                }
                return true;
            }
            else
                return false;
        break;
        case PieceType::Queen:
            if(deltaX == 0 && deltaY != 0)
            {
                auto maxY = std::max(source.y, destination.y);
                for(int8_t i = std::min(source.y, destination.y)+1; i < maxY; ++i)
                {
                    if(getPiece(source.x, i).type != PieceType::Empty)
                        return false;
                }
                return true;
            }
            else if(deltaX != 0 && deltaY == 0)
            {
                auto maxX = std::max(source.x, destination.x);
                for(int8_t i = std::min(source.x, destination.x)+1; i < maxX; ++i)
                {
                    if(getPiece(i, source.y).type != PieceType::Empty)
                        return false;
                }
                return true;
            }
            if(deltaX/deltaY == 1)
            {
                for(int8_t i = 1; i < std::max(deltaX, deltaY); ++i)
                {
                    auto p = getPiece(source.x+i, source.y+i);

                    if(p.type != PieceType::Empty)
                        return false;
                    ++i;
                }
                return true;
            }
            else if(deltaX/deltaY == -1)
            {
                for(int8_t i = 1; i < std::min(deltaX, deltaY); --i)
                {
                    auto p = getPiece(source.x+i, source.y+i);

                    if(p.type != PieceType::Empty)
                        return false;
                    ++i;
                }
                return true;
            }
            else
                return false;
        break;
        case PieceType::King:
            if(((deltaX*deltaX == 1 && deltaY*deltaY == 1) || (deltaX*deltaX == 0 && deltaY*deltaY == 1) || (deltaX*deltaX == 1 && deltaY*deltaY == 0)))
            {
                return true;
            }
            else if(deltaY == 0 && deltaX == 2 && !piece.hasMoved && !getPiece(7, source.y).hasMoved)
            {
                for(int8_t i = 0; i < 2; ++i)
                {
                    if(isKingChecked(Position(source.x+i, source.y), piece.owner))
                        return false;
                }
                move = MoveType::KingSideCastling;
                return true;
            }
            else if(deltaY == 0 && deltaX == -2 && !piece.hasMoved && !getPiece(0, source.y).hasMoved)
            {
                for(int8_t i = 0; i < 2; ++i)
                {
                    if(isKingChecked(Position(source.x-i, source.y), piece.owner))
                        return false;
                }
                move = MoveType::QueenSideCastling;
                return true;
            }
            else
                return false;
        break;

        case PieceType::Knight:
            if(deltaX*deltaY == 2 || deltaX*deltaY == -2)
                return true;
            else
                return false;
        break;
        default:
            break;
    }
    return false;
}

void Game::backupState()
{
    copyState(m_currentBoardState, m_previousBoardState);
    m_previousbitState = m_bitState;
    m_previouslastPawnRush = m_lastPawnRush;
    m_previousKingPos[0] = m_KingPos[0];
    m_previousKingPos[1] = m_KingPos[1];

}

void Game::restoreState()
{
    copyState(m_previousBoardState, m_currentBoardState);
    m_bitState = m_previousbitState;
    m_lastPawnRush = m_previouslastPawnRush;
    m_KingPos[0] = m_previousKingPos[0];
    m_KingPos[1] = m_previousKingPos[1];

}

bool Game::makeMove(Position& source, Position& destination)
{
    MoveType move;
    auto& sourceBitBoard = m_bitState.getBitBoard(getPiece(source));
    auto& destinationBitBoard = m_bitState.getBitBoard(getPiece(destination));
    if(getPiece(source).owner == m_playerToMove && isMoveLegal(source, destination, move))
    {
        backupState();
        getPiece(source).hasMoved = true;
        if(move == MoveType::Move)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            std::swap(getPiece(source), getPiece(destination));
            m_lastPawnRush = Position();
        }
        else if(move == MoveType::Capture)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);

            destinationBitBoard.setBitFalse(destination.x, destination.y);

            std::swap(getPiece(source), getPiece(destination));
            getPiece(source) = Piece();
            m_lastPawnRush = Position();
        }
        else if(move == MoveType::PawnRush)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            m_lastPawnRush = destination;
            std::swap(getPiece(source), getPiece(destination));
        }
        else if(move == MoveType::EnPassant)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            std::swap(getPiece(source), getPiece(destination));
            if(getPiece(destination).owner == Player::White)
            {
                auto& capturedPawn = getPiece(destination.x, destination.y + 1);
                m_bitState.getBitBoard(capturedPawn).setBitFalse(destination.x, destination.y + 1);
                capturedPawn = Piece();
            }
            else
            {
                auto& capturedPawn = getPiece(destination.x, destination.y - 1);
                m_bitState.getBitBoard(capturedPawn).setBitFalse(destination.x, destination.y - 1);
                capturedPawn = Piece();
            }
            m_lastPawnRush = Position();
        }
        else if(move == MoveType::KingSideCastling)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            auto& rookBitBoard = m_bitState.getBitBoard(getPiece(source).owner, PieceType::Rook);
            rookBitBoard.setBitFalse(7, source.y);
            rookBitBoard.setBitTrue(5, destination.y);
            std::swap(getPiece(source), getPiece(destination));
            std::swap(getPiece(7, source.y), getPiece(5, source.y));
            m_lastPawnRush = Position();
        }
        else if(move == MoveType::QueenSideCastling)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            auto& rookBitBoard = m_bitState.getBitBoard(getPiece(source).owner, PieceType::Rook);
            rookBitBoard.setBitFalse(0, source.y);
            rookBitBoard.setBitTrue(3, destination.y);
            std::swap(getPiece(source), getPiece(destination));
            std::swap(getPiece(0, source.y), getPiece(3, source.y));
            m_lastPawnRush = Position();
        }
        if(getPiece(destination).type == PieceType::King)
        {
            m_KingPos[m_playerToMove] = destination;
        }
        if(isKingChecked(m_KingPos[m_playerToMove], m_playerToMove))
        {
            restoreState();
            return false;
        }
        if(m_playerToMove == Player::White)
            m_playerToMove = Player::Black;
        else
            m_playerToMove = Player::White;
        return true;
    }
    else
    {
        return false;
    }
}

bool Game::spawnPiece(PieceType type, Position pos, Player owner)
{
    if(m_currentBoardState[pos.getVectorIndex()].type != Empty)
        return false;
    Piece piece(type, owner);
    if(type == PieceType::King)
    {
        m_KingPos[owner] = pos;
    }
    m_bitState.getBitBoard(owner, type).setBitTrue(pos.x, pos.y);
    m_currentBoardState[pos.getVectorIndex()] = piece;
    return true;
}

uint8_t getVectorIndex(const uint8_t x, const uint8_t y)
{
    return 8*y+x;
}

Position Game::findPieceAtColumnRay(const Position& pos, bool up)
{
    Position ret;
    ret.x = pos.x;
    ret.y = pos.y;
    if(up)
    {
        for(int8_t y = pos.y-1; y >= 0; --y)
        {
            if(getPiece(pos.x, y).type != PieceType::Empty)
            {
                ret.y = y;
                break;
            }
        }
    }
    else
    {
        for(int8_t y = pos.y+1; y < 8; ++y)
        {
            if(getPiece(pos.x, y).type != PieceType::Empty)
            {
                ret.y = y;
                break;
            }
        }
    }
    return ret;
}

Position Game::findPieceAtMainDiagonalRay(const Position& pos, bool up)
{
    Position ret;
    ret.x = pos.x;
    ret.y = pos.y;

    if(up)
    {
        int8_t minCoordinate = std::min(pos.x,pos.y);
        for(int8_t i = 1; i <= minCoordinate; ++i)
        {
            if(getPiece(pos.x-i, pos.y-i).type != PieceType::Empty)
            {
                ret.x=pos.x-i;
                ret.y=pos.y-i;
                break;
            }
        }
    }
    else
    {
        int8_t maxCoordinate = 7-std::max(pos.x,pos.y);
        for(int8_t i = 1; i <= maxCoordinate; ++i)
        {

            if(getPiece(pos.x+i, pos.y+i).type != PieceType::Empty)
            {
                ret.x=pos.x+i;
                ret.y=pos.y+i;
                break;
            }
        }
    }
    return ret;
}

Position Game::findPieceAtAntiDiagonalRay(const Position& pos, bool up)
{
    Position ret;
    ret.x = pos.x;
    ret.y = pos.y;

    if(up)
    {
        int8_t moves = numberOfMovesDiagonalUp[pos.x][pos.y];
        for(int8_t i = 1; i <= moves; ++i)
        {
            if(getPiece(pos.x+i, pos.y-i).type != PieceType::Empty)
            {
                ret.x=pos.x+i;
                ret.y=pos.y-i;
                break;
            }
        }
    }
    else
    {
        int8_t moves = numberOfMovesDiagonalDown[pos.x][pos.y];
        for(int8_t i = 1; i <= moves; ++i)
        {

            if(getPiece(pos.x-i, pos.y+i).type != PieceType::Empty)
            {
                ret.x=pos.x-i;
                ret.y=pos.y+i;
                break;
            }
        }
    }
    return ret;
}

Position Game::findPieceAtRowRay(const Position& pos, bool left)
{
    Position ret;
    ret.x = pos.x;
    ret.y = pos.y;
    if(left)
    {
        for(int8_t x = pos.x-1; x >= 0; --x)
        {
            if(getPiece(x, pos.y).type != PieceType::Empty)
            {
                ret.x = x;
                break;
            }
        }
    }
    else
    {
        for(int8_t x = pos.x+1; x < 8; ++x)
        {
            if(getPiece(x, pos.y).type != PieceType::Empty)
            {
                ret.x = x;
                break;
            }
        }
    }
    return ret;
}

CheckType Game::isKingChecked(const Position& kingPos, const Player owner)
{
    uint8_t check = 0;
    BitBoard board;
    for(int8_t player = Player::White; player <= Player::Black; ++player)
    {
        for(int8_t pieceType = PieceType::Pawn; pieceType <= PieceType::King; ++pieceType)
        {
            board.getBoard() |= m_bitState.getBitBoard(static_cast<Player>(player), static_cast<PieceType>(pieceType)).getBoard();
        }
    }

    printBitBoard(board);
    auto blockingPiece = getPiece(findPieceAtColumnRay(kingPos, true));

    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Rook || blockingPiece.type == PieceType::Queen))
        ++check;
    blockingPiece = getPiece(findPieceAtColumnRay(kingPos, false));
    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Rook || blockingPiece.type == PieceType::Queen))
        ++check;
    blockingPiece = getPiece(findPieceAtRowRay(kingPos, true));
    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Rook || blockingPiece.type == PieceType::Queen))
        ++check;
    blockingPiece = getPiece(findPieceAtRowRay(kingPos, false));
    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Rook || blockingPiece.type == PieceType::Queen))
        ++check;

    blockingPiece = getPiece(findPieceAtMainDiagonalRay(kingPos, false));
    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Bishop || blockingPiece.type == PieceType::Queen))
        ++check;

    blockingPiece = getPiece(findPieceAtMainDiagonalRay(kingPos, true));
    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Bishop || blockingPiece.type == PieceType::Queen))
        ++check;


    blockingPiece = getPiece(findPieceAtAntiDiagonalRay(kingPos, false));
    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Bishop || blockingPiece.type == PieceType::Queen))
        ++check;

    blockingPiece = getPiece(findPieceAtAntiDiagonalRay(kingPos, true));
    if(blockingPiece.owner != owner && (blockingPiece.type == PieceType::Bishop || blockingPiece.type == PieceType::Queen))
        ++check;

    if((getPawnAttackersAt(kingPos.x, kingPos.y, owner)
      & m_bitState.getBitBoard(getOtherPlayer(owner), PieceType::Pawn)).getBoard())
        ++check;

    if((getKnightAttackersAt(kingPos.x, kingPos.y)
      & m_bitState.getBitBoard(getOtherPlayer(owner), PieceType::Knight)).getBoard())
        ++check;

    if((getKingAttackerAt(kingPos.x, kingPos.y)
      & m_bitState.getBitBoard(getOtherPlayer(owner), PieceType::King)).getBoard())
        ++check;
    return static_cast<CheckType>(check);
}

Player getOtherPlayer(const Player otherPlayer)
{
    return static_cast<Player> (!otherPlayer);
}

bool Game::isMated(CheckType check, Player player)
{
    if(check == CheckType::DoubleCheck)
    {
        MoveVec kingMoves;
        g_MoveGenerator.generateKingMoves(*this, player, kingMoves);
        if(kingMoves.size() > 0)
            return false;
    }
    else if(check == CheckType::SingleCheck)
    {
        //if(getMoveList(player).size() > 0)
        //    return false;
    }
    return true;
}
