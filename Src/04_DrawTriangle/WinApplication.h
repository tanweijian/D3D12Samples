#pragma once

#include <string>
#include <Windows.h>

#include "D3D12GfxContext.h"
#include "D3D12GfxDevice.h"

class WinApplication
{
private:
    HWND _hWnd = nullptr;
    D3D12GfxDevice* _gfxDevice;

public:
    WinApplication(int width, int height, std::wstring title);
    ~WinApplication();
    int _width = 1280;
    int _height = 720;
    std::wstring _title;

    void SethWnd(const HWND hWnd);
    HWND GethWnd() const;
    bool Initialize();
    void Update();
    void Terminate();

    int GetWidth() const                    { return _width; }
    int GetHeight() const                   { return _height; }
    const wchar_t* GetTitle() const         { return _title.c_str(); }
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
