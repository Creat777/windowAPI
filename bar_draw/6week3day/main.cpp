#include <windows.h>
#include "GameManager.h"
#include "Block.h"

// windowAPI�� �����ϴ� ������ directX�� �ٷ�� ���� �߰������̴�
// ���α׷����߿��� windowAPI�� c��Ÿ�� �̰� C++�� MFC�� �ַ� ����Ѵ�
// ���� �߼��� ���α׷� ���߿� C#(.net)�� �ַ� ����Ѵ�.

// ���̺귯�� �߰�
#pragma comment(lib, "msimg32.lib")

cGameManager g_GameMng;

// CALLBACK : �Լ�ȣ�� �Ծ� 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //�ݹ� �Լ� - ������ �޽��� ó�� �Լ�

// windowAPI�� �ڵ����� �Լ�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	// ������Ʈ��
	WNDCLASS wcex = { 0, };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // ������ �޽����� �ö� ȣ�� ����
	wcex.hInstance = hInstance; //hWnd �ڵ�, ������ - �����
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // ������ �� ����
	wcex.lpszClassName = L"winclass"; 
	RegisterClass(&wcex);

	HWND hWnd = CreateWindow(
		L"winclass", L"�⺻ ������", WS_OVERLAPPEDWINDOW,
		500, 100, 1024, 768,
		nullptr, NULL, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);

	MSG msg; //& �ּҿ�����

	// WM_QUIT �� ���ƿ��� â�� ������.

	while (GetMessage(&msg, nullptr, 0, 0) == true)
	{
		TranslateMessage(&msg); //Ű���� ���� ���� ó��
		DispatchMessage(&msg); // WndProcȣ������ �ϰ� ��û
	}

	return (int)msg.wParam;
}

/*
void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit)
{
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, hBit);

	BITMAP bitmap;
	GetObject(hBit, sizeof(BITMAP), &bitmap);

	BitBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY);

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);

	ReleaseDC(hWnd, hDC);
}
*/

/*
void DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit)
{
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, hBit);

	BITMAP bitmap;
	GetObject(hBit, sizeof(BITMAP), &bitmap);

	// bitmap���� ����� �����ϴ� �Լ�
	TransparentBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight,
		hTempDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);

	ReleaseDC(hWnd, hDC);
}
*/

HBITMAP hBackBitmap; //test.bmp
HBITMAP hBarBitmap;  //bar.bmp
HBITMAP hBallBitmap;  //ball.bmp

HANDLE hTimer;

HBITMAP hBitMapBuffer;

RECT crt;

#define TIMERID 1


/*
//���� �����̴� �Լ�
void BallMove()
{
	if (eDirX == eRIGHT)
	{
		g_Ball_X += 5;
		if (g_Ball_X > (1024 - 39)) // 39�� ���� x�� ũ��
		{
			eDirX = eLEFT;
		}
	}
	else if (eDirX == eLEFT)
	{
		g_Ball_X -= 5;
		if (g_Ball_X < (0))
		{
			eDirX = eRIGHT;
		}
	}

	if (eDirY == eDOWN)
	{
		g_Ball_Y += 5;
		if (g_Ball_Y > (768 - 39)) // 39�� ���� x�� ũ��
		{
			eDirY = eUP;
		}
	}
	else if (eDirY == eUP)
	{
		g_Ball_Y -= 5;
		if (g_Ball_Y < (0))
		{
			eDirY = eDOWN;
		}
	}
}
*/

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	HDC MemDC;
	PAINTSTRUCT ps;

	BITMAP bit;
	int bx, by;
	
	switch (message)
	{
	case WM_CREATE:
	{
		RECT window_rc;

		// ������ �ٱ��� �������� ���
		GetWindowRect(hWnd, &window_rc);

		// �� ����ϴ� â�� ũ��
		GetClientRect(hWnd, &crt);

		int delta_x = (window_rc.right - window_rc.left) - crt.right;
		int delta_y = (window_rc.bottom - window_rc.top) - crt.bottom;

		SetWindowPos(hWnd, 0, 0, 0, 1024 + delta_x, 768 + delta_y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		// �̹��� ��������
		hBackBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBarBitmap = (HBITMAP)LoadImage(NULL, L"Bar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBallBitmap = (HBITMAP)LoadImage(NULL, L"Ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		g_GameMng.CreateBlock();

		hTimer = (HANDLE)SetTimer(hWnd, TIMERID, 1, NULL);

		GetClientRect(hWnd, &crt);
	}
	break;
	
	case WM_TIMER:
	{
		g_GameMng.BallMove();
		g_GameMng.CollisionBallAndBar();

		HDC hDC = GetDC(hWnd);

		HDC hBufferDC = CreateCompatibleDC(hDC);

		if (hBitMapBuffer == NULL)
		{
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); // ��ȭ��
		}

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer);

		// �׸���
		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, 0, 0, hBackBitmap);
		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, g_GameMng.m_Bar_X, g_GameMng.m_Bar_Y, hBarBitmap);
		g_GameMng.DrawBitmapToBufferColorKey(hWnd, hBufferDC, g_GameMng.m_Ball_X, g_GameMng.m_Ball_Y, hBallBitmap);

		g_GameMng.DrawBlock(hWnd, hBufferDC);

		SelectObject(hBufferDC, hOldBitmap);
		DeleteDC(hBufferDC);

		ReleaseDC(hWnd, hDC);

		InvalidateRect(hWnd, NULL, true); //WM_PAINT ��û
	}
	break;

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT: g_GameMng.m_Bar_X -= 10; break;
		case VK_RIGHT: g_GameMng.m_Bar_X += 10; break;
		default: break;
		}
	}
	break;

	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		{
			HBITMAP hOldBit;

			MemDC = CreateCompatibleDC(hdc);

			hOldBit = (HBITMAP)SelectObject(MemDC, hBitMapBuffer);

			BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDC, 0, 0, SRCCOPY);

			SelectObject(MemDC, hOldBit);

			DeleteDC(MemDC);
		}

		EndPaint(hWnd, &ps);
		return 0;
	}
	break;

	case WM_ERASEBKGND:
	{
		return true;
	}
	break;
	
	case WM_DESTROY:
	{
		DeleteObject(hBackBitmap);
		DeleteObject(hBallBitmap);
		DeleteObject(hBarBitmap);

		DeleteObject(hBitMapBuffer);

		KillTimer(hWnd, TIMERID);

		PostQuitMessage(0); // �޽��� ť�� WM_QUIT�� ����.
		return 0;
	}
	break;
		

	default: break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}





