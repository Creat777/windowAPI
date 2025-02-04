#include <windows.h>
#include "GameManager.h"
#include "Block.h"

// windowAPI를 공부하는 이유는 directX를 다루기 위한 중간과정이다
// 프로그램개발에서 windowAPI는 c스타일 이고 C++은 MFC를 주로 사용한다
// 요즘 추세는 프로그램 개발에 C#(.net)을 주로 사용한다.

// 라이브러리 추가
#pragma comment(lib, "msimg32.lib")

cGameManager g_GameMng;

// CALLBACK : 함수호출 규약 
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //콜백 함수 - 윈도우 메시지 처리 함수

// windowAPI의 자동실행 함수
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

	// bitmap에서 흰색을 제거하는 함수
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
//공을 움직이는 함수
void BallMove()
{
	if (eDirX == eRIGHT)
	{
		g_Ball_X += 5;
		if (g_Ball_X > (1024 - 39)) // 39는 공의 x축 크기
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
		if (g_Ball_Y > (768 - 39)) // 39는 공의 x축 크기
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

		// 윈도우 바깥쪽 프레임을 계산
		GetWindowRect(hWnd, &window_rc);

		// 실 사용하는 창의 크기
		GetClientRect(hWnd, &crt);

		int delta_x = (window_rc.right - window_rc.left) - crt.right;
		int delta_y = (window_rc.bottom - window_rc.top) - crt.bottom;

		SetWindowPos(hWnd, 0, 0, 0, 1024 + delta_x, 768 + delta_y, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		// 이미지 가져오기
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
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); // 빈화면
		}

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer);

		// 그리기
		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, 0, 0, hBackBitmap);
		g_GameMng.DrawBitmapToBuffer(hWnd, hBufferDC, g_GameMng.m_Bar_X, g_GameMng.m_Bar_Y, hBarBitmap);
		g_GameMng.DrawBitmapToBufferColorKey(hWnd, hBufferDC, g_GameMng.m_Ball_X, g_GameMng.m_Ball_Y, hBallBitmap);

		g_GameMng.DrawBlock(hWnd, hBufferDC);

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

		PostQuitMessage(0); // 메시지 큐에 WM_QUIT가 들어간다.
		return 0;
	}
	break;
		

	default: break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}





