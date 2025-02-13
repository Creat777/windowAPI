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
	POINT ptUp = { m_Ball_X + 20, m_Ball_Y };
	POINT ptDown = { m_Ball_X + 20, m_Ball_Y + 39 };
	POINT ptLeft = { m_Ball_X, m_Ball_Y + 20 };
	POINT ptRight = { m_Ball_X + 39, m_Ball_Y + 20 };

	RECT rect = { m_Bar_X, m_Bar_Y, m_Bar_X + 173, m_Bar_Y + 28 };

	if (PtInRect(&rect, ptDown) == true)
	{
		if (eDirY == eDOWN) eDirY = eUP;
	}
	else if (PtInRect(&rect, ptUp) == true)
	{
		if (eDirY == eUP) eDirY = eDOWN;
	}
	else if (PtInRect(&rect, ptLeft) == true)
	{
		if (eDirX == eLEFT) eDirX = eRIGHT;
	}
	else if (PtInRect(&rect, ptRight) == true)
	{
		if (eDirX == eRIGHT) eDirX = eLEFT;
	}
}

void cGameManager::CollisionBallAndBlock()
{
	POINT ptUp = { m_Ball_X + 20, m_Ball_Y };
	POINT ptDown = { m_Ball_X + 20, m_Ball_Y + 39 };
	POINT ptLeft = { m_Ball_X, m_Ball_Y+20 };
	POINT ptRight = { m_Ball_X + 39, m_Ball_Y + 20 };

	for (int i = 0; i < BlockRow; ++i)
	{
		for (int j = 0; j < Blockcolumn; j++)
		{
			if (BlockMatrix[i][j].GetLive() == false) continue;

			RECT rect = BlockMatrix[i][j].GetRect();

			if (PtInRect(&rect, ptUp) == true)
			{
				if (eDirY == eUP)
				{
					eDirY = eDOWN;
					BlockMatrix[i][j].SetLive(false);

					InitEffect(rect.left, rect.bottom);

					m_Score.AddScore();

					break;
				}
			}
			else if (PtInRect(&rect, ptDown) == true)
			{
				if (eDirY == eDOWN)
				{
					eDirY = eUP;
					BlockMatrix[i][j].SetLive(false);

					InitEffect(rect.left, rect.bottom);

					m_Score.AddScore();

					break;
				}
			}
			else if (PtInRect(&rect, ptLeft) == true)
			{
				if (eDirX == eLEFT)
				{
					eDirX = eRIGHT;
					BlockMatrix[i][j].SetLive(false);

					InitEffect(rect.left, rect.bottom);

					m_Score.AddScore();

					break;
				}
			}
			if (PtInRect(&rect, ptRight) == true)
			{
				if (eDirX == eRIGHT)
				{
					eDirX = eLEFT;
					BlockMatrix[i][j].SetLive(false);

					InitEffect(rect.left, rect.bottom);

					m_Score.AddScore();

					break;
				}
			}
		}
		
	}
}

void cGameManager::CreateBlock()
{
	for (int i = 0; i < BlockRow; ++i)
	{
		for (int j = 0; j < Blockcolumn; j++)
		{
			BlockMatrix[i][j].Init(10 + j * 130 + 15 * j, 10 + 40 * i);
		}
		
	}
}
void cGameManager::DrawBlock(HWND hWnd, HDC hBufferDC)
{
	for (int i = 0; i < BlockRow; ++i)
	{
		for (int j = 0; j < Blockcolumn; j++)
		{
			if (BlockMatrix[i][j].GetLive() == false) continue;

			BlockMatrix[i][j].Draw(hWnd, hBufferDC);
		}
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
	m_Score.Create(50, 10, 72, 77, RGB(0, 0, 0));
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