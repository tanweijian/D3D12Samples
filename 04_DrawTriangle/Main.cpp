#include <Windows.h>

#include "WinApplication.h"

_Use_decl_annotations_
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    WinApplication app(1280, 720, L"DrawTriangle");
    return Application::Run(app, hInstance, app.GetName().c_str(), nCmdShow);
}