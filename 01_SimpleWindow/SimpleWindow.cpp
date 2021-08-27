#ifndef UNICODE
#define UNICODE
#endif // !UNICODE

#include "SimpleWindow.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	// ����������
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };
	wc.lpfnWndProc = WindowProc;  // ���ڹ��̺���ָ��
	wc.hInstance = hInstance;  // ����ʵ�����
	wc.lpszClassName = CLASS_NAME;  // ��ʶ��������ַ�������

	RegisterClass(&wc);  // �����ϵͳע�ᴰ����

	// ��������
	HWND hwnd = CreateWindowEx(
		WS_EX_LEFT,   // ���ڿ�ѡ��Ϊ (https://docs.microsoft.com/en-us/windows/win32/winmsg/extended-window-styles)
		CLASS_NAME,        // Ҫ���������������
		L"Simple Window",    // �����ı������������ʾ���⣬�����ʾ�����ı�����
		WS_OVERLAPPEDWINDOW,      // ������ʽ (https://docs.microsoft.com/en-us/windows/win32/winmsg/window-styles)
		CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, // ���ô��ڴ�С��λ��
		NULL,       // ��������
		NULL,       // �˵�
		hInstance,  // ����ʵ�����
		nullptr        // ������Ҫ���ݴ��ڹ��̵�����ָ��
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	// ��ʾ����
	ShowWindow(hwnd, nCmdShow);

	// ��������Ϣ
	MSG msg = { };
	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))  // ����Ϣ��������ȥ��Ϣ
		{
			TranslateMessage(&msg);  // ����ת��
			DispatchMessage(&msg);  // �ַ���Ϣ
		}
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:  // �رմ���
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_PAINT:  // ���ƴ���
	{
		PAINTSTRUCT ps;  // ������Ϣ���ݽṹ
		HDC hdc = BeginPaint(hwnd, &ps);  // ��������
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));  // ��乤����
		EndPaint(hwnd, &ps);  // ��������
	}
	return 0;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
