#ifndef BOARD_H_INCLUDED
#define BOARD_H_INCLUDED

#include "framework/widget.h"
#include "game.h"

class Board : public Widget
{
public:
    Board(Widget* parent);
    virtual ~Board();
    virtual void paint();
    virtual void onMouseButtonClick(const SDL_Event& event);
    virtual void onMouseMove(const SDL_Event& event);
    void drawPiece(const Piece& piece, uint32_t x, uint32_t y);
    uint32_t getTileSize() const;
    Position getClickedPiecePosition(const int32_t x, const int32_t y) const;
    Position getClickedPiecePosition(const SDL_Rect& rect) const;
private:
    void makeMove();
    bool m_leftMouseButtonPressed = false;
    Position m_movedPiece;
    SDL_Rect m_mousePosition;
};

#endif // BOARD_H_INCLUDED
