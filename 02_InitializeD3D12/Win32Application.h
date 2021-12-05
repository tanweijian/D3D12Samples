#pragma once

#include <Windows.h>

class Win32Application
{
public:
    static int WINAPI Run(HINSTANCE hInstance, int nCmdShow);
    static HWND GetHwnd();

private:
    static bool _forceWarp;
    static HWND _hwnd;
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
