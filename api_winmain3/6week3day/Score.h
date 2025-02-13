#pragma once

#include <gdiplus.h>

using namespace Gdiplus;

class cScore
{
private:
	Gdiplus::Image* image = NULL;
	HBITMAP m_hBitmap;
	int m_x, m_y;
	int m_sprite_w, m_sprite_h;
	//int m_nFrame;
	COLORREF m_colorkey;
	
	int m_CurrentScore; //현재 점수가 몇점인가.
public:
	cScore();
	~cScore();

	void Destroy();

	void Create(int x, int y, int sprite_w, int sprite_h, COLORREF colorkey);
	void Draw(HWND hWnd, HDC hBufferDC);

	void AddScore() { m_CurrentScore++; }
};