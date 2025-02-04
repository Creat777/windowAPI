#include "Block.h"
#include "GameManager.h"

cGameManager::cGameManager()
{
}

cGameManager::~cGameManager()
{
}

void cGameManager::BallMove()
{
	if (eDirX == eRIGHT)
	{
		m_Ball_X += 5;
		if (m_Ball_X > (1024 - 39)) // 39�� ���� x�� ũ��
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
		if (m_Ball_Y > (768 - 39)) // 39�� ���� x�� ũ��
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
	// ��� ��ü�� �������� ���� ���
	// 
	// ���� �ϰ��� �� �ٿ� �ε����� ��ǥ��
	POINT pt_down = { m_Ball_X + 20, m_Ball_Y + 39 };
	POINT pt_up = { m_Ball_X + 20, m_Ball_Y };

	// ���� �浹�ڽ�(�簢��)
	RECT rect = { m_Bar_X, m_Bar_Y , m_Bar_X + 173, m_Bar_Y + 28 };

	// PtInRect : ���� �簢�� ���ο� ���ԵǴ��� üũ�ϴ� �Լ�
	if (PtInRect(&rect, pt_down) == true)
	{
		// ���� y�� ������ �ݴ�� ����
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

void cGameManager::CreateBlock()
{
	BlockArray[0].Init(10, 10);
}

void cGameManager::DrawBlock(HWND hWnd, HDC hBufferDC)
{
	BlockArray[0].Draw(hWnd, hBufferDC);
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

	// bitmap���� ����� �����ϴ� �Լ�
	TransparentBlt(hBufferDC, x, y, bitmap.bmWidth, bitmap.bmHeight,
		hTempDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));

	SelectObject(hTempDC, hOldBit);
	DeleteDC(hTempDC);

	ReleaseDC(hWnd, hDC);
}
