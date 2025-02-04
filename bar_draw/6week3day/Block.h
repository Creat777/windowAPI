#pragma once
#include <windows.h>

class cBlock
{
private :
	HBITMAP m_hBitmap;
	int m_x, m_y;

public :
	cBlock();
	~cBlock();

	void Init(int x, int y);
	void Draw(HWND hWnd, HDC hBufferDC);
};