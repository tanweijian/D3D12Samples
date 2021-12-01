#include "Win32Application.h"

HWND Win32Application::_hwnd = nullptr;

HWND Win32Application::GetHwnd()
{
    return _hwnd;
}

int WINAPI Win32Application::Run(HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASSEXW wc = { 0 };
    wc.cbSize = sizeof(WNDCLASSEXW);  // 窗口类结构体的内存大小
    wc.style = CS_HREDRAW | CS_VREDRAW;  // 当窗口水平和竖直方向大小发生变化时候重绘窗口
    wc.lpfnWndProc = WindowProc;  // 窗口过程函数指针
    wc.hInstance = hInstance;  // 窗口实例句柄
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // 光标
    wc.lpszClassName = L"D3D12";  // 标识窗口类的字符串名称

    RegisterClassExW(&wc);  // 向操作系统注册窗口类

    RECT rect = { 0, 0, 1280, 720 };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);  // 计算窗口矩形大小

    LONG width = rect.right - rect.left;  // 宽度
    LONG height = rect.bottom - rect.top;  // 高度

    // 创建窗口
    _hwnd = CreateWindowExW(
        WS_EX_LEFT,           // 窗口可选行为 (https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles)
        wc.lpszClassName,     // 要创建窗口类的名称
        L"D3D12",             // 窗口文本，如果窗口显示标题，则会显示窗口文本标题
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
        return -1;
    }

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

    return static_cast<int>(msg.wParam);
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
