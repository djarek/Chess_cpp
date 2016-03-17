#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include <iostream>
#include <SDL2/SDL.h>
class Object
{
public:
protected:
private:

};

class Widget : public Object
{
public:
    virtual uint32_t getHeight() const {return m_rect.h;}
    virtual uint32_t getWidth() const {return m_rect.w;}
    virtual uint32_t getPosX() const {return m_rect.x;}
    virtual uint32_t getPosY() const {return m_rect.y;}
    virtual SDL_Rect getRect() const {return m_rect;}
    virtual void paint() = 0;
    virtual SDL_Renderer* getRenderer(){return m_renderer;}
    virtual void onMouseButtonClick(const SDL_Event& event)=0;
    virtual void onMouseMove(const SDL_Event& event)=0;
    virtual ~Widget(){}
protected:
    SDL_Rect m_rect;
    Widget* m_parent;
    SDL_Renderer* m_renderer = nullptr;
private:
};


#endif // OBJECT_H_INCLUDED
