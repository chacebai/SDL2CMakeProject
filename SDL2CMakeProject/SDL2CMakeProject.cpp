// SDL2CMakeProject.cpp : Defines the entry point for the application.
//
#include <sstream>

#include "SDL2/SDL.h"

#include "SDL2CMakeProject.h"

using namespace std;

// Window size
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

const int IMAGE_WIDTH = 256;
const int IMAGE_HEIGHT = 256;

SDL_Rect rect = { 0 };
SDL_Window* window = nullptr;
SDL_Texture* texture = nullptr;
SDL_Renderer* renderer = nullptr;

bool running, fullscreen;
int frameCount = 0, timerFPS = 0, lastFrame = 0, fps = 0;

static int resizingEventWatcher(void* data, SDL_Event* event) {
    if (event->type == SDL_WINDOWEVENT &&
        event->window.event == SDL_WINDOWEVENT_RESIZED) {
        SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
        if (win == (SDL_Window*)data) {
            SDL_Log("resizing\n");

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, nullptr, &rect);
            SDL_RenderPresent(renderer);
        }
    }
    return 0;
}

static void update()
{
    if (fullscreen) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
    if (!fullscreen) SDL_SetWindowFullscreen(window, 0);
}

static void input()
{
    SDL_Event event;
    std::string inputText;
    const Uint8* keystatus = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) running = false;
        if (event.type == SDL_KEYDOWN) {
            std::cout
                << "Key Pressed! Key Code: "
                << event.key.keysym.sym
                << ", Key Name: "
                << SDL_GetKeyName(event.key.keysym.sym)
                << '\n';
        }
    }
}

static void draw()
{
    //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    //rect.x = 0;
    //rect.y = 0;
    //rect.w = WINDOW_WIDTH;
    //rect.h = WINDOW_HEIGHT;
    //SDL_RenderFillRect(renderer, &rect);

    // The image has 256x256 and it is positioned in the middle of the screen
    rect.x = (int)(WINDOW_WIDTH * 0.5f - IMAGE_WIDTH * 0.5f);
    rect.y = (int)(WINDOW_HEIGHT * 0.5f - IMAGE_HEIGHT * 0.5f);
    rect.w = IMAGE_WIDTH;
    rect.h = IMAGE_HEIGHT;

    // Draw the texture in the screen
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    frameCount++;
    int timerFPS = SDL_GetTicks() - lastFrame;
    if (timerFPS < (1000 / 60))
    {
        SDL_Delay((1000 / 60) - timerFPS);
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[])
{
    static int lastTime = 0;
    // SDL initialisation

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
    //if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    // Window creation and position in the center of the screen
    window = SDL_CreateWindow("Hello World SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (window == nullptr)
    {
        SDL_Log("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return -1;
    }
    //SDL_AddEventWatch(resizingEventWatcher, window);

    // Render creation
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr)
    {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 225, 225, 225, 0);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_SetWindowTitle(window, "SDL2 Window");
    SDL_ShowCursor(1);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_AddEventWatch(resizingEventWatcher, window);

    // Image and texture creation
    SDL_Surface* image = SDL_LoadBMP("D:/code/SDL2CMakeProject/480-360-sample.bmp");
    if (image == nullptr)
    {
        SDL_Log("SDL_LoadBMP Error: %s\n", SDL_GetError());
        return -1;
    }

    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (texture == nullptr)
    {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return -1;
    }

    // Free the image (only the texture is used now)
    SDL_FreeSurface(image);

    running = 1;
    fullscreen = 0;
    while (running)
    {
        lastFrame = SDL_GetTicks();
        int lastFramePlus = lastTime + 1000;
        if (lastFrame >= lastFramePlus)
        {
            lastTime = lastFrame;
            fps = frameCount;
            frameCount = 0;
        }
        std::stringstream title;
        title << "SDL2 Window fps:";
        title << fps;
        SDL_SetWindowTitle(window, title.str().c_str());
        update();
        input();
        draw();
    }

    // Free the texture
    SDL_DestroyTexture(texture);

    // Destroy the render, window and finalise SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
	return 0;
}
