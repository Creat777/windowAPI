#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //�ݹ� �Լ� - ������ �޼��� ó�� �Լ�

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	//������Ʈ�� ���
	WNDCLASS wcex = { 0, };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // window�޼����� �� �� ȣ������
	wcex.hInstance = hInstance; // hwnd �ڵ�(������)
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ������ �� ����
	wcex.lpszClassName = L"winclass"; // L :  ���̵��� ĳ����(wchar_t)
	RegisterClass(&wcex);

	//wchar_t* wstr = L"hello"; // ���� 1���� 2����Ʈ (�����ڵ� ����� ����)

	HWND hWnd = CreateWindow(
		L"winclass", L"�⺻ ������", WS_OVERLAPPEDWINDOW,
		500, 100, 1024, 768,
		nullptr, NULL, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);

	MSG msg; 

	// & : �ּҿ�����
	// WM_QUIT�� �޼����� ���ƿ��� �����
	while (GetMessage(&msg, nullptr, 0, 0) == true)
	{
		TranslateMessage(&msg); // Ű���� ���� ���� ó��
		DispatchMessage(&msg); // WndProcȣ������ ��� �ü��(window)�� ��û
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lparam)
{
	switch (message)
	{
	case WM_DESTROY :
		PostQuitMessage(0); // �޼��� ť�� WM_QUIT�� ����.
		return NULL;
	}

	return DefWindowProc(hWnd, message, wParam, lparam);
}
