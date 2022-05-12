#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>

#include "WinApplication.h"
#include "spdlog/spdlog.h"

using namespace Microsoft::WRL;

WinApplication::WinApplication(int width, int height, std::wstring title)
{
    _width = width;
    _height = height;
    _title = title;
    _gfxDevice = new D3D12GfxDevice();
}

WinApplication::~WinApplication()
{
    delete _gfxDevice;
    _gfxDevice = nullptr;
}

void WinApplication::SethWnd(const HWND hWnd)
{
    _hWnd = hWnd;
}

HWND WinApplication::GethWnd() const
{
    return _hWnd;
}

bool WinApplication::Initialize()
{
    UINT dxgiFactoryFlags = 0;
#if _DEBUG
    ComPtr<ID3D12Debug> debugLayer;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer))))
    {
        debugLayer->EnableDebugLayer();
        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
    else
    {
        return false;
    }
#endif

    ComPtr<IDXGIFactory> dxgiFactory;
    if (SUCCEEDED(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&dxgiFactory))))
    {

    }
    else
    {
        return false;
    }
    return true;
}

void WinApplication::Update()
{
}

void WinApplication::Terminate()
{
}

// ------------------------ static ------------------------

int WinApplication::Run(WinApplication* app, HINSTANCE hInstance, int nCmdShow)
{
#if _DEBUG
    AllocConsole();
#endif

    // Register class
    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = app->GetTitle();
    wcex.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
    RegisterClassEx(&wcex);

    int width = app->GetWidth();
    int height = app->GetHeight();
    HWND hWnd = CreateWindow(app->GetTitle(), app->GetTitle(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, nullptr, nullptr, hInstance, nullptr);
    app->SethWnd(hWnd);

    InitializeApplication(app);
    SPDLOG_INFO("测试");
    ShowWindow(hWnd, nCmdShow);

    do
    {
        MSG msg = {};
        bool exit = false;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))  // 从消息队列中拉去消息
        {
            TranslateMessage(&msg);  // 输入转换
            DispatchMessageW(&msg);  // 分发消息

            if (msg.message == WM_QUIT)
            {
                exit = true;
            }
        }
        if (exit)
        {
            break;
        }
    } while (UpdateApplication(app));

    TerminateApplication(app);

#if _DEBUG
    FreeConsole();
#endif

    return 0;
}

LRESULT WinApplication::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}

bool WinApplication::InitializeApplication(WinApplication* app)
{
    return app->Initialize();
}


bool WinApplication::UpdateApplication(WinApplication* app)
{
    app->Update();
    return true;
}

void WinApplication::TerminateApplication(WinApplication* app)
{
    app->Terminate();
}
