#pragma once

#include <Windows.h>

class WinApplication
{
private:
    HWND _hWnd = nullptr;
    int _width = 1280;
    int _height = 720;

public:
    void SethWnd(const HWND hWnd);
    HWND GethWnd();
    void SetWinSize(const int width, const int height);
    int GetWidth();
    int GetHeight();

public:
    void Update();
};

namespace Application
{
    int Run(WinApplication& app, HINSTANCE hInstance, const wchar_t* className, int nCmdShow);
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    void InitializeApplication(WinApplication& app);
    bool UpdateApplication(WinApplication& app);
    void TerminateApplication(WinApplication& app);
}
