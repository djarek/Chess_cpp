#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <SDL2/SDL.h>
#include <vector>
#include <functional>

#include "framework/widget.h"
#include "boost/signals2.hpp"
class MainWindow : public Widget
{
public:
    MainWindow(Widget* parent);
    bool init();
    void eventLoop();
    virtual void paint();
    virtual ~MainWindow();
    virtual void onMouseButtonClick(const SDL_Event& event);
    virtual void onMouseMove(const SDL_Event& event);
    virtual void addWidget(Widget* widget);
protected:
    SDL_Window* m_window = nullptr;
    std::vector<Widget*> widgets;
};

#endif // MAINWINDOW_H_INCLUDED
