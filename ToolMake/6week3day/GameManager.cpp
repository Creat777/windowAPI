#include <Windows.h>
#include <gdiplus.h>

#include "Block.h"
#include "Effect.h"
#include "Score.h"
#include "GameManager.h"

void cGameManager::BallMove()
{
	if (eDirX == eRIGHT)
	{
		m_Ball_X += 5;
		if (m_Ball_X > (1024 - 39))
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
		if (m_Ball_Y > (768 - 39))
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
	POINT pt = { m_Ball_X + 20, m_Ball_Y + 39 };

	RECT rect = { m_Bar_X, m_Bar_Y, m_Bar_X + 173, m_Bar_Y + 28 };

	if (PtInRect(&rect, pt) == true)
	{
		if (eDirY == eDOWN) eDirY = eUP;
	}
}

void cGameManager::CollisionBallAndBlock()
{
	POINT pt = { m_Ball_X + 20, m_Ball_Y };

	for (int i = 0; i < BlockCount; ++i)
	{
		if (BlockArray[i].GetLive() == false) continue;

		RECT rect = BlockArray[i].GetRect();

		if (PtInRect(&rect, pt) == true)
		{
			if (eDirY == eUP)
			{
				eDirY = eDOWN;
				BlockArray[i].SetLive(false);
				InitEffect(rect.left, rect.top);
				m_Score.AddScore();
				break;
			}
		}
	}
}

void cGameManager::CreateBlock()
{
	for (int i = 0; i < BlockCount; ++i)
	{
		BlockArray[i].Init(10 + i * 130 + 15 * i, 10);
	}
}
void cGameManager::DrawBlock(HWND hWnd, HDC hBufferDC)
{
	for (int i = 0; i < BlockCount; ++i)
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

void cGameManager::CreateScore()
{
	m_Score.Create(10, 10, 72, 77, RGB(0, 0, 0));
}
void cGameManager::DrawScore(HWND hWnd, HDC hBufferDC)
{
	m_Score.Draw(hWnd, hBufferDC);
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

	ReleaseDC(hWnd, hDC);
}

void cGameManager::DrawBitmapToBufferColorKey(HWND hWnd, HDC hBufferDC, int x, int y, HBITMAP hBit)
{
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, hBit);

	BITMAP bitmap;
	GetObject(hBit, sizeof(BITMAP), &bitmap);

	TransparentBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight,
		hTempDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);

	ReleaseDC(hWnd, hDC);
}