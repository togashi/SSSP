#pragma once

#include "icondc.h"
#include "iconutils.h"

class CButtonImage
{
private:
	enum { BIT_NA, BIT_XI, BIT_LP, BIT_II } m_Type;
	CIconDC *m_pIconDC;
public:
	ULONG m_ulRef;
	CString m_Filename;
	UINT m_uIndex;
	
	CButtonImage(LPCTSTR lpszFilename = NULL, UINT uIndex = 0);
	~CButtonImage();
	BOOL SetSource(LPCTSTR lpszFilename, UINT uIndex = 0);
	HICON GetIcon(void);
};

interface IIconList
{
public:
	virtual UINT AddIcon(LPCTSTR lpszFilename, UINT uIndex) = 0;
	virtual BOOL ReleaseIcon(UINT token) = 0;
	virtual HICON GetIcon(UINT token) = 0;
};

class CIconList : 
	public CAtlMap<UINT, CButtonImage *>,
	public IIconList
{
private:
	UINT m_uSerialIndex;
public:
	CIconList(void);
	virtual ~CIconList(void);
	
	// IIconList
	UINT AddIcon(LPCTSTR lpszFilename, UINT uIndex);
	BOOL ReleaseIcon(UINT token);
	HICON GetIcon(UINT token);
};
