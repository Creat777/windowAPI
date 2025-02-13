#pragma once

class cBlock
{
private:
	HBITMAP m_hBitmap;
	int m_x, m_y;
	bool m_bLive;
public:
	cBlock();
	~cBlock();

	void Init(int x, int y);
	void Draw(HWND hWnd, HDC hBufferDC);

	RECT GetRect();

	void SetLive(bool live) { m_bLive = live; }
	bool GetLive() { return m_bLive; }
};
