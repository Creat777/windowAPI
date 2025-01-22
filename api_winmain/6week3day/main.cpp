#include <windows.h>

// 라이브러리 추가
#pragma comment(lib, "msimg32.lib")

// LRESULT(Long Result) : Windows API에서 메시지 처리 함수(WndProc)가 반환하는 값의 데이터 타입
// CALLBACK : 함수 호출 규약을 나타내며, Windows API에서 사용하는 표준 호출 규약
// == __stdcall -> 피호출자가 스택을 정리, 전달받은 매개변수중 우측에 있는 것부터 매개변수 스텍에 쌓음
// 매개변수
//	->	HWND(Handle to a Window): 윈도우 핸들(윈도우를 식별하는 고유 값).
//			#define DECLARE_HANDLE(name) struct name##__{int unused;}; typedef struct name##__ *name
//			## : 토근 연결 연산자 -> DECLARE_HANDLE(MyHandle)를 호출하면, struct MyHandle__이라는 이름의 구조체가 생성
//			typedef struct name##__ *name -> 메크로 매개변수르 던진 MyHandle을 그대로 구조체의 포인터를 만드는 자료형으로 만듬
//			즉 윈도우 핸들은 새롭게 생성된 구조체의 포인터
// 
//		UINT(Unsigned Integer): 메시지의 종류를 나타냅니다.
//		WPARAM(Word Parameter) : 메시지와 관련된 작은 크기의 정수형 데이터나 **핸들(hWnd)**을 전달하는 데 사용
//		LPARAM(Long Parameter) : 메시지와 관련된 **추가 정보(주소나 구조체)**를 전달하는 데 사용

// 윈도우프로시저 함수 선언
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //콜백 함수 - 윈도우 메세지 처리 함수

// 윈도우api 메인 함수(자동호출)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	// WNDCLASS(Window Class) : 레지스트리 등록
	// 등록된 윈도우 클래스는 RegisterClass 함수를 통해 시스템에 등록되며, 이후 이 클래스를 기반으로 새 윈도우를 생성할 수 있음
	WNDCLASS wcex = { 0, };

	wcex.style = CS_HREDRAW | CS_VREDRAW; // 윈도우의 스타일을 정의. 예: CS_HREDRAW (수평 크기 변경 시 다시 그림).
	wcex.lpfnWndProc = WndProc; // (Long Pointer to Function Window Procedure) 윈도우 메시지를 처리할 콜백 함수
	wcex.hInstance = hInstance; // (Handle to Instance) hwnd 핸들(포인터)
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // 윈도우 색 지정
	wcex.lpszClassName = L"winclass"; // L :  와이드형 캐릭터(wchar_t)
	RegisterClass(&wcex);

	//wchar_t* wstr = L"hello"; // 글자 1개가 2바이트 (유니코드 사용을 위함)

	HWND hWnd = CreateWindow(
		L"winclass", L"기본 윈도우", WS_OVERLAPPEDWINDOW,
		500, 100, 1024, 768,
		nullptr, NULL, hInstance, nullptr
	);

	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);

	MSG msg; 

	// & : 주소연산자
	// WM_QUIT이 메세지로 날아오면 종료됨
	while (GetMessage(&msg, nullptr, 0, 0) == true)
	{
		TranslateMessage(&msg); // 키보드 관련 내용 처리
		DispatchMessage(&msg); // WndProc호출해줘 라고 운영체제(window)에 요청
	}

	return (int)msg.wParam;
}
void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hbit)
{
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hDC, hbit);

	BITMAP bitmap;

	// 그림의 정보를 추출하여 bitmap에 저장
	GetObject(hbit, sizeof(BITMAP), &bitmap);

	// 화면에 그리는 함수
	BitBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY); // SRCCOPY : 전체 카피

	SelectObject(hTempDC, hOldBit); // 이전상태 연결
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

	// bitmap에서 흰색을 제거하고 그리는 함수
	TransparentBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight,
		hTempDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);

	ReleaseDC(hWnd, hDC);
}

// 그림
HBITMAP hBackBitmap; // test.bmp
HBITMAP hBarBitmap; // bar.bmp
HBITMAP hBallBitmap;  //ball.bmp

HANDLE hTimer;

// 백버퍼 (화면 변수) -> 위의 그림을 백버퍼에 찍음
// 백버퍼는 프론트버퍼와 매 프레임마다 데이터를 교환하면서 프론트버퍼가 화면에 출력됨
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

// 윈도우 프로시저 함수 정의
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lparam)
{
	/*
	HDC hdc; // 화면 변수(프론트버퍼)
	HDC MemDC; // (백버퍼)

	PAINTSTRUCT ps;

	// lp : 롱포인터
	LPPAINTSTRUCT ptr_ps = &ps;

	// h : 핸들
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
		// 윈도우 생성시 날아오는 메세지
	case WM_CREATE:
	{
		// 이미지 가져오기
		hBackBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBarBitmap = (HBITMAP)LoadImage(NULL, L"Bar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBallBitmap = (HBITMAP)LoadImage(NULL, L"Ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// 일정시간마다  WM_TIMER를 메세지 큐에 보내도록 요청
		hTimer = (HANDLE)SetTimer(hWnd, TIMERID, 1, NULL);

		// ClientRect : window의 안쪽 사각형 영역을 반환하여 crt를 할당
		GetClientRect(hWnd, &crt);
	}
	break;
	

	// SetTimer로 일정시간마다 메세지큐에 들어감
	case WM_TIMER:
	{
		BallMove();

		HDC hDC = GetDC(hWnd);

		// MemDC랑 같은 역할
		// Compatible -> 매개변수로 들어가는 DC랑 호환되는 DC를 만듬
		HDC hBufferDC = CreateCompatibleDC(hDC);

		if (hBitMapBuffer == NULL)
		{
			// 가상의 비트맵 생성
			hBitMapBuffer = CreateCompatibleBitmap(hDC, crt.right, crt.bottom); // 빈화면 생성하여 저장
		}
		
		// SelectObject : 화면과 비트맵을 연결하여 화면에 보이게 만듬, 이전 상태를 반환함
		// 여기서는 빈화면(hBitMapBuffer)을 연결
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hBufferDC, hBitMapBuffer);

		//그리기
		DrawBitmapToBuffer(hWnd, hBufferDC, 0, 0, hBackBitmap);
		DrawBitmapToBuffer(hWnd, hBufferDC, g_Bar_X, 0, hBarBitmap);
		DrawBitmapToBufferColorKey(hWnd, hBufferDC, g_Ball_X, g_Ball_Y, hBallBitmap);

		// 이전상태 복귀
		SelectObject(hBufferDC, hOldBitmap); 

		// 호환되는 HDC삭제
		DeleteDC(hBufferDC); // hBufferDC 반환

		// 윈도우에 제어권 반납
		ReleaseDC(hWnd, hDC);

		InvalidateRect(hWnd, NULL, true); // WM_PAINT를 OS에 요청
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

	// 화면의 변경이 있을때 메세지큐에 들어감
	case WM_PAINT :
	{
		/*
		hdc = BeginPaint(hWnd, &ps);

		MemDC = CreateCompatibleDC(hdc);

		hMyFileBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		// hMyFileBitmap로 전환, 이전상태 반환
		hOldFilebitmap = (HBITMAP)SelectObject(MemDC, hMyFileBitmap); 

		// 읽어온 이미지의 정보를 bitmap_image_info에 저장
		GetObject(hMyFileBitmap, sizeof(BITMAP), &bitmap_image_info);

		// 이미지의 가로세로 길이
		width = bitmap_image_info.bmWidth;
		height = bitmap_image_info.bmHeight;

		BitBlt(hdc, 0, 0, width, height, MemDC, 0, 0, SRCCOPY);

		// 이전 상태 복귀
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
		

	case WM_DESTROY : // x버튼 클릭시하는 경우
	{
		PostQuitMessage(0); // 메세지 큐에 WM_QUIT가 들어간다.
		return NULL;
	}
	break;

	default: break;
	}

	// (Default Window Procedure) : 
	//		사용자가 처리하지 않은 메시지를 처리하기 위해 호출
	//		메시지에 따라 기본 동작(예: 창 이동, 크기 조정, 닫기 등)을 수행
	return DefWindowProc(hWnd, message, wParam, lparam);
}
