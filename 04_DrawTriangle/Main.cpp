#include <Windows.h>

#include "WinApplication.h"

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WinApplication app;
    app.SetWinSize(1280, 720);
    return Application::Run(app, hInstance, L"DrawTriangle", nCmdShow);
}