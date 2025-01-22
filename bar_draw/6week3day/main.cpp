#include <windows.h>

// 라이브러리 추가
#pragma comment(lib, "msimg32.lib")

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

HBITMAP hBackBitmap; //test.bmp
HBITMAP hBarBitmap;  //bar.bmp
HBITMAP hBallBitmap;  //ball.bmp

HANDLE hTimer;

HBITMAP hBitMapBuffer;

RECT crt;

int g_Bar_X = 200;

enum BALL_DIR_X
{
	eRIGHT,
	eLEFT
};

enum BALL_DIR_Y
{
	eDOWN,
	eUP
};

int g_Ball_X = 0;
int g_Ball_Y = 100;

#define TIMERID 1

BALL_DIR_X eDirX = eRIGHT;
BALL_DIR_Y eDirY = eDOWN;

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
		// 이미지 가져오기
		hBackBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBarBitmap = (HBITMAP)LoadImage(NULL, L"Bar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBallBitmap = (HBITMAP)LoadImage(NULL, L"Ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		hTimer = (HANDLE)SetTimer(hWnd, TIMERID, 1, NULL);

		GetClientRect(hWnd, &crt);
	}
	break;
	
	case WM_TIMER:
	{
		BallMove();

		HDC hDC = GetDC(hWnd);

		HDC hBufferDC = CreateCompatibleDC(hDC);

		if (hBitMapBuffer == NULL)
		{
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); // 빈화면
		}

		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer);

		// 그리기
		DrawBitmapToBuffer(hWnd, hBufferDC, 0, 0, hBackBitmap);
		DrawBitmapToBuffer(hWnd, hBufferDC, g_Bar_X, 680, hBarBitmap);
		DrawBitmapToBufferColorKey(hWnd, hBufferDC, g_Ball_X, g_Ball_Y, hBallBitmap);

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
		case VK_LEFT: g_Bar_X -= 10; break;
		case VK_RIGHT: g_Bar_X += 10; break;
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
		PostQuitMessage(0); // 메시지 큐에 WM_QUIT가 들어간다.
		return 0;
	}
	break;
		

	default: break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}





