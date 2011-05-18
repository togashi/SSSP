#pragma once

class CIconDC
{
private:
	CSize m_Size;
	HDC m_hDC;
	HBITMAP m_hBmpXor;
	HGDIOBJ m_hOldObj;

	LPBYTE m_baXor;
	LPBYTE m_baAnd;
	UINT m_BitCount;

	void SetSize(int cx, int cy);
	COLORREF m_ChromaKey;
public:
	CIconDC();
	~CIconDC();
	
	COLORREF SetTransparentColor(COLORREF cr);

	HDC GetDC(void);
	void ReleaseDC();
	HICON CreateIcon(void);
};
