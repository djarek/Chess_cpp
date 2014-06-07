#include "mainwindow.h"
#include "application.h"
#include "globals.h"
#include "board.h"

MainWindow::MainWindow(Widget* parent)
{
    if(parent)
        throw;
    m_parent = nullptr;
    try
    {
        m_rect.x = g_ConfigManager.getConfigValue<int32_t>("defaultWindowXPos");
        m_rect.y = g_ConfigManager.getConfigValue<int32_t>("defaultWindowYPos");;
        m_rect.h = g_ConfigManager.getConfigValue<int32_t>("defaultWindowHeight");
        m_rect.w = g_ConfigManager.getConfigValue<int32_t>("defaultWindowWidth");
    }
    catch(...)
    {

    }
}

MainWindow::~MainWindow()
{
    for(auto widget : widgets)
    {
        delete widget;
    }
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
}

void MainWindow::addWidget(Widget* widget)
{
    widgets.push_back(widget);
}

void MainWindow::paint()
{
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);

    SDL_RenderClear(m_renderer);
    for(auto widget : widgets)
    {
        widget->paint();
    }
    SDL_RenderPresent(m_renderer);
}

void MainWindow::eventLoop()
{
    SDL_Event event;
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    paint();
                    continue;
                case SDL_MOUSEMOTION:
                    onMouseMove(event);
                    break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN:
                    onMouseButtonClick(event);
                    break;
                default:
                    break;
            }
        }
        paint();
        SDL_Delay(16.67);
    }
}

bool MainWindow::init()
{
    m_window = SDL_CreateWindow(g_ConfigManager.getConfigValue<std::string>("defaultWindowName").c_str(),
                              m_rect.x, m_rect.y, m_rect.w, m_rect.h, SDL_WINDOW_SHOWN);
    m_renderer = SDL_CreateRenderer(m_window, -1,
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    addWidget(new Board(this));
    return true;
}

bool isPointInRect(SDL_Rect& rect, int32_t x, int32_t y)
{
    if(x > rect.x && x < rect.w + rect.x && y > rect.y && y < rect.h + rect.y)
        return true;
    else
        return false;
}
void MainWindow::onMouseButtonClick(const SDL_Event& event)
{
    SDL_Rect rect, mouse_rect;
    if(event.button.button == SDL_BUTTON_LEFT)
    {
        for(auto* widget : widgets)
        {
            mouse_rect.x = event.button.x;
            mouse_rect.y = event.button.y;
            mouse_rect.w = mouse_rect.h = 1;
            rect = widget->getRect();
            if(SDL_HasIntersection(&rect, &mouse_rect))
            //if(isPointInRect(rect, event.button.x, event.button.y))
                widget->onMouseButtonClick(event);
        }
    }
}

void MainWindow::onMouseMove(const SDL_Event& event)
{
    SDL_Rect rect, mouse_rect;

    for(const auto& widget : widgets)
    {
        mouse_rect.x = event.button.x;
        mouse_rect.y = event.button.y;
        mouse_rect.w = mouse_rect.h = 10;
        rect = widget->getRect();
        if(SDL_HasIntersection(&rect, &mouse_rect))
            widget->onMouseMove(event);
    }
}
