#include "stdafx.h"
#include "iconutils.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

CGDIPlusContext::CGDIPlusContext()
{
	GdiplusStartupInput gsi;
	GdiplusStartup(&m_token, &gsi, NULL);
}

CGDIPlusContext::~CGDIPlusContext()
{
	GdiplusShutdown(m_token);
}

HBITMAP LoadPicture(LPCTSTR lpszFilename, BOOL *pbAlphaEnabled)
{
	CGDIPlusContext gdipctx;
	HBITMAP hBmp = NULL;
	CStringW wstrFn(lpszFilename);
	{
		Bitmap bmp(wstrFn);
		bmp.GetHBITMAP(Color(0, 0, 0, 0), &hBmp);
		if (pbAlphaEnabled != NULL) {
			PixelFormat pf = bmp.GetPixelFormat();
			*pbAlphaEnabled = IsAlphaPixelFormat(pf);
		}
	}
	return hBmp;
}

BOOL GetBitmapSize(HBITMAP hBmp, SIZE &sz)
{
	HDC hDC = ::CreateCompatibleDC(NULL);
	
	BITMAPINFOHEADER bmi = { sizeof(bmi) };
	::GetDIBits(hDC, hBmp, 0, 0, NULL, reinterpret_cast<BITMAPINFO *>(&bmi), DIB_RGB_COLORS);
	sz.cx = bmi.biWidth;
	sz.cy = bmi.biHeight;
	::DeleteDC(hDC);

	return TRUE;
}

COLORREF GetTransparentColor(HBITMAP hBmp)
{
	HDC hDC = ::CreateCompatibleDC(NULL);
	
	BITMAPINFOHEADER bmi = { sizeof(bmi) };
	::GetDIBits(hDC, hBmp, 0, 0, NULL, reinterpret_cast<BITMAPINFO *>(&bmi), DIB_RGB_COLORS);
	
	::SelectObject(hDC, hBmp);
	COLORREF cr = ::GetPixel(hDC, 0, bmi.biHeight - 1);
	::DeleteDC(hDC);
	
	return cr;
}

BOOL GetIconImage(HDC hIconDC, HBITMAP hBmp, UINT uIndex)
{
	SIZE sz;
	GetBitmapSize(hBmp, sz);
	int oy = uIndex / (sz.cx / 16) * 16;
	int ox = uIndex % (sz.cx / 16) * 16;
	HDC hBmpDC = ::CreateCompatibleDC(NULL);
	::SelectObject(hBmpDC, hBmp);
	::BitBlt(hIconDC, 0, 0, 16, 16, hBmpDC, ox, oy, SRCCOPY);
	::DeleteDC(hBmpDC);
	return TRUE;
}
