#include "Block.h"



cBlock::cBlock()
{
	m_hBitmap = (HBITMAP)LoadImage(NULL, L"block.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bLive = true;
}

cBlock::~cBlock()
{
	DeleteObject(m_hBitmap);
}

void cBlock::Init(int x, int y)
{
	m_x = x; m_y = y;
}

void cBlock::Draw(HWND hWnd, HDC hBufferDC)
{
	// 화면
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	// 비트맵 파일 SelectObject : 두 매개변수를 연결, 원래 연결되어있던 HBITMAP을 반환
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, m_hBitmap);
	
	// 비트맵 정보
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap); // bitmap에 저장공간을 할당받고 비트맵의 정보를 저장

	BitBlt(hBufferDC, m_x, m_y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY);

	// 이전 연결상태 복귀
	SelectObject(hTempDC, hOldBit);

	// 다 썼으니 반환
	DeleteDC(hTempDC);

	// 실제 화면의 반환
	ReleaseDC(hWnd, hDC);
}

RECT cBlock::GetRect()
{
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);


	RECT rect = { m_x, m_y , m_x + bitmap.bmWidth, m_y + bitmap.bmHeight };
	return rect;
}
