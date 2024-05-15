#include "Window.h"
#include <GdiPlus.h>
#include <Windowsx.h>

// ���岢��ʼ��ȫ�ֱ���
Window* Window::g_window = nullptr;

Window::Window(int width, int height)
{
    // ��ѯ��ĻDPI
    int dpix, dpiy;
    GetScreenDpi(dpix, dpiy);
    // ���ڴ�С
    m_width = (int)(width * dpix / 96.0f);
    m_height = (int)(height * dpiy / 96.0f);
    m_image_width = 256;
    m_image_height = 256;
    Window::g_window = this;
}


Window::~Window()
{
    // Free the texture
    SDL_DestroyTexture(m_texture);

    // Destroy the render, window and finalise SDL
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}


bool Window::Initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) std::cout << "Failed at SDL_Init()" << std::endl;
    //if (SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) std::cout << "Failed at SDL_CreateWindowAndRenderer()" << std::endl;
    // Window creation and position in the center of the screen
    m_window = SDL_CreateWindow("Hello World SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    return m_window == nullptr;
}


// ���ò���
void Window::setLayout(ILayout* pLayout)
{
    m_pLayout = pLayout;
    m_pLayout->setPos(0, 0, m_width, m_height);
}


void Window::Run()
{
    // Render creation
    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (m_renderer == nullptr)
    {
        SDL_Log("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        return;
    }
    SDL_SetRenderDrawColor(m_renderer, 225, 225, 225, 0);
    SDL_RenderClear(m_renderer);
    SDL_RenderPresent(m_renderer);

    SDL_SetWindowTitle(m_window, "SDL2 Window");
    SDL_ShowCursor(1);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "2");
    SDL_AddEventWatch(Window::resizingEventWatcher, m_window);

    // Image and texture creation
    SDL_Surface* image = SDL_LoadBMP("D:/code/sdl2-cmake-project/480-360-sample.bmp");
    if (image == nullptr)
    {
        SDL_Log("SDL_LoadBMP Error: %s\n", SDL_GetError());
        return;
    }

    m_texture = SDL_CreateTextureFromSurface(m_renderer, image);
    if (m_texture == nullptr)
    {
        SDL_Log("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        return;
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
        SDL_SetWindowTitle(m_window, title.str().c_str());
        update();
        input();
        draw();
    }
}


LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return DefWindowProc(hWnd, message, wParam, lParam);
}


void Window::MoveToCenter()
{
    int srcWidth = GetSystemMetrics(SM_CXSCREEN);
    int srcHeight = GetSystemMetrics(SM_CYSCREEN);

    int left = (srcWidth - m_width) / 2;
    int top = (srcHeight - m_height) / 2;

    //MoveWindow(m_hwnd, left, top, m_width, m_height, false);
}


// ����
void Window::onPaint(HDC dc)
{
}


// �ж����λ���Ƿ���ĳ���ؼ���
bool Window::ifMouseOnControl(int x, int y)
{
    return m_pLayout->hitTest(x, y) != nullptr;
}


// ������Ƶ���Ϣ����Ӧ
void Window::onMouseMove(int x, int y)
{
    auto var = dynamic_cast<IControl*>(m_pLayout->hitTest(x, y));

    if (var == m_pControlCaptureMouse)
    {
        return;
    }

    if (m_pControlCaptureMouse != nullptr)
    {
        m_pControlCaptureMouse->mouseMoveOut();
        //InvalidateRect(m_hwnd, NULL, false);
    }

    if (var != nullptr)
    {
        var->mouseMoveIn();
        //InvalidateRect(m_hwnd, NULL, false);
    }

    m_pControlCaptureMouse = var;
}


// ��Ӧ������������Ϣ
void Window::onLButtonDown(int x, int y)
{
    auto var = dynamic_cast<IControl*>(m_pLayout->hitTest(x, y));

    if (var != nullptr)
    {
        var->LMBDown();
        //InvalidateRect(m_hwnd, NULL, false);
    }
    // ������µĿؼ�����ת������
    if (var != m_pControlGotFocus)
    {
        if (var != nullptr)
        {
            var->getFocus();
            //InvalidateRect(m_hwnd, NULL, false);
        }

        if (m_pControlGotFocus != nullptr)
        {
            m_pControlGotFocus->loseFocus();
            //InvalidateRect(m_hwnd, NULL, false);
        }

        m_pControlGotFocus = var;
    }
    //UpdateWindow(m_hwnd);
}


// ��Ӧ������������Ϣ
void Window::onLButtonUp(int x, int y)
{
    // �ж���굯��ʱ�����λ���Ƿ���һ���ؼ���
    auto var = dynamic_cast<IControl*>(m_pLayout->hitTest(x, y));

    // �����
    if (var != nullptr && var == m_pControlGotFocus)
    {
        var->LMBUp();
        //InvalidateRect(m_hwnd, NULL, false);
    }
}
