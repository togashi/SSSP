#pragma once

//#include "activescriptsite.h"
#include "sspscript.h"
#include "searchpathmonitor.h"
#include "iconlist.h"

// script API
#include "smartscrollapi.h"

class CSSPScripts:
	public CWindowImpl<CSSPScripts>,
	public CAtlList<CSSPScript>
{
private:
	CSearchPathMonitor m_PathMonitor;
	HWND m_hTargetWindow;
	CAtlList<CString> m_SearchPathes;
	CAtlList<CString> m_ImportPathes;
	void Refresh(void);
	CIconList m_IconList;
private:
	// 通知メッセージ関連
	enum { NM_FSCHANGED = WM_USER };
	BEGIN_MSG_MAP(CSSPScripts)
		MESSAGE_HANDLER(NM_FSCHANGED, OnFSChanged)
	END_MSG_MAP()
	LRESULT OnFSChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled);

	BOOL AddImportPath(LPCTSTR lpszPath);
public:
	CSSPScripts(void);
	virtual ~CSSPScripts(void);
public:
	BOOL AddSearchPath(LPCTSTR lpszPath);
	BOOL AddSearchPath(int nFolder, LPCTSTR lpszSubdirectory);
public:
	// ISSPScript
	HRESULT Call_ApplyProc(HWND hWnd, SSP_STATUS &ret);
	HRESULT Call_InitProc(PINITPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_ReleaseProc(void);
	HRESULT Call_ButtonProc(PBUTTONPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_GetWheelEffect(SSP_STATUS &ret);
	HRESULT Call_WheelProc(PWHEELPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_ScrollProc(PSCROLLPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_GetIcon(DWORD dwType, HICON &ret);
	HRESULT Call_GetString(DWORD dwType, LPSTR lpStr, UINT uiSize, int &ret);
	HRESULT Call_TimerProc(void);
};
