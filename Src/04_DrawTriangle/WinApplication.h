#pragma once

#include <string>
#include <Windows.h>

#include "D3D12Context.h"
#include "D3D12GfxDevice.h"

class WinApplication
{
private:
    // propertice
    HWND _hWnd = nullptr;
public:
    // constructor
    WinApplication();
    ~WinApplication();
    // propertice
    int Width = 1280;
    int Height = 720;
    std::wstring Name;

    D3D12Context* Context;
    D3D12GfxDevice* GfxDevice;

    // function
    void SethWnd(const HWND hWnd);
    HWND GethWnd();
    void Initialize();
    void Update();
    void Terminate();

    // ------------------------ static ------------------------
public:
    static int Run(WinApplication& app, HINSTANCE hInstance, const wchar_t* className, int nCmdShow);
protected:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static void InitializeApplication(WinApplication& app);
    static bool UpdateApplication(WinApplication& app);
    static void TerminateApplication(WinApplication& app);
    // --------------------------------------------------------
};
