#include <windows.h>

// ���̺귯�� �߰�
#pragma comment(lib, "msimg32.lib")

// LRESULT(Long Result) : Windows API���� �޽��� ó�� �Լ�(WndProc)�� ��ȯ�ϴ� ���� ������ Ÿ��
// CALLBACK : �Լ� ȣ�� �Ծ��� ��Ÿ����, Windows API���� ����ϴ� ǥ�� ȣ�� �Ծ�
// == __stdcall -> ��ȣ���ڰ� ������ ����, ���޹��� �Ű������� ������ �ִ� �ͺ��� �Ű����� ���ؿ� ����
// �Ű�����
//	->	HWND(Handle to a Window): ������ �ڵ�(�����츦 �ĺ��ϴ� ���� ��).
//			#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
//			## : ��� ���� ������ -> DECLARE_HANDLE(MyHandle)�� ȣ���ϸ�, struct MyHandle__�̶�� �̸��� ����ü�� ����
//			typedef struct name##__ *name -> ��ũ�� �Ű������� ���� MyHandle�� �״�� ����ü�� �����͸� ����� �ڷ������� ����
//			�� ������ �ڵ��� ���Ӱ� ������ ����ü�� ������
// 
//		UINT(Unsigned Integer): �޽����� ������ ��Ÿ���ϴ�.
//		WPARAM(Word Parameter) : �޽����� ���õ� ���� ũ���� ������ �����ͳ� **�ڵ�(hWnd)**�� �����ϴ� �� ���
//		LPARAM(Long Parameter) : �޽����� ���õ� **�߰� ����(�ּҳ� ����ü)**�� �����ϴ� �� ���

// ���������ν��� �Լ� ����
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //�ݹ� �Լ� - ������ �޼��� ó�� �Լ�

// ������api ���� �Լ�(�ڵ�ȣ��)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	// WNDCLASS(Window Class) : ������Ʈ�� ���
	// ��ϵ� ������ Ŭ������ RegisterClass �Լ��� ���� �ý��ۿ� ��ϵǸ�, ���� �� Ŭ������ ������� �� �����츦 ������ �� ����
	WNDCLASS wcex = { 0, };

	wcex.style = CS_HREDRAW | CS_VREDRAW; // �������� ��Ÿ���� ����. ��: CS_HREDRAW (���� ũ�� ���� �� �ٽ� �׸�).
	wcex.lpfnWndProc = WndProc; // (Long Pointer to Function Window Procedure) ������ �޽����� ó���� �ݹ� �Լ�
	wcex.hInstance = hInstance; // (Handle to Instance) hwnd �ڵ�(������)
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
void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hbit)
{
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hDC, hbit);

	BITMAP bitmap;

	// �׸��� ������ �����Ͽ� bitmap�� ����
	GetObject(hbit, sizeof(BITMAP), &bitmap);

	// ȭ�鿡 �׸��� �Լ�
	BitBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY); // SRCCOPY : ��ü ī��

	SelectObject(hTempDC, hOldBit); // �������� ����
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

	// bitmap���� ����� �����ϰ� �׸��� �Լ�
	TransparentBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight,
		hTempDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);

	ReleaseDC(hWnd, hDC);
}

// �׸�
HBITMAP hBackBitmap; // test.bmp
HBITMAP hBarBitmap; // bar.bmp
HBITMAP hBallBitmap;  //ball.bmp

HANDLE hTimer;

// ����� (ȭ�� ����) -> ���� �׸��� ����ۿ� ����
// ����۴� ����Ʈ���ۿ� �� �����Ӹ��� �����͸� ��ȯ�ϸ鼭 ����Ʈ���۰� ȭ�鿡 ��µ�
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

// ������ ���ν��� �Լ� ����
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lparam)
{
	/*
	HDC hdc; // ȭ�� ����(����Ʈ����)
	HDC MemDC; // (�����)

	PAINTSTRUCT ps;

	// lp : ��������
	LPPAINTSTRUCT ptr_ps = &ps;

	// h : �ڵ�
	HBITMAP hMyFileBitmap, hOldFilebitmap;
	BITMAP bitmap_image_info;
	int width, height;
	*/
	HDC hdc;
	HDC MemDc;
	PAINTSTRUCT ps;

	BITMAP bit;
	int bx, by;

	switch (message)
	{
		// ������ ������ ���ƿ��� �޼���
	case WM_CREATE:
	{
		// �̹��� ��������
		hBackBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBarBitmap = (HBITMAP)LoadImage(NULL, L"Bar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBallBitmap = (HBITMAP)LoadImage(NULL, L"Ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// �����ð�����  WM_TIMER�� �޼��� ť�� �������� ��û
		hTimer = (HANDLE)SetTimer(hWnd, TIMERID, 1, NULL);

		// ClientRect : window�� ���� �簢�� ������ ��ȯ�Ͽ� crt�� �Ҵ�
		GetClientRect(hWnd, &crt);
	}
	break;
	

	// SetTimer�� �����ð����� �޼���ť�� ��
	case WM_TIMER:
	{
		BallMove();

		HDC hDC = GetDC(hWnd);

		// MemDC�� ���� ����
		// Compatible -> �Ű������� ���� DC�� ȣȯ�Ǵ� DC�� ����
		HDC hBufferDC = CreateCompatibleDC(hDC);

		if (hBitMapBuffer == NULL)
		{
			// ������ ��Ʈ�� ����
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); // ��ȭ�� �����Ͽ� ����
		}
		
		// SelectObject : ȭ��� ��Ʈ���� �����Ͽ� ȭ�鿡 ���̰� ����, ���� ���¸� ��ȯ��
		// ���⼭�� ��ȭ��(hBitMapBuffer)�� ����
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer);

		//�׸���
		DrawBitmapToBuffer(hWnd, hBufferDC, 0, 0, hBackBitmap);
		DrawBitmapToBuffer(hWnd, hBufferDC, g_Bar_X, 0, hBarBitmap);
		DrawBitmapToBufferColorKey(hWnd, hBufferDC, g_Ball_X, g_Ball_Y, hBallBitmap);

		// �������� ����
		SelectObject(hBufferDC, hOldBitmap); 

		// ȣȯ�Ǵ� HDC����
		DeleteDC(hBufferDC); // hBufferDC ��ȯ

		// �����쿡 ����� �ݳ�
		ReleaseDC(hWnd, hDC);

		InvalidateRect(hWnd, NULL, true); // WM_PAINT�� OS�� ��û
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

	// ȭ���� ������ ������ �޼���ť�� ��
	case WM_PAINT :
	{
		/*
		hdc = BeginPaint(hWnd, &ps);

		MemDC = CreateCompatibleDC(hdc);

		hMyFileBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// hMyFileBitmap�� ��ȯ, �������� ��ȯ
		hOldFilebitmap = (HBITMAP)SelectObject(MemDC, hMyFileBitmap); 

		// �о�� �̹����� ������ bitmap_image_info�� ����
		GetObject(hMyFileBitmap, sizeof(BITMAP), &bitmap_image_info);

		// �̹����� ���μ��� ����
		width = bitmap_image_info.bmWidth;
		height = bitmap_image_info.bmHeight;

		BitBlt(hdc, 0, 0, width, height, MemDC, 0, 0, SRCCOPY);

		// ���� ���� ����
		SelectObject(MemDC, hOldFilebitmap); 

		DeleteObject(hMyFileBitmap);
		DeleteObject(MemDC);

		EndPaint(hWnd, &ps);
	*/
		hdc = BeginPaint(hWnd, &ps);
		{
			HBITMAP hOldBit;

			MemDc = CreateCompatibleDC(hdc);

			hOldBit = (HBITMAP)SelectObject(MemDc, hBitMapBuffer);

			BitBlt(hdc, 0, 0, crt.right, crt.bottom, MemDc, 0, 0, SRCCOPY);

			SelectObject(MemDc, hOldBit);

			DeleteDC(MemDc);
		}

		EndPaint(hWnd, &ps);

		return NULL;
	}
	break;

	case WM_ERASEBKGND:
	{
		return true;
	}
	break;
		

	case WM_DESTROY : // x��ư Ŭ�����ϴ� ���
	{
		PostQuitMessage(0); // �޼��� ť�� WM_QUIT�� ����.
		return NULL;
	}
	break;

	default: break;
	}

	// (Default Window Procedure) : 
	//		����ڰ� ó������ ���� �޽����� ó���ϱ� ���� ȣ��
	//		�޽����� ���� �⺻ ����(��: â �̵�, ũ�� ����, �ݱ� ��)�� ����
	return DefWindowProc(hWnd, message, wParam, lparam);
}
