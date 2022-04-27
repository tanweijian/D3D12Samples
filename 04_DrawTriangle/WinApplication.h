#pragma once

#include <Windows.h>
#include <string>

class WinApplication
{
private:
    HWND _hWnd = nullptr;
    int _width = 1280;
    int _height = 720;
    std::wstring _name;

public:
    WinApplication(int width, int height, std::wstring name);
    void SethWnd(const HWND hWnd);
    HWND GethWnd();
    int GetWidth();
    int GetHeight();
    std::wstring GetName();

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
