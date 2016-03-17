#include "game.h"
#include <iostream>
#include "globals.h"

std::mutex g_Game_mutex;

AIEngine AIPlayers[2];

bool operator!=(const Position& first, const Position& second)
{
    return first.x != second.x || second.y != first.y;
}

bool operator==(const Position& first, const Position& second)
{
    return first.x == second.x && second.y == first.y;
}

Game::Game()
{
    if(m_players[0] == AI)
        AIPlayers[0].init();
    if(m_players[1] == AI)
        AIPlayers[1].init();
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

    m_gameStatus = inProgress;
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
                else if(deltaX == 0 && targetPiece.type == PieceType::Empty)
                    return true;
                else
                    return false;
            }
            else if(deltaY == 2*playerSign && !piece.hasMoved && deltaX == 0 && getPiece(source.x, source.y+1*playerSign).type == PieceType::Empty)
            {
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
                if(deltaY < 0)
                {
                    auto p = findPieceAtMainDiagonalRay(source, true);
                    if(p.x <= destination.x || p == source)
                        return true;
                    else
                        return false;
                }
                else if(deltaY > 0)
                {
                    auto p = findPieceAtMainDiagonalRay(source, false);
                    if(p.x >= destination.x || p == source)
                        return true;
                    else
                        return false;
                }
            }
            else if(deltaX/deltaY == -1)
            {
                if(deltaY < 0)
                {
                    auto p = findPieceAtAntiDiagonalRay(source, true);
                    if(p.x >= destination.x  || p == source)
                        return true;
                    else
                        return false;
                }
                else if(deltaY > 0)
                {
                    auto p = findPieceAtAntiDiagonalRay(source, false);
                    if(p.x <= destination.x || p == source)
                        return true;
                    else
                        return false;
                }
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
            else if(deltaX/deltaY == 1)
            {
                if(deltaY < 0)
                {
                    auto p = findPieceAtMainDiagonalRay(source, true);
                    if(p.x <= destination.x || p == source)
                        return true;
                    else
                        return false;
                }
                else if(deltaY > 0)
                {
                    auto p = findPieceAtMainDiagonalRay(source, false);
                    if(p.x >= destination.x || p == source)
                        return true;
                    else
                        return false;
                }
            }
            else if(deltaX/deltaY == -1)
            {
                if(deltaY < 0)
                {
                    auto p = findPieceAtAntiDiagonalRay(source, true);
                    if(p.x >= destination.x  || p == source)
                        return true;
                    else
                        return false;
                }
                else if(deltaY > 0)
                {
                    auto p = findPieceAtAntiDiagonalRay(source, false);
                    if(p.x <= destination.x || p == source)
                        return true;
                    else
                        return false;
                }
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
    m_previousgameStatus = m_gameStatus;
}

void Game::restoreState()
{
    copyState(m_previousBoardState, m_currentBoardState);
    m_bitState = m_previousbitState;
    m_lastPawnRush = m_previouslastPawnRush;
    m_KingPos[0] = m_previousKingPos[0];
    m_KingPos[1] = m_previousKingPos[1];
    m_gameStatus = m_previousgameStatus;

}

bool Game::makeMove(const Position& source, const Position& destination, const bool mateCheck)
{
    MoveType move;
    if(m_gameStatus != inProgress)
        return false;
    auto& sourcePiece = getPiece(source);
    auto& destinationPiece = getPiece(destination);
    auto& sourceBitBoard = m_bitState.getBitBoard(getPiece(source));
    auto& destinationBitBoard = m_bitState.getBitBoard(getPiece(destination));
    //If mateCheck is true (default value) we still perform legality checks, however
    //if it's false we skip them (because it mean the function has been called with pre-generated moves anyway,
    //so no need to check them again (the moving player is presumably the AI)
    if(sourcePiece.owner == m_playerToMove && (isMoveLegal(source, destination, move)))
    {
        backupState();
        sourcePiece.hasMoved = true;
        if(move == MoveType::Move)
        {

            sourceBitBoard.setBitFalse(source.x, source.y);
            if(sourcePiece.type == PieceType::Pawn && (destination.y == 0 || destination.y == 7))
            {
                m_bitState.getBitBoard(m_playerToMove, PieceType::Queen).setBitTrue(destination.x, destination.y);
                sourcePiece.type = PieceType::Queen;
            }
            else
                sourceBitBoard.setBitTrue(destination.x, destination.y);
            std::swap(sourcePiece, destinationPiece);
            m_lastPawnRush = Position();
        }
        else if(move == MoveType::Capture)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            if(sourcePiece.type == PieceType::Pawn && (destination.y == 0 || destination.y == 7))
            {
                m_bitState.getBitBoard(m_playerToMove, PieceType::Queen).setBitTrue(destination.x, destination.y);
                sourcePiece.type = PieceType::Queen;
            }
            else
                sourceBitBoard.setBitTrue(destination.x, destination.y);

            destinationBitBoard.setBitFalse(destination.x, destination.y);

            std::swap(sourcePiece, destinationPiece);
            sourcePiece = Piece();
            m_lastPawnRush = Position();
        }
        else if(move == MoveType::PawnRush)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            m_lastPawnRush = destination;
            std::swap(sourcePiece, destinationPiece);
        }
        else if(move == MoveType::EnPassant)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            std::swap(sourcePiece, destinationPiece);
            if(destinationPiece.owner == Player::White)
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
            auto& rookBitBoard = m_bitState.getBitBoard(sourcePiece.owner, PieceType::Rook);
            rookBitBoard.setBitFalse(7, source.y);
            rookBitBoard.setBitTrue(5, destination.y);
            std::swap(sourcePiece, destinationPiece);
            std::swap(getPiece(7, source.y), getPiece(5, source.y));
            m_lastPawnRush = Position();
        }
        else if(move == MoveType::QueenSideCastling)
        {
            sourceBitBoard.setBitFalse(source.x, source.y);
            sourceBitBoard.setBitTrue(destination.x, destination.y);
            auto& rookBitBoard = m_bitState.getBitBoard(sourcePiece.owner, PieceType::Rook);
            rookBitBoard.setBitFalse(0, source.y);
            rookBitBoard.setBitTrue(3, destination.y);
            std::swap(sourcePiece, destinationPiece);
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

        m_playerToMove = getOtherPlayer(m_playerToMove);
        if(mateCheck && isMated(isKingChecked(m_KingPos[m_playerToMove], m_playerToMove), m_playerToMove))
        {
            std::cout << ((m_playerToMove == Player::White) ? std::string("Black") : std::string("White")) << "won!" << std::endl;
            m_gameStatus = static_cast<State>(getOtherPlayer(m_playerToMove));
            return true;
        }
        #ifdef DEBUG_LOG
        std::cout << "Board value for White: " << AIEngine::evaluateBoard(*this, Player::White) << std::endl;
        std::cout << "Board value for Black: " << AIEngine::evaluateBoard(*this, Player::Black) << std::endl;
        #endif
        if(&g_Game == this && mateCheck && m_players[m_playerToMove] == AI)
        {
            AIPlayers[m_playerToMove].getMove(*this, m_playerToMove);
        }
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

Position Game::findPieceAtColumnRay(const Position& pos, const bool up) const
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

Position Game::findPieceAtMainDiagonalRay(const Position& pos, const bool up) const
{
    Position ret;
    ret.x = pos.x;
    ret.y = pos.y;

    if(up)
    {
        int8_t moves = numberOfMovesMainDiagonalUp[pos.x][pos.y];
        for(int8_t i = 1; i <= moves; ++i)
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
        int8_t moves = numberOfMovesMainDiagonalDown[pos.x][pos.y];
        for(int8_t i = 1; i <= moves; ++i)
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

Position Game::findPieceAtAntiDiagonalRay(const Position& pos, const bool up) const
{
    Position ret;
    ret.x = pos.x;
    ret.y = pos.y;

    if(up)
    {
        int8_t moves = numberOfMovesAntiDiagonalUp[pos.x][pos.y];
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
        int8_t moves = numberOfMovesAntiDiagonalDown[pos.x][pos.y];
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

Position Game::findPieceAtRowRay(const Position& pos, const bool left) const
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
    #ifdef DEBUG_LOG
    BitBoard board;
    for(int8_t player = Player::White; player <= Player::Black; ++player)
    {
        for(int8_t pieceType = PieceType::Pawn; pieceType <= PieceType::King; ++pieceType)
        {
            board.getBoard() |= m_bitState.getBitBoard(static_cast<Player>(player), static_cast<PieceType>(pieceType)).getBoard();
        }
    }
    printBitBoard(board);
    #endif //DEBUG

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

    if((g_MoveGenerator.getPawnAttackersAt(kingPos.x, kingPos.y, owner)
      & m_bitState.getBitBoard(getOtherPlayer(owner), PieceType::Pawn)).getBoard())
        ++check;

    if((g_MoveGenerator.getKnightAttackersAt(kingPos.x, kingPos.y)
      & m_bitState.getBitBoard(getOtherPlayer(owner), PieceType::Knight)).getBoard())
        ++check;

    if((g_MoveGenerator.getKingAttackerAt(kingPos.x, kingPos.y)
      & m_bitState.getBitBoard(getOtherPlayer(owner), PieceType::King)).getBoard())
        ++check;
    return static_cast<CheckType>(check);
}

Player getOtherPlayer(const Player otherPlayer)
{
    return static_cast<Player> (!otherPlayer);
}

bool Game::isMated(const CheckType check, const Player player)
{
    MoveVec moves;
    if(check == CheckType::NoCheck)
        return false;
    else if(check == CheckType::DoubleCheck)
    {
        g_MoveGenerator.generateKingMoves(*this, player, moves);
        if(moves.size() == 0)
            return true;
        for(const auto& move : moves)
        {
            if(makeMove(move.fromPos, move.toPos, false))
            {
                restoreState();
                m_playerToMove = getOtherPlayer(m_playerToMove);
                #ifdef DEBUG_LOG
                std::cout << "Found move: frompos= " << move.fromPos.x
                          << " " << move.fromPos.y << " topos= " << move.toPos.x
                          << " " << move.toPos.y << std::endl;
                #endif //DEBUG
                return false;
            }
            restoreState();
            m_playerToMove = getOtherPlayer(m_playerToMove);
        }
    }
    else if(check == CheckType::SingleCheck)
    {
        moves = std::move(g_MoveGenerator.generateAllMoves(*this, m_playerToMove));
        auto& pc = getPiece(7,7);
        for(const auto& move : moves)
        {
            if(makeMove(move.fromPos, move.toPos, false))
            {
                restoreState();
                #ifdef DEBUG_LOG
                std::cout << "Found move: frompos= " << (int32_t)move.fromPos.x
                          << " " << (int32_t)move.fromPos.y << " topos= " << (int32_t)move.toPos.x
                          << " " << (int32_t)move.toPos.y << std::endl;
                #endif //DEBUG
                m_playerToMove = getOtherPlayer(m_playerToMove);
                return false;
            }
        }
    }
    return true;
}
Game::Game(const Game& other)
{
    *this = other;
}
Game& Game::operator=(const Game& other)
{
    m_bitState = other.m_bitState;
    std::copy(other.m_currentBoardState, other.m_currentBoardState+64, m_currentBoardState);
    m_lastPawnRush = other.m_lastPawnRush;
    m_KingPos[0]  = other.m_KingPos[0];
    m_KingPos[1]  = other.m_KingPos[1];
    m_gameStatus = other.m_gameStatus;
    m_playerToMove = other.m_playerToMove;
    return *this;
}
