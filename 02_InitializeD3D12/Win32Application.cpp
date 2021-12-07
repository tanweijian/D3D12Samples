#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include <stdexcept>

#include "Include/log.h"
#include "Win32Application.h"

using namespace Microsoft::WRL;

bool Win32Application::_forceWarp = false;

HWND Win32Application::_hwnd = nullptr;

HWND Win32Application::GetHwnd()
{
    return _hwnd;
}

int WINAPI Win32Application::Run(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);  // 窗口类结构体的内存大小
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;  // 当窗口水平和竖直方向大小发生变化时候重绘窗口
    wc.lpfnWndProc = WindowProc;  // 窗口过程函数指针
    wc.hInstance = hInstance;  // 窗口实例句柄
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // 光标
    wc.lpszClassName = L"InitializeD3D12";  // 标识窗口类的字符串名称
    wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);  // 防止背景重绘

    RegisterClassExW(&wc);  // 向操作系统注册窗口类

    RECT rect = { 0, 0, 1280, 720 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);  // 计算窗口矩形大小

    LONG width = rect.right - rect.left;  // 宽度
    LONG height = rect.bottom - rect.top;  // 高度

    // 创建窗口
    _hwnd = CreateWindowExW(
        WS_EX_LEFT,           // 窗口可选行为 (https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles)
        wc.lpszClassName,     // 要创建窗口类的名称
        L"InitializeD3D12",   // 窗口文本，如果窗口显示标题，则会显示窗口文本标题
        WS_OVERLAPPEDWINDOW,  // 窗口样式 (https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles)
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        width,
        height,
        nullptr,       // 父级窗口
        nullptr,       // 菜单
        hInstance,     // 窗口实例句柄
        nullptr        // 额外需要传递窗口过程的数据指针
    );

    if (_hwnd == nullptr)
    {
        DebugPrint(L"can not create window.\n");
        return -1;
    }

    // init pipeline start ---------------------------
    UINT dxgiFactoryFlags = 0;
    HRESULT hr = E_FAIL;

#if defined(_DEBUG)
    ComPtr<ID3D12Debug> debugController;
    hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));
    if (FAILED(hr))
    {
        DebugPrintf(L"d3d12: failed create debug layer (0x%08X).\n", hr);
        return -1;
    }
    debugController->EnableDebugLayer();
    dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

    // factory
    ComPtr<IDXGIFactory7> factory;
    hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));
    if (FAILED(hr))
    {
        DebugPrintf(L"d3d12: failed create dxgi factory (0x%08X).\n", hr);
        return -1;
    }

    // device
    ComPtr<ID3D12Device8> device;
    if (Win32Application::_forceWarp)
    {
        ComPtr<IDXGIAdapter4> adapter;
        hr = factory->EnumWarpAdapter(IID_PPV_ARGS(&adapter));
        if (FAILED(hr))
        {
            DebugPrintf(L"d3d12: failed enum warp adapter (0x%08X).\n", hr);
            return -1;
        }
        hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device));
        if (FAILED(hr))
        {
            DebugPrintf(L"d3d12: failed create warp device (0x%08X).\n", hr);
            return -1;
        }
    }
    else
    {
        ComPtr<IDXGIAdapter4> adapter;
        UINT adapterIndex = 0;
        while (SUCCEEDED(factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter))))
        {
            adapterIndex++;
            DXGI_ADAPTER_DESC3 desc;
            adapter->GetDesc3(&desc);
            if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            {
                // don't select the basic render driver adapter.
                continue;
            }
            // check to see whether the adapter supports Direct3D 12, but don't create the actual device yet.
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, __uuidof(ID3D12Device8), nullptr)))
            {
                break;
            }
        }
        if (adapter.Get() == nullptr)
        {
            DebugPrint(L"d3d12: failed created hardware adapter.\n");
            return -1;
        }
        hr = D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device));
        if (FAILED(hr))
        {
            DebugPrintf(L"d3d12: failed created hardware device (0x%08X).\n", hr);
            return -1;
        }
    }

    // command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    ComPtr<ID3D12CommandQueue> commandQueue;
    hr = device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&commandQueue));
    if (FAILED(hr))
    {
        DebugPrintf(L"d3d12: failed created command queue (0x%08X).\n", hr);
        return -1;
    }

    // swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    swapChainDesc.Width = 1280;
    swapChainDesc.Height = 720;
    swapChainDesc.SampleDesc.Count = 1;   // dx12 does not support msaa swap chains
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Stereo = FALSE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
    swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
    swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    ComPtr<IDXGISwapChain1> swapChain1;
    hr = factory->CreateSwapChainForHwnd(commandQueue.Get(), Win32Application::GetHwnd(), &swapChainDesc, nullptr, nullptr, &swapChain);
    if (FAILED(hr))
    {
        DebugPrintf(L"d3d12: failed created swap chain (0x%08X).\n", hr);
        return -1;
    }
    ComPtr<IDXGISwapChain4> swapChain;
    hr = swapChain1.As(&swapChain);
    if (FAILED(hr))
    {
        DebugPrintf(L"d3d12: failed transfer IDXGISwapChain1 to IDXGISwapChain4 (0x%08X).\n", hr);
        return -1;
    }

    // init pipeline end -----------------------------

    // 显示窗口
    ShowWindow(_hwnd, nCmdShow);

    // 处理窗口信息
    MSG msg = { 0 };
    while (msg.message != WM_QUIT)
    {
        if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))  // 从消息队列中拉去消息
        {
            TranslateMessage(&msg);  // 输入转换
            DispatchMessageW(&msg);  // 分发消息
        }
    }

    return 0;
}

LRESULT CALLBACK Win32Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
    {
        return 0;
    }
    case WM_DESTROY:  // 关闭窗口
    {
        PostQuitMessage(0);
        return 0;
    }
    case WM_PAINT:  // 绘制窗口
    {
        return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
