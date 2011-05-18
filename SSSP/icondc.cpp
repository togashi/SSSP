#include "stdafx.h"
#include "icondc.h"

CIconDC::CIconDC():
	m_hDC(NULL),
	m_hBmpXor(NULL),
	m_baXor(NULL),
	m_baAnd(NULL)
{
	m_ChromaKey = 0;
	m_Size.cx = 0;
	m_Size.cy = 0;
	SetSize(16, 16);
}

CIconDC::~CIconDC()
{
	if (m_hBmpXor != NULL) {
		::DeleteObject(m_hBmpXor);
	}
	if (m_hDC != NULL) {
		::DeleteDC(m_hDC);
	}
	delete [] m_baXor;
	delete [] m_baAnd;
}

void CIconDC::SetSize(int cx, int cy)
{
	if (cx != m_Size.cx || cy != m_Size.cy) {
		m_Size.cx = cx;
		m_Size.cy = cy;
	}
}

COLORREF CIconDC::SetTransparentColor(COLORREF cr)
{
	COLORREF old = m_ChromaKey;
	m_ChromaKey = cr;
	return cr;
}

HDC CIconDC::GetDC(void)
{
	if (m_hDC == NULL) {
		m_hDC = ::CreateCompatibleDC(NULL);
		HDC htmpdc = ::GetDC(NULL);
		m_hBmpXor = ::CreateCompatibleBitmap(htmpdc, m_Size.cx, m_Size.cy);
		::ReleaseDC(NULL, htmpdc);
		m_hOldObj = ::SelectObject(m_hDC, m_hBmpXor);
	}
	return m_hDC;
}

void CIconDC::ReleaseDC()
{
	::SelectObject(m_hDC, m_hOldObj);

	BITMAPV5HEADER bi = {
		sizeof(BITMAPINFOHEADER)
	};
	::GetDIBits(m_hDC, m_hBmpXor, 0, m_Size.cy, NULL, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);
	m_BitCount = bi.bV5BitCount;

	m_baXor = new BYTE[m_Size.cx * m_Size.cy * bi.bV5BitCount / 8];
	m_baAnd = new BYTE[m_Size.cx * m_Size.cy / 8];
	
	bi.bV5Height = -bi.bV5Height;
	::GetDIBits(m_hDC, m_hBmpXor, 0, m_Size.cy, m_baXor, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);
	
	LPDWORD craXor = NULL;
	LPBYTE baXor32 = NULL;
	if (m_BitCount != 32) {
		baXor32 = new BYTE[m_Size.cx * m_Size.cy * 4];
		bi.bV5BitCount = 32;
		bi.bV5Compression = BI_RGB;
		::GetDIBits(m_hDC, m_hBmpXor, 0, m_Size.cy, baXor32, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);
		craXor = reinterpret_cast<LPDWORD>(baXor32);
	} else {
		craXor = reinterpret_cast<LPDWORD>(m_baXor);
	}
	
	::DeleteObject(m_hBmpXor);
	m_hBmpXor = NULL;
	::DeleteDC(m_hDC);
	m_hDC = NULL;

	BOOL bChromaKeyEnabled = (m_ChromaKey >> 24 == 0xFF);
	DWORD ckPixel = GetBValue(m_ChromaKey) | GetGValue(m_ChromaKey) << 8 | GetRValue(m_ChromaKey) << 16 | (m_ChromaKey & 0xFF000000);
	
	ULONG pixelCount = m_Size.cx * m_Size.cy;
	for (ULONG i = 0; i < pixelCount; i++) {
		int byteIndex = i / 8;
		int bitIndex = i % 8;
		
		if (bitIndex == 0) {
			m_baAnd[byteIndex] = 0;
		}
		
		if (bChromaKeyEnabled) {
			static const BYTE bitmasks[8] = { 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01 };
			craXor[i] |= 0xFF000000;
			if (craXor[i] == ckPixel) {
				craXor[i] &= 0x00FFFFFF;	// 32bpp の場合に対応するためクロマキーとの一致でアルファをゼロにする
				m_baAnd[byteIndex] |= bitmasks[bitIndex];
				m_baAnd[byteIndex] = m_baAnd[byteIndex];
			}
		}
	}
	
	delete [] baXor32;
}

HICON CIconDC::CreateIcon(void)
{
	return ::CreateIcon(NULL, m_Size.cx, m_Size.cy, 1, m_BitCount, m_baAnd, m_baXor);
}
