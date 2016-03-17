#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED
#include "mainwindow.h"
#include "framework/widget.h"
class Application : public Object
{
public:
    Application();
    bool init(int argc, char** argv);
    void run();
    virtual ~Application();
    Application(const Application& other) = delete;
    Application(Application&& other) = delete;
private:

};


#endif // APPLICATION_H
