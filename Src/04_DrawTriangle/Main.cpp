#include <Windows.h>

#include "WinApplication.h"

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WinApplication app;
    app.Width = 1280;
    app.Height = 720;
    app.Name = L"Draw Triangle";
    return WinApplication::Run(app, hInstance, app.Name.c_str(), nCmdShow);
}