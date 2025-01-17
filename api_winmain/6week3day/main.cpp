#include <windows.h>

// LRESULT : Long Result (Windows API에서 메시지 처리 함수(WndProc)가 반환하는 값의 데이터 타입)
// CALLBACK : 함수 호출 규약을 나타내며, Windows API에서 사용하는 표준 호출 규약입니다.
// 매개변수
//	->	HWND: 윈도우 핸들(윈도우를 식별하는 고유 값).
//		UINT: 메시지의 종류를 나타냅니다.
//		WPARAM : Word PARAMeter -> 메시지와 관련된 작은 크기의 정수형 데이터나 **핸들(hWnd)**을 전달하는 데 사용
//		LPARAM : Long PARAMeter -> 메시지와 관련된 **추가 정보(주소나 구조체)**를 전달하는 데 사용

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); //콜백 함수 - 윈도우 메세지 처리 함수

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow)
{
	//레지스트리 등록
	WNDCLASS wcex = { 0, };

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // window메세지가 올 때 호출해줘
	wcex.hInstance = hInstance; // hwnd 핸들(포인터)
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

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lparam)
{
	HDC hdc; // 화면 변수(프론트버퍼)
	HDC MemDC; // (백버퍼)
	PAINTSTRUCT ps;
	// lp : 롱포인터
	LPPAINTSTRUCT ptr_ps = &ps;

	// h : 핸들
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

		hOldFilebitmap = (HBITMAP)SelectObject(MemDC, hMyFileBitmap); // hMyFileBitmap로 전환, 이전상태 저장

		GetObject(hMyFileBitmap, sizeof(BITMAP), &bitmap_image_info);

		width = bitmap_image_info.bmWidth;
		height = bitmap_image_info.bmHeight;

		BitBlt(hdc, 0, 0, width, height, MemDC, 0, 0, SRCCOPY);

		SelectObject(MemDC, hOldFilebitmap); // 이전 상태 복귀

		DeleteObject(hMyFileBitmap);
		DeleteObject(MemDC);

		EndPaint(hWnd, &ps);
		
		return NULL;
	}

	case WM_DESTROY : // x버튼 클릭시하는 경우
	{
		PostQuitMessage(0); // 메세지 큐에 WM_QUIT가 들어간다.
		return NULL;
	}
		
	}

	return DefWindowProc(hWnd, message, wParam, lparam);
}
