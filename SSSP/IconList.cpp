#include "StdAfx.h"
#include "IconList.h"

CButtonImage::CButtonImage(LPCTSTR lpszFilename, UINT uIndex):
	m_Type(BIT_NA),
	m_pIconDC(NULL),
	m_ulRef(0)
{
	m_Filename = lpszFilename;
	m_uIndex = uIndex;
}

CButtonImage::~CButtonImage()
{
	delete m_pIconDC;
}

BOOL CButtonImage::SetSource(LPCTSTR lpszFilename, UINT uIndex)
{
	m_Filename = lpszFilename;
	m_uIndex = uIndex;
	return TRUE;
}

HICON CButtonImage::GetIcon(void)
{
	if (m_Filename.IsEmpty()) {
		return NULL;
	}
	
	HICON hIcon = NULL;
	if (m_Type == BIT_II) {
		return hIcon;
	}
	if (m_Type == BIT_NA || m_Type == BIT_XI) {
		HICON hIconL = NULL;
		if (::ExtractIconEx(m_Filename, m_uIndex, &hIconL, &hIcon, 1) > 0) {
			if (hIcon == NULL) {
				hIcon = hIconL;
			} else if (hIconL != NULL) {
				// •s—v
				::DestroyIcon(hIconL);
			}
			m_Type = BIT_XI;
			return hIcon;
		}
	}
	m_Type = BIT_LP;
	if (m_pIconDC == NULL) {
		m_pIconDC = new CIconDC;
		HDC hDC = m_pIconDC->GetDC();
		BOOL bIsAlpha;
		HBITMAP hBmp = ::LoadPicture(m_Filename, &bIsAlpha);
		if (hBmp == NULL) {
			m_Type = BIT_II;
			return NULL;
		}
		GetIconImage(hDC, hBmp, m_uIndex);
		if (bIsAlpha) {
			m_pIconDC->SetTransparentColor(0);
		} else {
			m_pIconDC->SetTransparentColor(GetTransparentColor(hBmp) | 0xFF000000);
		}
		m_pIconDC->ReleaseDC();
	}
	if (m_pIconDC != NULL) {
		hIcon = m_pIconDC->CreateIcon();
	}
	return hIcon;
}




CIconList::CIconList(void):
	m_uSerialIndex(0)
{
}

CIconList::~CIconList(void)
{
	for (POSITION pos = GetStartPosition(); pos != NULL; GetNext(pos)) {
		CPair *p = GetAt(pos);
		delete p->m_value;
	}
}

HICON CIconList::GetIcon(UINT token)
{
	CPair *p = Lookup(token);
	if (p == NULL) {
		return NULL;
	}
	CButtonImage *cur = p->m_value;
	return cur->GetIcon();
}

UINT CIconList::AddIcon(LPCTSTR lpszFilename, UINT uIndex)
{
	POSITION pos = GetStartPosition();
	while (pos != NULL) {
		CPair *p = GetAt(pos);
		CButtonImage *cur = p->m_value;
		if (cur->m_Filename.CompareNoCase(lpszFilename) == 0 && cur->m_uIndex == uIndex) {
			cur->m_ulRef++;
			return GetKeyAt(pos);
		}
		GetNext(pos);
	}
	UINT uToken = ++m_uSerialIndex;
	CButtonImage *cur = new CButtonImage(lpszFilename, uIndex);
	cur->m_ulRef++;
	SetAt(uToken, cur);
	return uToken;
}

BOOL CIconList::ReleaseIcon(UINT token)
{
	CPair *p = Lookup(token);
	if (p == NULL) {
		return FALSE;
	}
	CButtonImage *cur = p->m_value;
	if (--cur->m_ulRef <= 0) {
		RemoveKey(token);
	}
	return TRUE;
}
