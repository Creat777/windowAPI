#include "Block.h"



cBlock::cBlock()
{
	m_hBitmap = (HBITMAP)LoadImage(NULL, L"block.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	m_bLive = true;
}

cBlock::~cBlock()
{
	DeleteObject(m_hBitmap);
}

void cBlock::Init(int x, int y)
{
	m_x = x; m_y = y;
}

void cBlock::Draw(HWND hWnd, HDC hBufferDC)
{
	// ȭ��
	HDC hDC = GetDC(hWnd);

	HDC hTempDC = CreateCompatibleDC(hDC);

	// ��Ʈ�� ���� SelectObject : �� �Ű������� ����, ���� ����Ǿ��ִ� HBITMAP�� ��ȯ
	HBITMAP hOldBit = (HBITMAP)SelectObject(hTempDC, m_hBitmap);
	
	// ��Ʈ�� ����
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap); // bitmap�� ��������� �Ҵ�ް� ��Ʈ���� ������ ����

	BitBlt(hBufferDC, m_x, m_y, bitmap.bmWidth, bitmap.bmHeight, hTempDC, 0, 0, SRCCOPY);

	// ���� ������� ����
	SelectObject(hTempDC, hOldBit);

	// �� ������ ��ȯ
	DeleteDC(hTempDC);

	// ���� ȭ���� ��ȯ
	ReleaseDC(hWnd, hDC);
}

RECT cBlock::GetRect()
{
	BITMAP bitmap;
	GetObject(m_hBitmap, sizeof(BITMAP), &bitmap);


	RECT rect = { m_x, m_y , m_x + bitmap.bmWidth, m_y + bitmap.bmHeight };
	return rect;
}
