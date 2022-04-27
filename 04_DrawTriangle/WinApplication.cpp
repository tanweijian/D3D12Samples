#include "WinApplication.h"

void WinApplication::SethWnd(const HWND hWnd)
{
    _hWnd = hWnd;
}

HWND WinApplication::GethWnd()
{
    return _hWnd;
}

void WinApplication::SetWinSize(const int width, const int height)
{
    _width = width;
    _height = height;
}

namespace Application
{
    int Run(WinApplication& app, HINSTANCE hInstance, const wchar_t* className, int nCmdShow)
    {
        // Register class
        WNDCLASSEX wcex;
        wcex.cbSize = sizeof(WNDCLASSEX);
        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WindowProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = nullptr;
        wcex.lpszClassName = className;
        wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
        RegisterClassEx(&wcex);

        HWND hInst = CreateWindow(className, className, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, nullptr, nullptr, hInstance, nullptr);
        app.SethWnd(hInst);

        return 0;
    }

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_SIZE:
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }

        return 0;
    }
}
