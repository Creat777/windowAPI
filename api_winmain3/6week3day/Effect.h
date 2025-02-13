#pragma once

class cEffect
{
private:
	Gdiplus::Image* image = NULL;
	HBITMAP m_hBitmap;
	int m_x, m_y;
	int m_sprite_w, m_sprite_h;
	int m_nFrame;
	COLORREF m_colorkey;
	DWORD m_StartTime;
	bool m_bLive;
public:
	cEffect();
	~cEffect();

	void Destory();

	void Init(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey);
	void Draw(HWND hWnd, HDC hBufferDC);
};
