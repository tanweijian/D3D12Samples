#include <Windows.h>

#include "WinApplication.h"

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE 、hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WinApplication app;
    return Application::Run(app, hInstance, L"", nCmdShow);
}