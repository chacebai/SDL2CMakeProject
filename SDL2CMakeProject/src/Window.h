#ifndef __IWINDOW_H__
#define __IWINDOW_H__

#include <sstream>
#include <iostream>

#include "SDL2/SDL.h"
#include "ILayout.h"
#include "IControl.h"

class Window
{
public:
    Window() = delete;
    Window(int width, int height);
    ~Window();

    // 初始化
    bool Initialize();
    // 设置布局
    void setLayout(ILayout* pLayout);
    // 将窗口移到屏幕中间
    void MoveToCenter();
    // 运行
    void Run();
    // 全局变量
    static Window* g_window;

private:
    // 绘制
    void onPaint(HDC dc);
    // 判断鼠标位置是否在某个控件上
    bool ifMouseOnControl(int x, int y);
    // 对鼠标移到消息的响应
    void onMouseMove(int x, int y);
    // 响应鼠标左键按下消息
    void onLButtonDown(int x, int y);
    // 响应鼠标左键弹起消息
    void onLButtonUp(int x, int y);
    // 获取屏幕DPI
    void GetScreenDpi(int& dpix, int& dpiy)
    {
        // 获取屏幕HDC
        HDC hdcScreen = GetDC(NULL);
        dpix = GetDeviceCaps(hdcScreen, LOGPIXELSX);
        dpiy = GetDeviceCaps(hdcScreen, LOGPIXELSY);
    }

private:
    void update()
    {
        if (fullscreen) SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN);
        if (!fullscreen) SDL_SetWindowFullscreen(m_window, 0);
    }
    void input()
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
    void draw()
    {
        //SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        //rect.x = 0;
        //rect.y = 0;
        //rect.w = WINDOW_WIDTH;
        //rect.h = WINDOW_HEIGHT;
        //SDL_RenderFillRect(renderer, &rect);

        // The image has 256x256 and it is positioned in the middle of the screen
        m_rect.x = (int)(m_width * 0.5f - m_image_width * 0.5f);
        m_rect.y = (int)(m_height * 0.5f - m_image_height * 0.5f);
        m_rect.w = m_image_width;
        m_rect.h = m_image_height;

        // Draw the texture in the screen
        SDL_RenderClear(m_renderer);
        SDL_RenderCopy(m_renderer, m_texture, nullptr, &m_rect);

        frameCount++;
        int timerFPS = SDL_GetTicks() - lastFrame;
        if (timerFPS < (1000 / 60))
        {
            SDL_Delay((1000 / 60) - timerFPS);
        }

        SDL_RenderPresent(m_renderer);
    }
    static int resizingEventWatcher(void* data, SDL_Event* event) {
        if (event->type == SDL_WINDOWEVENT &&
            event->window.event == SDL_WINDOWEVENT_RESIZED) {
            SDL_Window* win = SDL_GetWindowFromID(event->window.windowID);
            if (win == (SDL_Window*)data) {
                SDL_Log("resizing xPos: %d yPos: %d\n", event->window.data1, event->window.data2);
                SDL_RenderClear(Window::g_window->m_renderer);
                SDL_RenderCopy(Window::g_window->m_renderer, Window::g_window->m_texture, nullptr, &Window::g_window->m_rect);
                SDL_RenderPresent(Window::g_window->m_renderer);
            }
        }
        return 0;
    }
    // 窗口过程函数
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    // 变量
    HWND    m_hwnd;     //  窗口句柄
    int     m_width;    // 窗口宽度，单位为像素
    int     m_height;   // 窗口高度，单位为像素
    ILayout* m_pLayout = nullptr; // 窗口布局

    IControl* m_pControlCaptureMouse = nullptr; // 鼠标所在位置的控件
    IControl* m_pControlGotFocus = nullptr; // 获得焦点的控件

    int lastTime = 0;
    int     m_image_width;    // 窗口宽度，单位为像素
    int     m_image_height;   // 窗口高度，单位为像素
    bool running, fullscreen;
    int frameCount = 0, timerFPS = 0, lastFrame = 0, fps = 0;

    SDL_Rect m_rect = { 0 };
    SDL_Window* m_window = nullptr;
    SDL_Texture* m_texture = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};

#endif
