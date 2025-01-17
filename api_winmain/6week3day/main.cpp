#include <windows.h>

// LRESULT : Long Result (Windows API���� �޽��� ó�� �Լ�(WndProc)�� ��ȯ�ϴ� ���� ������ Ÿ��)
// CALLBACK : �Լ� ȣ�� �Ծ��� ��Ÿ����, Windows API���� ����ϴ� ǥ�� ȣ�� �Ծ��Դϴ�.
// �Ű�����
//	->	HWND: ������ �ڵ�(�����츦 �ĺ��ϴ� ���� ��).
//		UINT: �޽����� ������ ��Ÿ���ϴ�.
//		WPARAM : Word PARAMeter -> �޽����� ���õ� ���� ũ���� ������ �����ͳ� **�ڵ�(hWnd)**�� �����ϴ� �� ���
//		LPARAM : Long PARAMeter -> �޽����� ���õ� **�߰� ����(�ּҳ� ����ü)**�� �����ϴ� �� ���

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
	HDC hdc; // ȭ�� ����(����Ʈ����)
	HDC MemDC; // (�����)
	PAINTSTRUCT ps;
	// lp : ��������
	LPPAINTSTRUCT ptr_ps = &ps;

	// h : �ڵ�
	HBITMAP hMyFileBitmap, hOldFilebitmap;
	BITMAP bitmap_image_info;
	int width, height;

	switch (message)
	{
	case WM_PAINT :
	{
		
		hdc = BeginPaint(hWnd, &ps);

		MemDC = CreateCompatibleDC(hdc);

		hMyFileBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		hOldFilebitmap = (HBITMAP)SelectObject(MemDC, hMyFileBitmap); // hMyFileBitmap�� ��ȯ, �������� ����

		GetObject(hMyFileBitmap, sizeof(BITMAP), &bitmap_image_info);

		width = bitmap_image_info.bmWidth;
		height = bitmap_image_info.bmHeight;

		BitBlt(hdc, 0, 0, width, height, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hOldFilebitmap); // ���� ���� ����

		DeleteObject(hMyFileBitmap);
		DeleteObject(MemDC);

		EndPaint(hWnd, &ps);
		
		return NULL;
	}

	case WM_DESTROY : // x��ư Ŭ�����ϴ� ���
	{
		PostQuitMessage(0); // �޼��� ť�� WM_QUIT�� ����.
		return NULL;
	}
		
	}

	return DefWindowProc(hWnd, message, wParam, lparam);
}
