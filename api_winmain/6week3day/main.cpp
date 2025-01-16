#include <windows.h>

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
	switch (message)
	{
	case WM_DESTROY :
		PostQuitMessage(0); // 메세지 큐에 WM_QUIT가 들어간다.
		return NULL;
	}

	return DefWindowProc(hWnd, message, wParam, lparam);
}
