#include <stdio.h>
#include <string.h>
#include "..\cc\cc.All.h"

/*
 *	Description:
 *
 */

#ifndef _SHOWIMAGE_H_
#define _SHOWIMAGE_H_

using namespace cc;
using namespace cc::win;
class ShowImage : public Panel
{
public:
	ShowImage();
	virtual ~ShowImage();

	void SetImage(HBITMAP hBitmap, int nW, int nH, bool isNeedDestroy = false);
	void SetImage(HICON hIcon, int nW, int nH, bool isNeedDestroy = false);
	void SetEnlarge(int nEnlarge);
	void SetStretch(bool isStretch);
	void SetRect(bool isShow, int nX, int nY, int nW, int nH);
	void SetPasteImage(HICON hIcon, int nW, int nH);

protected:
	HBITMAP m_hBitmap;
	HICON m_hIcon;
	bool isStretch;
	bool isNeedDestroy;
	bool isShowRect;
	int nRectX;
	int nRectY;
	int nRectW;
	int nRectH;
	HICON m_hPasteIcon;
	int nPasteX;
	int nPasteY;
	int nPasteW;
	int nPasteH;
	int nEnlarge;
	SIZE szSize;
	HBITMAP m_hDotBmp;
	HBITMAP m_hDotBmpMask;

	void Clear();
	void ClearDotMask();
	virtual void CreateDotMask();
	virtual void DoPaint(HDC hdc);
	virtual void OnPaint(PaintEventArgs *e);
};

#endif //#ifndef _SHOWIMAGE_H_
