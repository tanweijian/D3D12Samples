#include <Windows.h>

#include "WinApplication.h"

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WinApplication app(1280, 720, L"Draw Triangle");
    return WinApplication::Run(&app, hInstance, nCmdShow);
}