#pragma once
#include <Windows.h>
#include "Block.h"
#include "Effect.h"
#define BlockCount 7

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

	// 이미지
	HBITMAP hBackBitmap; //test.bmp
	HBITMAP hBarBitmap;  //bar.bmp
	HBITMAP hBallBitmap;  //ball.bmp

	// 이미지 정보

private :
	BALL_DIR_X eDirX = eRIGHT;
	BALL_DIR_Y eDirY = eDOWN;

	cBlock BlockArray[BlockCount];

	cEffect m_Effect;

public :
	cGameManager();
	~cGameManager();

	void BallMove();
	void CollisionBallAndBar();
	void CollisionBallAndBlock();

	void CreateBlock();
	void DrawBlock(HWND hWnd, HDC hBufferDC);

	void InitEffect(int x, int y);
	void DrawEffect(HWND hWnd, HDC hBufferDC);

	void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
	void DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
};