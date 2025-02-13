#include "Block.h"
#include "Effect.h"
#include "GameManager.h"


cGameManager::cGameManager()
{
	// 이미지 가져오기
	hBackBitmap = (HBITMAP)LoadImage(NULL, L"test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hBarBitmap = (HBITMAP)LoadImage(NULL, L"Bar.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hBallBitmap = (HBITMAP)LoadImage(NULL, L"Ball.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

cGameManager::~cGameManager()
{
	DeleteObject(hBackBitmap);
	DeleteObject(hBallBitmap);
	DeleteObject(hBarBitmap);
}

void cGameManager::BallMove()
{
	if (eDirX == eRIGHT)
	{
		m_Ball_X += 5;
		if (m_Ball_X > (1024 - 39)) // 39는 공의 x축 크기
		{
			eDirX = eLEFT;
		}
	}
	else if (eDirX == eLEFT)
	{
		m_Ball_X -= 5;
		if (m_Ball_X < (0))
		{
			eDirX = eRIGHT;
		}
	}

	if (eDirY == eDOWN)
	{
		m_Ball_Y += 5;
		if (m_Ball_Y > (768 - 39)) // 39는 공의 x축 크기
		{
			eDirY = eUP;
		}
	}
	else if (eDirY == eUP)
	{
		m_Ball_Y -= 5;
		if (m_Ball_Y < (0))
		{
			eDirY = eDOWN;
		}
	}
}

void cGameManager::CollisionBallAndBar()
{
	// 모든 객체의 시작점은 좌측 상단
	// 
	// 공이 하강할 때 바와 부딛히는 좌표점
	POINT pt_down = { m_Ball_X + 20, m_Ball_Y + 39 };
	POINT pt_up = { m_Ball_X + 20, m_Ball_Y };

	// 바의 충돌박스(사각형)
	RECT rect = { m_Bar_X, m_Bar_Y , m_Bar_X + 173, m_Bar_Y + 28 };

	// PtInRect : 점이 사각형 내부에 포함되는지 체크하는 함수
	if (PtInRect(&rect, pt_down) == true)
	{
		// 공의 y축 방향을 반대로 변경
		if (eDirY == eDOWN)
		{
			eDirY = eUP;
		}
	}
	else if (PtInRect(&rect, pt_up))
	{
		if (eDirY == eUP)
		{
			eDirY = eDOWN;
		}
	}
}

void cGameManager::CollisionBallAndBlock()
{


	// 모든 객체의 시작점은 좌측 상단
	BITMAP bitmap;
	GetObject(hBallBitmap, sizeof(BITMAP), &bitmap);

	POINT pt_down = { m_Ball_X + bitmap.bmWidth / 2 +1, m_Ball_Y + bitmap.bmWidth };
	POINT pt_up = { m_Ball_X + bitmap.bmWidth / 2+1, m_Ball_Y };
	POINT pt_right = { m_Ball_X + bitmap.bmWidth, m_Ball_Y + bitmap.bmWidth / 2 +1};
	POINT pt_left = { m_Ball_X, m_Ball_Y + bitmap.bmWidth / 2 +1};

	for (int i = 0; i < BlockCount; i++)
	{
		if (BlockArray[i].GetLive() == false) continue;

		// 블럭의 충돌박스(사각형)
		RECT rect = BlockArray[i].GetRect();

		// PtInRect : 점이 사각형 내부에 포함되는지 체크하는 함수
		if (PtInRect(&rect, pt_down) == true)
		{
			// 공의 y축 방향을 반대로 변경
			if (eDirY == eDOWN)
			{
				eDirY = eUP;
				BlockArray[i].SetLive(false); return;
			}
		}
		else if (PtInRect(&rect, pt_up))
		{
			if (eDirY == eUP)
			{
				eDirY = eDOWN;
				BlockArray[i].SetLive(false); return;
			}
		}
		else if (PtInRect(&rect, pt_right))
		{
			if (eDirX == eRIGHT)
			{
				eDirX = eLEFT;
				BlockArray[i].SetLive(false); return;
			}
		}
		else if (PtInRect(&rect, pt_left))
		{
			if (eDirX == eLEFT)
			{
				eDirX = eRIGHT;
				BlockArray[i].SetLive(false); return;
			}
		}
	}
	
}

void cGameManager::CreateBlock()
{
	for (int i = 0; i < BlockCount; i++)
	{
		BlockArray[i].Init(10 + i*(130 + 15), 10);
	}
	
}

void cGameManager::DrawBlock(HWND hWnd, HDC hBufferDC)
{
	for (int i = 0; i < BlockCount; i++)
	{
		if (BlockArray[i].GetLive() == false) continue;

		BlockArray[i].Draw(hWnd, hBufferDC);
	}
}

void cGameManager::InitEffect(int x, int y)
{
	m_Effect.Init(x, y, 100, 100, RGB(0, 0, 0));
}

void cGameManager::DrawEffect(HWND hWnd, HDC hBufferDC)
{
	m_Effect.Draw(hWnd, hBufferDC);
}

void cGameManager::DrawBitmapToBuffer(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit)
{
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, hBit);

	BITMAP bitmap;
	GetObject(hBit, sizeof(BITMAP), &bitmap);

	BitBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY);

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);
}

void cGameManager::DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit)
{
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, hBit);

	BITMAP bitmap;
	GetObject(hBit, sizeof(BITMAP), &bitmap);

	// bitmap에서 흰색을 제거하는 함수
	TransparentBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight,
		hTempDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);

	ReleaseDC(hWnd, hDC);
}
