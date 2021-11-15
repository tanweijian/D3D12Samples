#include <Windows.h>

#include "Win32Application.h"

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    Win32Application::Run(hInstance, hPrevInstance, pCmdLine, nCmdShow);
    return 0;
}