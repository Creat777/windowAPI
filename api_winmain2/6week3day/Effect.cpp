#include "Effect.h"

using namespace Gdiplus;

#pragma comment(lib, "winmm.lib")

cEffect::cEffect()
{
	m_nFrame = 0;
}

cEffect::~cEffect()
{

}

void cEffect::Destroy()
{
	DeleteObject(m_hBitmap);
}

void cEffect::Init(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey)
{
	if (image == NULL)
	{
		image = Gdiplus::Image::FromFile(L"flame_sprite2.png");

		Bitmap* pBitmap = (Bitmap*)image->Clone();

		pBitmap->GetHBITMAP(Color(0, 0, 0), &m_hBitmap);
	}

	m_x = x; m_y = y - 30;
	m_sprite_w = sprite_w;
	m_sprite_h = sprite_h;
	m_colorkey = colorkey;

	m_StartTime = timeGetTime();

	m_bLive = true;
}

void cEffect::Draw(HWND hWnd, HDC hBufferDC)
{
	if (m_bLive == false) return;

	HDC hDC = GetDC(hWnd);

	HDC hTemDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hTemDC, m_hBitmap);

	BITMAP bitmap; // 비트맵 정보
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);

	// bitmap에서 바탕을 제거하는 함수
	TransparentBlt(hBufferDC, m_x, m_y, m_sprite_w, m_sprite_h,
		hBufferDC, m_nFrame * m_sprite_w, 0, m_sprite_w, m_sprite_h, m_colorkey);

	SelectObject(hTemDC, hOldBit);
	DeleteDC(hTemDC);

	ReleaseDC(hWnd, hDC);
}
