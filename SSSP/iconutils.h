#pragma once

extern COLORREF GetTransparentColor(HBITMAP hBmp);
extern HBITMAP LoadPicture(LPCTSTR lpszFilename, BOOL *pbAlphaEnabled = NULL);
extern BOOL GetIconImage(HDC hIconDC, HBITMAP hBmp, UINT uIndex);

#define RGBA(r,g,b,a)	((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16))|(((DWORD)(BYTE)(a))<<24))
#define SET_ALPHA(c, a)		((COLORREF)(c) | (((DWORD)(BYTE)(a))<<24))

class CGDIPlusContext
{
private:
	ULONG_PTR m_token;
public:
	CGDIPlusContext();
	~CGDIPlusContext();
};