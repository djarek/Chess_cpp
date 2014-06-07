#include "application.h"

/** \brief
*/
Application::Application()
{

}

/** \brief Application's destructor
    Cleans up any allocated resources
*/
Application::~Application()
{
    SDL_Quit();
}

/** \brief Initializes any resources needed by the Application
*/
bool Application::init(int /*argc*/, char** /*argv*/)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        //TODO: error handling
        //std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return false;
	}
	return true;
}

/** \brief
*/
void Application::run()
{
    //main event loop
    MainWindow main(nullptr);
    main.init();
    main.eventLoop();
}
