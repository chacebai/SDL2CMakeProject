// SDL2CMakeProject.cpp : Defines the entry point for the application.
//
#include <sstream>

#include "SDL2/SDL.h"

#include "SDL2CMakeProject.h"

#include "src/Window.h"

using namespace std;

// Window size
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

int main(int argc, char* args[])
{
    // SDL initialisation
    Window window(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (window.Initialize())
    {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -1;
    }
    //SDL_AddEventWatch(resizingEventWatcher, window);
    window.Run();
	return 0;
}
