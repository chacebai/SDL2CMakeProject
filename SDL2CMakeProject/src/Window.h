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

    // ��ʼ��
    bool Initialize();
    // ���ò���
    void setLayout(ILayout* pLayout);
    // �������Ƶ���Ļ�м�
    void MoveToCenter();
    // ����
    void Run();
    // ȫ�ֱ���
    static Window* g_window;

private:
    // ����
    void onPaint(HDC dc);
    // �ж����λ���Ƿ���ĳ���ؼ���
    bool ifMouseOnControl(int x, int y);
    // ������Ƶ���Ϣ����Ӧ
    void onMouseMove(int x, int y);
    // ��Ӧ������������Ϣ
    void onLButtonDown(int x, int y);
    // ��Ӧ������������Ϣ
    void onLButtonUp(int x, int y);
    // ��ȡ��ĻDPI
    void GetScreenDpi(int& dpix, int& dpiy)
    {
        // ��ȡ��ĻHDC
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
    // ���ڹ��̺���
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    // ����
    HWND    m_hwnd;     //  ���ھ��
    int     m_width;    // ���ڿ�ȣ���λΪ����
    int     m_height;   // ���ڸ߶ȣ���λΪ����
    ILayout* m_pLayout = nullptr; // ���ڲ���

    IControl* m_pControlCaptureMouse = nullptr; // �������λ�õĿؼ�
    IControl* m_pControlGotFocus = nullptr; // ��ý���Ŀؼ�

    int lastTime = 0;
    int     m_image_width;    // ���ڿ�ȣ���λΪ����
    int     m_image_height;   // ���ڸ߶ȣ���λΪ����
    bool running, fullscreen;
    int frameCount = 0, timerFPS = 0, lastFrame = 0, fps = 0;

    SDL_Rect m_rect = { 0 };
    SDL_Window* m_window = nullptr;
    SDL_Texture* m_texture = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};

#endif
