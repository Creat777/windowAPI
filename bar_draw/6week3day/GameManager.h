#pragma once
#include <Windows.h>

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

class cGameManager
{
public :
	int m_Bar_X = 200;
	int m_Bar_Y = 500;

	int m_Ball_X = 0;
	int m_Ball_Y = 0;

private :
	BALL_DIR_X eDirX = eRIGHT;
	BALL_DIR_Y eDirY = eDOWN;

public :
	cGameManager();
	~cGameManager();

	void BallMove();
	void CollisionBallAndBar();

	void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
	void DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
};