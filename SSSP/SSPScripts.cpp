#include "StdAfx.h"
#include "SSPScripts.h"

CSSPScripts::CSSPScripts(void):
	m_hTargetWindow(NULL)
{
	Create(HWND_MESSAGE);		// 通知ウィンドウの作成
}

CSSPScripts::~CSSPScripts(void)
{
	DestroyWindow();			// 通知ウィンドウの破棄
}

BOOL CSSPScripts::AddImportPath(LPCTSTR lpszPath)
{
	//CString msg;
	//msg.Format(_T("%s \"%s\""), _T(__FUNCTION__), lpszPath);
	//SspTraceOut(CT2A(msg));
	
	if (m_ImportPathes.Find(lpszPath) == NULL && m_SearchPathes.Find(lpszPath) == NULL) {
		POSITION p = m_ImportPathes.AddTail(lpszPath);
		if (p != NULL) {
			m_PathMonitor.AddPath(lpszPath);
			m_PathMonitor.Stop();
			m_PathMonitor.Start(m_hWnd, NM_FSCHANGED);
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CSSPScripts::AddSearchPath(LPCTSTR lpszPath)
{
	//CString msg;
	//msg.Format(_T("%s \"%s\""), _T(__FUNCTION__), lpszPath);
	//SspTraceOut(CT2A(msg));
	
	CString tmp;
	::ExpandEnvironmentStrings(lpszPath, tmp.GetBufferSetLength(MAX_PATH + 1), MAX_PATH + 1);
	tmp.ReleaseBuffer();

	if (m_SearchPathes.Find(tmp) == NULL) {
		POSITION p = m_SearchPathes.AddTail(tmp);
		if (p != NULL) {
			Refresh();
			m_PathMonitor.AddPath(tmp);
			m_PathMonitor.Stop();
			m_PathMonitor.Start(m_hWnd, NM_FSCHANGED);
			return TRUE;
		}
	}
	
	return FALSE;
}

BOOL CSSPScripts::AddSearchPath(int nFolder, LPCTSTR lpszSubdirectory)
{
	CPath tmp;
	if (!::SHGetSpecialFolderPath(NULL, tmp.m_strPath.GetBufferSetLength(MAX_PATH), nFolder, TRUE)) {
		return FALSE;
	}
	tmp.m_strPath.ReleaseBuffer();
	tmp.Append(lpszSubdirectory);
	return AddSearchPath(tmp);
}

void CSSPScripts::Refresh(void)
{
	//CString msg;
	//msg.Format(_T("%s"), _T(__FUNCTION__));
	//SspTraceOut(CT2A(msg));
	
	// 削除されたスクリプトのアンロード
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		POSITION rp = NULL;
		CSSPScript &cur = GetAt(sp);
		
		if (cur.IsScriptFileExists() == FALSE) {	// ロードされてるスクリプトのファイルがない
			//msg.Format(_T("  removing %s"), cur.GetMainScriptFilePath());
			//SspTraceOut(CT2A(msg));
			rp = sp;
		}
		GetNext(sp);
		if (rp != NULL) {
			RemoveAt(rp);
		}
	}
	
	POSITION pp = m_SearchPathes.GetHeadPosition();
	while (pp != NULL) {
		CPath path = m_SearchPathes.GetNext(pp);
		CPath filter = path;
		filter.Append(_T("*.js"));
		WIN32_FIND_DATA fd;
		HANDLE hff = ::FindFirstFile(filter, &fd);
		if (hff != INVALID_HANDLE_VALUE) {
			do {
				if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
					continue;
				}
				
				CPath sPath = path;
				sPath.Append(fd.cFileName);
				
				BOOL found = FALSE;

				// ロードされているスクリプトを検索
				sp = GetHeadPosition();
				while (sp != NULL) {
					POSITION rp = NULL;
					CSSPScript &cur = GetAt(sp);
					CString filepath = cur.GetMainScriptFilePath();
					if (filepath == sPath.m_strPath) {	// 発見
						found = TRUE;
						if (cur.IsScriptFileUpToDate() == FALSE) {	// 更新されたファイルがある
							//msg.Format(_T("  reloading %s"), filepath);
							//SspTraceOut(CT2A(msg));
							if (FAILED(cur.Reload())) {
								//msg.Format(_T("    ERROR unload %s"), filepath);
								//SspTraceOut(CT2A(msg));
								rp = sp;
							} else {
								CAtlList<CString> tmpList;
								if (cur.GetImportPathes(tmpList) > 0) {
									for (POSITION pos = tmpList.GetHeadPosition(); pos != NULL; tmpList.GetNext(pos)) {
										AddImportPath(tmpList.GetAt(pos));
									}
								}
							}
						}
					}
					GetNext(sp);
					if (rp != NULL) {
						RemoveAt(rp);
					}
				}

				if (!found) {	// 新しいスクリプトをロード
					//CString msg;
					//msg.Format(_T("  adding %s"), sPath);
					//SspTraceOut(CT2A(msg));
					
					sp = AddTail();
					CSSPScript &cur = GetAt(sp);
					cur.m_pIconList = &m_IconList;
					HRESULT hr = cur.Load(sPath);
					if (FAILED(hr)) {
						//msg.Format(_T("    ERROR unload %s"), sPath);
						//SspTraceOut(CT2A(msg));
						RemoveAt(sp);					
					} else if (hr == S_FALSE) {
						//msg.Format(_T("    declined by script. unload %s"), sPath);
						//SspTraceOut(CT2A(msg));
						RemoveAt(sp);					
					} else {
						CAtlList<CString> tmpList;
						if (cur.GetImportPathes(tmpList) > 0) {
							for (POSITION pos = tmpList.GetHeadPosition(); pos != NULL; tmpList.GetNext(pos)) {
								AddImportPath(tmpList.GetAt(pos));
							}
						}
					}
				}
				
			} while (FindNextFile(hff, &fd) != 0);
			::FindClose(hff);
		}
	}

	// 特定名称のスクリプトの序列を操作
	sp = GetTailPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetAt(sp);
		if (::_tcsicmp(_T("default.js"), cur.GetMainScriptFilename()) == 0) {
			POSITION mp = sp;
			GetPrev(sp);
			MoveToTail(mp);
		} else {
			GetPrev(sp);
		}
	}
}

LRESULT CSSPScripts::OnFSChanged(UINT, WPARAM wParam, LPARAM lParam, BOOL bHandled)
{
	//SspTraceOut(__FUNCTION__);
	bHandled = TRUE;
	Refresh();
	return ERROR_SUCCESS;
}

HRESULT CSSPScripts::Call_ApplyProc(HWND hWnd, SSP_STATUS &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		hr = cur.Call_ApplyProc(hWnd, ret);
		if (SUCCEEDED(hr) && ret == SSP_TRUE) {
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_InitProc(PINITPARAM pParam, SSP_STATUS &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive) {
			hr = cur.Call_InitProc(pParam, ret);
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_ReleaseProc(void)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		hr = cur.Call_ReleaseProc();
	}
	return hr;
}

HRESULT CSSPScripts::Call_ButtonProc(PBUTTONPARAM pParam, SSP_STATUS &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive) {
			hr = cur.Call_ButtonProc(pParam, ret);
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_GetWheelEffect(SSP_STATUS &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive) {
			hr = cur.Call_GetWheelEffect(ret);
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_WheelProc(PWHEELPARAM pParam, SSP_STATUS &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive) {
			hr = cur.Call_WheelProc(pParam, ret);
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_ScrollProc(PSCROLLPARAM pParam, SSP_STATUS &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive) {
			hr = cur.Call_ScrollProc(pParam, ret);
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_GetIcon(DWORD dwType, HICON &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive) {
			hr = cur.Call_GetIcon(dwType, ret);
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_GetString(DWORD dwType, LPSTR lpStr, UINT uiSize, int &ret)
{
	HRESULT hr = S_OK;
	POSITION sp = GetHeadPosition();
	CStringW wStr;
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive) {
			hr = cur.Call_GetString(dwType, wStr);
			CStringA aStr = CW2A(wStr);
			::strncpy_s(lpStr, uiSize, aStr, uiSize - 1);
			ret = ::strlen(lpStr);
			break;
		}
	}
	return hr;
}

HRESULT CSSPScripts::Call_TimerProc(void)
{
	HRESULT hr = S_OK;
	// Call_InitProc で SSP_TIMER を返しているスクリプトだけ Call_TimerProc
	POSITION sp = GetHeadPosition();
	while (sp != NULL) {
		CSSPScript &cur = GetNext(sp);
		if (cur.m_bActive && cur.m_bTimerEnabled) {
			hr = cur.Call_TimerProc();
		}
	}
	return hr;
}
