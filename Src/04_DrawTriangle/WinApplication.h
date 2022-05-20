#pragma once

#include <string>
#include <Windows.h>

#include "D3D12GfxDevice.h"

class WinApplication
{
private:
    HWND _hWnd = nullptr;
    D3D12GfxDevice* _gfxDevice;

public:
    int mWidth = 1280;
    int mHeight = 720;
    std::wstring mTitle;

public:
    WinApplication(int width, int height, std::wstring title);
    ~WinApplication();

    void SethWnd(const HWND hWnd);
    HWND GethWnd() const;
    bool Initialize();
    void Update();
    void Terminate();

    const wchar_t* GetTitle() const         { return mTitle.c_str(); }
    D3D12GfxDevice* GetGfxDevice() const    { return _gfxDevice; }

    // ------------------------ static ------------------------
public:
    static int Run(WinApplication* app, HINSTANCE hInstance, int nCmdShow);
protected:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    static bool InitializeApplication(WinApplication* app);
    static bool UpdateApplication(WinApplication* app);
    static void TerminateApplication(WinApplication* app);
    // --------------------------------------------------------
};
