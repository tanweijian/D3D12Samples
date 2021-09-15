#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include "DXWindow.h"

using namespace Microsoft::WRL;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// 声明窗口类
	const wchar_t CLASS_NAME[] = L"DX12 Class";

	WNDCLASSEX wc = { };
	wc.cbSize = sizeof(WNDCLASSEX);  // 窗口类结构体的内存大小
	wc.style = CS_HREDRAW | CS_VREDRAW;  // 当窗口水平和竖直方向大小发生变化时候重绘窗口
	wc.lpfnWndProc = WindowProc;  // 窗口过程函数指针
	wc.hInstance = hInstance;  // 窗口实例句柄
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);  // 光标
	wc.lpszClassName = CLASS_NAME;  // 标识窗口类的字符串名称

	RegisterClassEx(&wc);  // 向操作系统注册窗口类

	RECT rect = { 0, 0, 1280, 720 };
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);  // 计算窗口矩形大小

	LONG width = rect.right - rect.left;  // 宽度
	LONG height = rect.bottom - rect.top;  // 高度

	// 创建窗口
	HWND hwnd = CreateWindowEx(
		WS_EX_LEFT,   // 窗口可选行为 (https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles)
		CLASS_NAME,        // 要创建窗口类的名称
		L"DX12 Window",    // 窗口文本，如果窗口显示标题，则会显示窗口文本标题
		WS_OVERLAPPEDWINDOW,      // 窗口样式 (https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles)
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		nullptr,       // 父级窗口
		nullptr,       // 菜单
		hInstance,  // 窗口实例句柄
		nullptr        // 额外需要传递窗口过程的数据指针
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// load pipeline
	{
		UINT dxgiFactoryFlags = 0;
#if defined(_DEBUG)
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
#endif
		ComPtr<IDXGIFactory4> factory;
		ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
	}


	// 显示窗口
	ShowWindow(hwnd, nCmdShow);

	// 处理窗口信息
	MSG msg = { };
	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  // 从消息队列中拉去消息
		{
			TranslateMessage(&msg);  // 输入转换
			DispatchMessage(&msg);  // 分发消息
		}
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		PAINTSTRUCT ps;  // 绘制信息数据结构
		HDC hdc = BeginPaint(hwnd, &ps);  // 启动绘制
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));  // 填充工作区
		EndPaint(hwnd, &ps);  // 结束绘制
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
