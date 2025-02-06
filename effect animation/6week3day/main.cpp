#include <windows.h>
#include <gdiplus.h>

#include "Block.h"
#include "Effect.h"
#include "GameManager.h"

#pragma comment(lib, "msimg32.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

cGameManager g_GameMng;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //콜백 함수 - 윈도우 메시지 처리 함수

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	// 레지스트리
	WNDCLASS wcex = { 0, };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // 윈도우 메시지가 올때 호출 해줘
	wcex.hInstance = hInstance; //hWnd 핸들, 포인터 - 제어권
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 윈도우 색 지정
	wcex.lpszClassName = L"winclass"; 
	RegisterClass(&wcex);

	HWND hWnd = CreateWindow(
		L"winclass", L"기본 윈도우", WS_OVERLAPPEDWINDOW,
		500, 100, 1024, 768,
		nullptr, NULL, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);

	MSG msg; //& 주소연산자

	// WM_QUIT 가 날아오면 창이 꺼진다.

	while (GetMessage(&msg, nullptr, 0, 0) == true)
	{
		TranslateMessage(&msg); //키보드 관련 내용 처리
		DispatchMessage(&msg); // WndProc호출해줘 하고 요청
	}

	return (int)msg.wParam;
}

HBITMAP hBackBitmap; //test.bmp
HBITMAP hBarBitmap;  //bar.bmp
HBITMAP hBallBitmap;

HANDLE hTimer;

HBITMAP hBitMapBuffer;

RECT crt;

#define TIMERID 1

ULONG_PTR gdiplusToken;
GdiplusStartupInput gdiplusStartupInput;

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
		GetWindowRect(hWnd, &window_rc);

		GetClientRect(hWnd, &crt);

		int delta_X = (window_rc.right - window_rc.left) - crt.right;
		int delta_Y = (window_rc.bottom - window_rc.top) - crt.bottom;

		SetWindowPos(hWnd, 0, 0, 0, 1024 + delta_X, 768 + delta_Y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		hBackBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBarBitmap = (HBITMAP)LoadImage(NULL, L"Bar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBallBitmap = (HBITMAP)LoadImage(NULL, L"Ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		g_GameMng.CreateBlock();

		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		g_GameMng.InitEffect(0, 0);

		hTimer = (HANDLE)SetTimer(hWnd, TIMERID, 1, NULL);

		GetClientRect(hWnd, &crt);
	}
	break;
	
	case WM_TIMER:
	{
		g_GameMng.BallMove();
		g_GameMng.CollisionBallAndBar();
		g_GameMng.CollisionBallAndBlock();

		HDC hDC = GetDC(hWnd);

		HDC hBufferDC = CreateCompatibleDC(hDC);

		if (hBitMapBuffer == NULL)
		{
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); // 빈화면
		}

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer);

		// 그리기
		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, 0, 0, hBackBitmap);
		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, g_GameMng.m_Bar_X, 680, hBarBitmap);
		g_GameMng.DrawBitmapToBufferColorKey(hWnd, hBufferDC, g_GameMng.m_Ball_X, g_GameMng.m_Ball_Y, hBallBitmap);

		g_GameMng.DrawBlock(hWnd, hBufferDC);
		g_GameMng.DrawEffect(hWnd, hBufferDC);

		SelectObject(hBufferDC, hOldBitmap);
		DeleteDC(hBufferDC);

		ReleaseDC(hWnd, hDC);

		InvalidateRect(hWnd, NULL, true); //WM_PAINT 요청
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

	case WM_ERASEBKGND:
		return true;
	
	case WM_DESTROY:

		DeleteObject(hBackBitmap);
		DeleteObject(hBarBitmap);
		DeleteObject(hBallBitmap);

		DeleteObject(hBitMapBuffer);

		KillTimer(hWnd, TIMERID);

		PostQuitMessage(0); // 메시지 큐에 WM_QUIT가 들어간다.
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}





