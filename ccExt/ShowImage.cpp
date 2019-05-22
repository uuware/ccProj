#include "ShowImage.h"
/*
 *	Description:
 *
 */

/*
int SHA1_sample()
{
	return 0;
}
*/

/*	
 *	ShowImage
 *
 *	Description:
 *
 */
ShowImage::ShowImage()
{
	m_hBitmap = NULL;
	m_hDotBmp = NULL;
	m_hDotBmpMask = NULL;
	m_hIcon = NULL;
	m_hPasteIcon = NULL;
	isStretch = true;
	isNeedDestroy = false;
	nEnlarge = 1;
	szSize.cx = 16;
	szSize.cy = 16;
	SetTrackMouse(true);
}

ShowImage::~ShowImage()
{
	Clear();
	ClearDotMask();
}

void ShowImage::Clear()
{
	if(isNeedDestroy)
	{
		if(m_hBitmap != NULL)
		{
			DeleteObject(m_hBitmap);
			m_hBitmap = NULL;
		}
		if(m_hIcon != NULL)
		{
			::DestroyIcon(m_hIcon);
			m_hIcon = NULL;
		}
	}
}

void ShowImage::ClearDotMask()
{
	if(m_hDotBmp != NULL)
	{
		DeleteObject(m_hDotBmp);
		m_hDotBmp = NULL;
	}
	if(m_hDotBmpMask != NULL)
	{
		DeleteObject(m_hDotBmpMask);
		m_hDotBmpMask = NULL;
	}
}

void ShowImage::SetEnlarge(int nEnlarge)
{
	if(this->nEnlarge != nEnlarge)
	{
		ClearDotMask();
	}
	this->nEnlarge = nEnlarge;
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void ShowImage::SetStretch(bool isStretch)
{
	this->isStretch = isStretch;
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void ShowImage::SetRect(bool isShow, int nX, int nY, int nW, int nH)
{
	isShowRect = isShow;
	nRectX = nX * nEnlarge;
	nRectY = nY * nEnlarge;
	nRectW = nW * nEnlarge;
	nRectH = nH * nEnlarge;
	if(m_hWnd != NULL)
	{
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

//this hIcon is need release out scope
void ShowImage::SetPasteImage(HICON hIcon, int nX, int nY)
{
	nPasteX = nX * nEnlarge;
	nPasteY = nY * nEnlarge;
	if(m_hPasteIcon != hIcon)
	{
		m_hPasteIcon = hIcon;
		if(hIcon != NULL)
		{
			ICONINFO iconInfo = {0};
			// get the icon information
			::GetIconInfo(hIcon, &iconInfo);
			nPasteW = iconInfo.xHotspot * 2 * nEnlarge;
			nPasteH = iconInfo.yHotspot * 2 * nEnlarge;
			::DeleteObject(iconInfo.hbmColor);
			::DeleteObject(iconInfo.hbmMask);
		}
	}
//cc::Log::debug(_T("nPasteX:%d, nPasteY:%d, nPasteW:%d, nPasteH:%d, nEnlarge:%d"), nPasteX, nPasteY, nPasteW, nPasteH, nEnlarge);
	if(m_hWnd != NULL)
	{
		HDC hDC = ::GetDC(m_hWnd);
		DoPaint(hDC);
		::ReleaseDC(m_hWnd, hDC);
		//::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void ShowImage::CreateDotMask()
{
	ClearDotMask();
	if(m_hWnd != NULL && nEnlarge >= 6)
	{
		int nH = nEnlarge * szSize.cy;
		int nW = nEnlarge * szSize.cx;
		HDC hScreenDC = ::GetDC(m_hWnd);
		m_hDotBmp = CreateCompatibleBitmap(hScreenDC, nW, nH);
		m_hDotBmpMask = CreateBitmap(nW, nH, 1, 1, NULL);

		HDC hMemDC1 = CreateCompatibleDC(hScreenDC);
		HDC hMaskDC = CreateCompatibleDC(hScreenDC);
		HBITMAP hOldBitmap1 = (HBITMAP)SelectObject(hMemDC1, m_hDotBmp);
		HBITMAP hOldBitmap2 = (HBITMAP)SelectObject(hMaskDC, m_hDotBmpMask);

		//draw picture on orignal image
		int nXY;
		for(int i = 0; i < szSize.cx + 1; i++)
		{
			nXY = nEnlarge * i;
			CC_APP_GetDrawActor()->DotLine(hMemDC1, nXY, 0, nXY, nW, 0, 2, RGB(255, 255, 255), false);
			CC_APP_GetDrawActor()->DotLine(hMemDC1, nXY, 1, nXY, nW, 0, 2, RGB(128, 128, 128), false);
		}
		for(int i = 0; i < szSize.cy + 1; i++)
		{
			nXY = nEnlarge * i;
			CC_APP_GetDrawActor()->DotLine(hMemDC1, 0, nXY, nH, nXY, 2, 0, RGB(255, 255, 255), false);
			CC_APP_GetDrawActor()->DotLine(hMemDC1, 1, nXY, nH, nXY, 2, 0, RGB(128, 128, 128), false);
		}

		//create mask image
		::SetBkColor(hMemDC1, RGB(0, 0, 0));
		//BitBlt(hMaskDC, 0, 0, nW, nH, NULL, 0, 0, BLACKNESS);
		BitBlt(hMaskDC, 0, 0, nW, nH, hMemDC1, 0, 0, SRCCOPY);

		//clear colors in transparent area from orgial image
        //::SetBkColor(hMemDC1, RGB(0, 0, 0));
        //::SetTextColor(hMemDC1, RGB(255, 255, 255));
		//BitBlt(hMemDC1, 0, 0, nW, nH, hMaskDC, 0, 0, SRCAND);

		SelectObject(hMemDC1, hOldBitmap1);
		SelectObject(hMaskDC, hOldBitmap2);
		DeleteDC(hMemDC1);
		DeleteDC(hMaskDC);
		ReleaseDC(m_hWnd, hScreenDC);
	}
}

void ShowImage::DoPaint(HDC hDC)
{
	RECT rcItem;
	::GetClientRect(m_hWnd, &rcItem);

	HDC hChaceDC = CreateCompatibleDC(hDC);
	HBITMAP hChaceBmp = CreateCompatibleBitmap(hDC, rcItem.right - rcItem.left, rcItem.bottom - rcItem.top);
	HBITMAP hChaceBmpOld = (HBITMAP)SelectObject(hChaceDC, hChaceBmp);
	
	//HS_BDIAGONAL = /, and or HS_FDIAGONAL
	HBRUSH hBrush = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 255, 255));
	::SetBkColor(hChaceDC, GetBackground());
	::SetBkMode(hChaceDC, TRANSPARENT);
	::ExtTextOut(hChaceDC, 0, 0, ETO_OPAQUE, &rcItem, NULL, 0, NULL);
	::FillRect(hChaceDC, &rcItem, hBrush);
	DeleteObject(hBrush);

	int nH = nEnlarge * szSize.cy;
	int nW = nEnlarge * szSize.cx;
	if(m_hBitmap != NULL)
	{
		HDC hTargetDC = CreateCompatibleDC(hDC);
		HBITMAP hOldBitmap = (HBITMAP)SelectObject(hTargetDC, m_hBitmap);
		if(isStretch)
		{
			SetStretchBltMode(hChaceDC, COLORONCOLOR);
			StretchBlt(hChaceDC, 0, 0, nW, nH, hTargetDC, 0, 0, szSize.cx, szSize.cy, SRCCOPY);
		}
		else
		{
			::BitBlt(hChaceDC, 0, 0, nW, nH, hTargetDC, 0, 0, SRCCOPY);
		}
		SelectObject(hTargetDC, hOldBitmap);
		DeleteDC(hTargetDC);
	}
	else if(m_hIcon != NULL)
	{
		if(isStretch)
		{
			::DrawIconEx(hChaceDC, 0, 0, m_hIcon, nW, nH, 0, NULL, DI_NORMAL);
		}
		else
		{
			::DrawIcon(hChaceDC, 0, 0, m_hIcon);
		}
	}
	if(m_hPasteIcon != NULL)
	{
		::DrawIconEx(hChaceDC, nPasteX, nPasteY, m_hPasteIcon, nPasteW, nPasteH, 0, NULL, DI_NORMAL);
	}

	if(nEnlarge >= 6)
	{
		if(m_hDotBmp == NULL)
		{
			CreateDotMask();
		}
		if(m_hDotBmp != NULL)
		{
			HDC hTargetDC = CreateCompatibleDC(hDC);
			HDC hMaskDC = CreateCompatibleDC(hDC);
			HBITMAP hOldBmp1 = (HBITMAP)SelectObject(hTargetDC, m_hDotBmp);
			HBITMAP hOldBmp2 = (HBITMAP)SelectObject(hMaskDC, m_hDotBmpMask);

			::SetBkColor(hChaceDC, RGB(255, 255, 255));
			::SetTextColor(hChaceDC, RGB(0, 0, 0));
			BitBlt(hChaceDC, 0, 0, nW, nH, hMaskDC, 0, 0, SRCAND);
			BitBlt(hChaceDC, 0, 0, nW, nH, hTargetDC, 0, 0, SRCPAINT);

			SelectObject(hTargetDC, hOldBmp1);
			SelectObject(hMaskDC, hOldBmp2);
			DeleteDC(hTargetDC);
			DeleteDC(hMaskDC);
		}
	}
	if(isShowRect)
	{
		//CC_InvertBorder(nRectX, nRectY, nRectW, nRectH, hChaceDC);
		CC_APP_GetDrawActor()->InvertBorder(nRectX, nRectY, nRectW, nRectH, hChaceDC);
	}

	::BitBlt(hDC, 0, 0, nW, nH, hChaceDC, 0, 0, SRCCOPY);
	SelectObject(hChaceDC, hChaceBmpOld);
	DeleteObject(hChaceBmp);
	DeleteDC(hChaceDC);
}

void ShowImage::OnPaint(PaintEventArgs* e)
{
	PAINTSTRUCT ps;
	HDC hDC = BeginPaint(m_hWnd, &ps);
	PaintBegin.Invoke((Object*)this, e);
	DoPaint(hDC);
	PaintEnd.Invoke((Object*)this, e);
	EndPaint(m_hWnd, &ps);
}

void ShowImage::SetImage(HBITMAP hBitmap, int nW, int nH, bool isNeedDestroy)
{
	if(m_hBitmap != hBitmap)
	{
		Clear();
	}
	this->isNeedDestroy = isNeedDestroy;
	m_hBitmap = hBitmap;
	if(szSize.cx != nW || szSize.cy != nH)
	{
		ClearDotMask();
	}
	szSize.cx = nW;
	szSize.cy = nH;
	if(m_hWnd != NULL)
	{
		HDC hDC = ::GetDC(m_hWnd);
		DoPaint(hDC);
		::ReleaseDC(m_hWnd, hDC);
		//::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}

void ShowImage::SetImage(HICON hIcon, int nW, int nH, bool isNeedDestroy)
{
	if(this->m_hIcon != hIcon)
	{
		Clear();
	}
	this->isNeedDestroy = isNeedDestroy;
	this->m_hIcon = hIcon;
	if(szSize.cx != nW || szSize.cy != nH)
	{
		ClearDotMask();
	}
	szSize.cx = nW;
	szSize.cy = nH;
	if(m_hWnd != NULL)
	{
		HDC hDC = ::GetDC(m_hWnd);
		DoPaint(hDC);
		::ReleaseDC(m_hWnd, hDC);
		//::InvalidateRect(m_hWnd, NULL, TRUE);
	}
}
