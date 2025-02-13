#pragma once

enum BALL_DIR_X
{
	eRIGHT,
	eLEFT,
};

enum BALL_DIR_Y
{
	eDOWN,
	eUP,
};

#define BlockCount 7

class cGameManager
{
public:
	int m_Bar_X = 200;
	int m_Bar_Y = 680;

	int m_Ball_X = 0;
	int m_Ball_Y = 0;

private:
	BALL_DIR_X eDirX = eRIGHT;
	BALL_DIR_Y eDirY = eDOWN;

	//int BlockCount;
	cBlock BlockArray[BlockCount];

	cEffect m_Effect;
	cScore m_Score;

public:
	cGameManager() {}

	void BallMove();
	void CollisionBallAndBar();
	void CollisionBallAndBlock();

	void CreateBlock();
	void DrawBlock(HWND hWnd, HDC hBufferDC);

	void InitEffect(int x, int y);
	void DrawEffect(HWND hWnd, HDC hBufferDC);

	void CreateScore();
	void DrawScore(HWND hWnd, HDC hBufferDC);

	void DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
	void DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit);
};
