#include "board.h"
#include "globals.h"


Board::Board(Widget* parent)
{
    m_parent = parent;
    m_renderer = parent->getRenderer();
    m_rect = parent->getRect();
    m_mousePosition.h = m_mousePosition.w = getTileSize();
}

Board::~Board()
{

}

uint32_t Board::getTileSize() const
{
    return 80;
}

void Board::paint()
{
    SDL_Rect rect;
    rect.x = m_rect.x;
    rect.y = m_rect.y;
    rect.h = rect.w = getTileSize();
    uint8_t x = 0, y = 0;
    uint8_t colors[] ={150, 255};

    for(uint8_t i = 0; i < 8; ++i)
    {
        for(uint8_t j = 0; j < 8; ++j)
        {
            if(m_leftMouseButtonPressed && m_movedPiece.x == i && m_movedPiece.y == j)
            {
                x = i;
                y = j;
                SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
                SDL_RenderFillRect(m_renderer, &rect);
            }
            else
            {
                SDL_SetRenderDrawColor(m_renderer, colors[(i+j+1)%2], colors[(i+j+1)%2], colors[(i+j+1)%2], 255);
                SDL_RenderFillRect(m_renderer, &rect);
                drawPiece(g_Game.getPiece(i, j), i*getTileSize(), j*getTileSize());
            }
            rect.y += rect.h;
        }
        rect.y = 0;
        rect.x += rect.w;
    }
    if(m_leftMouseButtonPressed)
        drawPiece(g_Game.getPiece(x, y), m_mousePosition.x, m_mousePosition.y);
}

void Board::drawPiece(const Piece& piece, uint32_t x, uint32_t y)
{
    SDL_Rect rect;
    rect.h = rect.w = getTileSize();
    rect.x = x;
    rect.y = y;
    std::string assetName;
    if(piece.owner == Player::White)
        assetName = "white_";
    else
        assetName = "black_";
    if(piece.type == PieceType::Empty)
        return;

    assetName += PIECE_NAMES[piece.type];
    SDL_RenderCopy(getRenderer(), g_AssetManager.getAsset(assetName, this).getTexture(), nullptr, &rect);
}
Position Board::getClickedPiecePosition(const int32_t x, const int32_t y) const
{
    Position ret((x-getRect().x)/getTileSize(), (y-getRect().y)/getTileSize());
    return ret;
}

Position Board::getClickedPiecePosition(const SDL_Rect& rect) const
{
    return Position((rect.x-getRect().x)/getTileSize(), (rect.y-getRect().y)/getTileSize());
}

void Board::onMouseButtonClick(const SDL_Event& event)
{
    if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONUP)
    {
        if(!m_leftMouseButtonPressed)//Started moving a piece
        {
            if(g_Game.getGameStatus() != State::inProgress || g_Game.getPiece(getClickedPiecePosition(event.button.x, event.button.y)).type == PieceType::Empty || g_Game.getPiece(getClickedPiecePosition(event.button.x, event.button.y)).owner != g_Game.whoMoves())
            {
                return;
            }
            m_mousePosition.x = event.button.x-getTileSize()/2;
            m_mousePosition.y = event.button.y-getTileSize()/2;
            m_movedPiece = getClickedPiecePosition(event.button.x, event.button.y);
            m_leftMouseButtonPressed = true;
        }
        else //stop moving a piece
        {
            m_mousePosition.x = event.button.x;
            m_mousePosition.y = event.button.y;
            makeMove();
            m_leftMouseButtonPressed = false;

        }

        //std::cout << "lbmPressed = " << (uint32_t)pos.x << " "<< (uint32_t)pos.y << std::endl;
    }
}

void Board::onMouseMove(const SDL_Event& event)
{
    if(!m_leftMouseButtonPressed)
        return;
    m_mousePosition.x = event.button.x-getTileSize()/2;
    m_mousePosition.y = event.button.y-getTileSize()/2;
}

void Board::makeMove()
{
    auto newPos = getClickedPiecePosition(m_mousePosition);
    if(newPos != m_movedPiece)
        g_Game.makeMove(m_movedPiece, newPos);
}
