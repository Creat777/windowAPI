#pragma once
#include <Windows.h>
#include "Block.h"

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

	int BlockCount;
	cBlock BlockArray[1];

public :
	cGameManager();
	~cGameManager();

	void BallMove();
	void CollisionBallAndBar();

	void CreateBlock();
	void DrawBlock(HWND hWnd, HDC hBufferDC);

	void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
	void DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
};