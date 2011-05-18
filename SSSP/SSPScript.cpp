#include "StdAfx.h"
#include "SSPScript.h"
#include "win32consts.h"
#include "smartscrollconsts.h"
#include "ssspconsts.h"

LPOLESTR ScriptNames[] = {
	L"ApplyProc",
	L"InitProc",
	L"ReleaseProc",
	L"ButtonProc",
	L"GetWheelEffect",
	L"WheelProc",
	L"ScrollProc",
	L"GetIcon",
	L"GetString",
	L"TimerProc",

	L"onLoad",
	L"isApplicable",
	L"onAttach",
	L"onDetach",
	L"onButton",
	L"onButtonDown",
	L"onButtonUp",
	L"onWheel",
	L"onScroll",
	L"onTimer",
	
	L"onButtonClick",
	L"onButtonDoubleClick",
	L"onButtonTripleClick",
	L"onButtonLongPress",
	L"onButtonRelease",
	NULL
};

CSSPScript::CSSPScript(void):
	m_bActive(FALSE),
	m_bWheelEnabled(TRUE),
	m_bDefaultWheel(TRUE),
	m_bDefaultScroll(TRUE),
	m_bTimerEnabled(FALSE),
	m_pIconList(NULL)
{
	//SspTraceOut(__FUNCTION__);
	::ZeroMemory(&m_ContextData, sizeof(m_ContextData));
	::CoCreateInstance(
		CLSID_JScript,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IActiveScript,
		reinterpret_cast<void **>(&m_pActiveScript)
	);
	m_pActiveScriptParse = m_pActiveScript;
	m_pActiveScript->SetScriptSite(this);
	m_ButtonStatus.SetClient(this);
}

CSSPScript::~CSSPScript(void)
{
	//SspTraceOut(__FUNCTION__);
	Stop();
	m_pActiveScript->Close();
	m_pActiveScript->SetScriptSite(NULL);
	m_pActiveScriptParse = NULL;
	m_pActiveScript = NULL;
	ReleaseAllIcons();
}

HRESULT STDMETHODCALLTYPE CSSPScript::QueryInterface(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject
) {
	if (riid == IID_IActiveScriptSite || riid == IID_IUnknown || riid == IID_IDispatch) {
		*ppvObject = reinterpret_cast<void *>(this);
		m_dwRef++;
		return S_OK;
	} else {
		return E_NOINTERFACE;
	}
}

ULONG STDMETHODCALLTYPE CSSPScript::AddRef(void)
{
	return ++m_dwRef;
}

ULONG STDMETHODCALLTYPE CSSPScript::Release(void)
{
	if (--m_dwRef == 0) {
		return 0;
	}
	return m_dwRef;
}

HRESULT STDMETHODCALLTYPE CSSPScript::GetLCID(/* [out] */ LCID *plcid)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSSPScript::GetItemInfo(/* [in] */ LPCOLESTR pstrName, /* [in] */ DWORD dwReturnMask, /* [out] */ IUnknown **ppiunkItem, /* [out] */ ITypeInfo **ppti)
{
	if (ppti != NULL) {
		*ppti = NULL;
		
		if (dwReturnMask & SCRIPTINFO_ITYPEINFO) {
			return TYPE_E_ELEMENTNOTFOUND;
		}
	}

	if (ppiunkItem != NULL) {
		*ppiunkItem = NULL;

		if (dwReturnMask & SCRIPTINFO_IUNKNOWN) {
			HRESULT hr;
			if (::_wcsicmp(L"SmartScroll", pstrName) == 0) {
				ISmartScrollAPIPtr pSS;
				hr = CSmartScrollAPI::CreateInstance(&pSS);
				if (FAILED(hr) || pSS == NULL) {		// エラー
					return hr;
				}
				((CSmartScrollAPI *)(ISmartScrollAPI *)pSS)->m_pHost = this;
				hr = pSS->QueryInterface(IID_IUnknown, (void **)ppiunkItem);
				if (FAILED(hr) || ppiunkItem == NULL) {			// エラー
					return hr;
				}
				return S_OK;
			} else if (::_wcsicmp(L"SSSP", pstrName) == 0) {
				ISSSPAPIPtr pSP;
				hr = CSSSPAPI::CreateInstance(&pSP);
				if (FAILED(hr) || pSP == NULL) {
					return E_FAIL;
				}
				((CSSSPAPI *)(ISSSPAPI *)pSP)->m_pScriptPluginContext = this;
				hr = pSP->QueryInterface(IID_IUnknown, (void **)ppiunkItem);
				if (FAILED(hr) || ppiunkItem == NULL) {
					return E_FAIL;
				}
				return S_OK;
			} else if (::_wcsicmp(L"Win32API", pstrName) == 0) {
				IWin32APIPtr pWF;
				CWin32API::CreateInstance(&pWF);
				if (pWF == NULL) {
					return E_FAIL;
				}
				m_pActiveScript->GetScriptDispatch(NULL, &((CWin32API *)(IWin32API *)pWF)->m_pDispScript);
				pWF->QueryInterface(IID_IUnknown, (void **)ppiunkItem);
				if (ppiunkItem == NULL) {
					return E_FAIL;
				}
				return S_OK;
			} else if (::_wcsicmp(L"Win32APIConsts", pstrName) == 0) {
				CDispatchConstProperties *win32consts = new CDispatchConstProperties(win32consts_);
				win32consts->AddRef();
				*ppiunkItem = win32consts;
				win32consts->AddRef();
				return S_OK;
			} else if (::_wcsicmp(L"SmartScrollConsts", pstrName) == 0) {
				CDispatchConstProperties *smartscrollconsts = new CDispatchConstProperties(smartscrollconsts_);
				smartscrollconsts->AddRef();
				*ppiunkItem = smartscrollconsts;
				smartscrollconsts->AddRef();
				return S_OK;
			} else if (::_wcsicmp(L"SSSPConsts", pstrName) == 0) {
				CDispatchConstProperties *ssspconsts = new CDispatchConstProperties(ssspconsts_);
				ssspconsts->AddRef();
				*ppiunkItem = ssspconsts;
				ssspconsts->AddRef();
				return S_OK;
			}
		}
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSSPScript::GetDocVersionString(BSTR *pbstrVersion)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSSPScript::OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSSPScript::OnStateChange(SCRIPTSTATE ssScriptState)
{
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSSPScript::OnScriptError(IActiveScriptError *pscripterror)
{
	CString msg;
	msg.Format(_T("%s [main script]: %s"), _T(__FUNCTION__), GetMainScriptFilePath());
	//SspTraceOut(CT2A(msg));
	
	DWORD ctx;
	ULONG srow;
	LONG scol;
	pscripterror->GetSourcePosition(&ctx, &srow, &scol);
	srow++;
	scol++;

	// 実ファイル名と行数を計算
	CString scriptFile;
	ULONG lineCount = 0;
	for (POSITION pos = m_ScriptFiles.GetHeadPosition(); pos != NULL; m_ScriptFiles.GetNext(pos)) {
		CScriptFileInfo &sfi = m_ScriptFiles.GetAt(pos);
		if (srow >= lineCount && srow < lineCount + sfi.m_LineCount) {
			scriptFile = sfi.m_ScriptFilePath;
			srow -= lineCount;
			break;
		}
		lineCount += sfi.m_LineCount;
	}
	
	EXCEPINFO ei;
	pscripterror->GetExceptionInfo(&ei);
	msg.Format(_T("%s [script]: %s"), _T(__FUNCTION__), scriptFile);
	SspTraceOut(CT2A(msg));
	
	if (ei.bstrDescription != NULL) {
		msg.Format(_T("  %s (scode:0x%8.8x)"), ei.bstrDescription, ei.scode);
		SspTraceOut(CT2A(msg));
	} else {
		msg.Format(_T("  scode:0x%8.8x"), ei.scode);
		SspTraceOut(CT2A(msg));
	}
	
	if (ei.bstrSource != NULL) {
		msg.Format(_T("  %s"), ei.bstrSource);
		SspTraceOut(CT2A(msg));
	}
	
	msg.Format(_T("  line:%d, col:%d"), srow, scol);
	SspTraceOut(CT2A(msg));
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSSPScript::OnEnterScript(void)
{
	//SspTraceOut(__FUNCTION__);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSSPScript::OnLeaveScript(void)
{
	//SspTraceOut(__FUNCTION__);
	return S_OK;
}

void CSSPScript::ReleaseAllIcons(void)
{
	if (m_pIconList != NULL) {
		for (int i = 0; i < SSP_BUTTON_NUM; i++) {
			m_pIconList->ReleaseIcon(m_ButtonIconImages[i]);
			m_ButtonIconImages[i] = 0;
		}
	}
}

HRESULT CSSPScript::Load(LPCTSTR lpszFile)
{
	ReleaseAllIcons();
	m_ScriptFiles.RemoveAll();
	m_LoadingContext.RemoveAll();
	m_TotalLineCount = 0;
	
	HRESULT hr;
	try {
		m_pActiveScriptParse->InitNew();
		hr = m_pActiveScript->AddNamedItem(L"SmartScroll", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_GLOBALMEMBERS);
		if (FAILED(hr)) SspTraceOut("AddNamedItem(\"SmartScroll\") failed.");
		hr = m_pActiveScript->AddNamedItem(L"SSSP", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_GLOBALMEMBERS);
		if (FAILED(hr)) SspTraceOut("AddNamedItem(\"SSSP\") failed.");
		hr = m_pActiveScript->AddNamedItem(L"Win32API", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_GLOBALMEMBERS);
		if (FAILED(hr)) SspTraceOut("AddNamedItem(\"Win32API\") failed.");
		hr = m_pActiveScript->AddNamedItem(L"Win32APIConsts", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_GLOBALMEMBERS);
		if (FAILED(hr)) SspTraceOut("AddNamedItem(\"Win32APIConsts\") failed.");
		hr = m_pActiveScript->AddNamedItem(L"SmartScrollConsts", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_GLOBALMEMBERS);
		if (FAILED(hr)) SspTraceOut("AddNamedItem(\"SmartScrollConsts\") failed.");
		hr = m_pActiveScript->AddNamedItem(L"SSSPConsts", SCRIPTITEM_ISVISIBLE | SCRIPTITEM_ISPERSISTENT | SCRIPTITEM_GLOBALMEMBERS);
		if (FAILED(hr)) SspTraceOut("AddNamedItem(\"SSSPConsts\") failed.");
		
		hr = Stop();
		if (FAILED(hr)) {
			throw hr;
		}
		CStringW wstrScript;
		FILETIME ft;
		hr = LoadScriptFile(lpszFile, wstrScript, ft);
		if (FAILED(hr)) {
			throw hr;
		}
		ULONG lineCount = 0;
		ULONG scrlen = wstrScript.GetLength();
		for (ULONG i = 0; i < scrlen; i++) {
			if (wstrScript[i] == _T('\n')) {
				lineCount++;
			}
		}
		POSITION pos = m_ScriptFiles.AddTail();
		CScriptFileInfo &sfi = m_ScriptFiles.GetAt(pos);
		sfi.m_LineCount = lineCount;
		sfi.SetFilePath(lpszFile);
		sfi.m_Timestamp = ft;
		m_LoadingContext.Push(sfi);
		hr = ParseScriptText(wstrScript);
		m_LoadingContext.Pop();
		if (FAILED(hr)) {
			throw hr;
		}
		hr = Start();
		if (FAILED(hr)) {
			throw hr;
		}
	}
	catch (...) {
	}
	return hr;
}

HRESULT CSSPScript::Reload(void)
{
	CString fn = GetMainScriptFilePath();
	return Load(fn);
}

HRESULT CSSPScript::DoImport(LPCTSTR lpszFile)
{
	if (m_LoadingContext.GetCount() == 0) {
		SspTraceOut("実行不可能なコンテクストで Import が使用されました");
		return E_FAIL;
	}
	
	CPath path = lpszFile;
	if (path.IsRelative()) {
		CScriptFileInfo &sfi = m_LoadingContext.GetTop();
		path.m_strPath = sfi.m_ScriptFileDir;
		path.Append(lpszFile);
		path.Canonicalize();
	}
	
	for (POSITION pos = m_ScriptFiles.GetHeadPosition(); pos != NULL; m_ScriptFiles.GetNext(pos)) {
		CScriptFileInfo &sfi = m_ScriptFiles.GetAt(pos);
		if (sfi.m_ScriptFilePath == path.m_strPath) {
			SspTraceOut("スクリプトのインポートが重複または循環しています");
			return E_FAIL;
		}
	}

	HRESULT hr;
	try {
		CStringW wstrScript;
		//hr = Stop();
		FILETIME ft;
		hr = LoadScriptFile(path, wstrScript, ft);
		if (FAILED(hr)) {
			throw hr;
		}
		ULONG lineCount = 1;
		ULONG scrlen = wstrScript.GetLength();
		for (ULONG i = 0; i < scrlen; i++) {
			if (wstrScript[i] == _T('\n')) {
				lineCount++;
			}
		}
		POSITION pos = m_ScriptFiles.AddTail();
		CScriptFileInfo &sfi = m_ScriptFiles.GetAt(pos);
		sfi.m_LineCount = lineCount;
		sfi.SetFilePath(path);
		sfi.m_Timestamp = ft;
		m_LoadingContext.Push(sfi);
		hr = ParseScriptText(wstrScript);
		m_LoadingContext.Pop();
		if (FAILED(hr)) {
			throw hr;
		}
	}
	catch (...) {
		return hr;
	}
	return S_OK;
}

HRESULT CSSPScript::ParseScriptText(LPCWSTR lpszScript)
{
	HRESULT hr;
	ULONG uStartLine = m_TotalLineCount;
	CScriptFileInfo &sfi = m_LoadingContext.GetTop();
	m_TotalLineCount += sfi.m_LineCount;
	hr = m_pActiveScriptParse->ParseScriptText(
		lpszScript,
		NULL,
		NULL,
		NULL,
		0,
		uStartLine,
		SCRIPTTEXT_ISEXPRESSION,
		&sfi.m_Result,
		&sfi.m_ExcepInfo
	);
	m_bScriptEnabled = (sfi.m_Result.vt != VT_BOOL || sfi.m_Result.boolVal != VARIANT_FALSE);
	return hr;
}

HRESULT CSSPScript::LoadScriptFile(LPCTSTR lpszFilename, CStringW &wstrScript, FILETIME &ts)
{
	HRESULT hr;
	CAtlFile file;
	try {
		hr = file.Create(lpszFilename, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
		if (FAILED(hr)) {
			throw hr;
		}
		::GetFileTime(file, NULL, NULL, &ts);
		ULONGLONG fsize;
		hr = file.GetSize(fsize);
		if (FAILED(hr)) {
			throw hr;
		}
		if (fsize > 65536L) {
			throw ERROR_FILE_TOO_LARGE;
		}
		DWORD dwRead;
		WORD wBOM;
		hr = file.Read(&wBOM, 2, dwRead);
		if (FAILED(hr)) {
			throw hr;
		}
		if (dwRead < 2 || wBOM == 0xFFFE) {
			throw ERROR_INVALID_DATA;
		}
		DWORD dwSize;
		DWORD dwChar;
		if (wBOM == 0xFEFF) {
			dwSize = static_cast<DWORD>(fsize - 2);
			dwChar = dwSize / sizeof(TCHAR);
			LPWSTR buffer = wstrScript.GetBufferSetLength(dwChar + 1);
			hr = file.Read(buffer, dwSize, dwRead);
			if (FAILED(hr)) {
				throw hr;
			}
			if (dwRead < dwSize) {
				throw ERROR_READ_FAULT;
			}
			buffer[dwChar] = L'\0';
			wstrScript.ReleaseBuffer();
		} else {
			dwSize = static_cast<DWORD>(fsize);
			dwChar = dwSize;
			CStringA astrScript;
			LPSTR buffer = astrScript.GetBufferSetLength(dwChar + 1);
			file.Seek(0, SEEK_SET);
			hr = file.Read(buffer, dwSize, dwRead);
			if (FAILED(hr)) {
				throw hr;
			}
			if (dwRead < dwSize) {
				throw ERROR_READ_FAULT;
			}
			buffer[dwChar] = '\0';
			wstrScript = CA2W(buffer);
		}
	}
	catch (HRESULT e) {
		hr = e;
	}
	return hr;
}

HRESULT CSSPScript::Start(void)
{
	HRESULT hr;
	hr = m_pActiveScript->SetScriptState(SCRIPTSTATE_CONNECTED);
	if (FAILED(hr)) {
		return hr;
	}
	hr = m_pActiveScript->GetScriptDispatch(NULL, &m_pDispScript);

	if (SUCCEEDED(hr)) {
		DISPID did;
		HRESULT tmpHr;
		for (UINT i = 0; ScriptNames[i] != NULL; i++) {
			did = DISPID_UNKNOWN;
			tmpHr = m_pDispScript->GetIDsOfNames(IID_NULL, &ScriptNames[i], 1, LOCALE_USER_DEFAULT, &did);
			m_DispIds[ScriptNames[i]] = did;
		}
	}
	Call_onLoad();
	return hr;
}

HRESULT CSSPScript::Stop(void)
{
	SCRIPTSTATE ss;
	m_pActiveScript->GetScriptState(&ss);
	if (ss == SCRIPTSTATE_CONNECTED) {
		m_pActiveScript->SetScriptState(SCRIPTSTATE_DISCONNECTED);
	}
	m_DispIds.RemoveAll();
	m_pDispScript = NULL;
	return S_OK;
}

BOOL CSSPScript::SetTitle(LPCTSTR lpszText)
{
	m_Title = lpszText;
	return TRUE;
}

BOOL CSSPScript::SetButtonParams(DWORD dwButton, DWORD dwDoubleClick, DWORD dwTripleClick, DWORD dwLongPress)
{
	return m_ButtonStatus.SetButtonParams(dwButton, dwDoubleClick, dwTripleClick, dwLongPress);
}

BOOL CSSPScript::SetButtonText(DWORD dwButton, LPCTSTR lpszText)
{
	if (dwButton >= SSP_BUTTON_NUM) {
		return FALSE;
	}
	m_ButtonTexts[dwButton] = lpszText;
	return TRUE;
}

BOOL CSSPScript::SetButtonImage(DWORD dwButton, LPCTSTR lpszImageFilename)
{
	CPath path = lpszImageFilename;
	ULONG uIndex = 0;
	if (!path.m_strPath.IsEmpty()) {
		if (path.IsRelative()) {
			CScriptFileInfo &sfi = m_ScriptFiles.GetHead();
			path.m_strPath = sfi.m_ScriptFileDir;
			path.Append(lpszImageFilename);
			path.Canonicalize();
		}
		uIndex = ::PathParseIconLocation(path.m_strPath.GetBuffer());
		path.m_strPath.ReleaseBuffer();
	}
	if (path.m_strPath.IsEmpty()) {
		m_ButtonIconImages[dwButton] = 0;
		return TRUE;
	}
	if (m_pIconList == NULL) {
		return FALSE;
	}
	ULONG oldToken = m_ButtonIconImages[dwButton];
	m_ButtonIconImages[dwButton] = m_pIconList->AddIcon(path, uIndex);
	m_pIconList->ReleaseIcon(oldToken);
	return TRUE;
}

BOOL CSSPScript::SetWheelParams(DWORD dwThreshold, DWORD dwTimeout)
{
	m_ContextData.dwWheelThreshold = dwThreshold;
	m_ContextData.dwWheelResetTime = dwTimeout;
	return TRUE;
}

BOOL CSSPScript::SetScrollParams(DWORD dwThreshold, DWORD dwTimeout)
{
	m_ContextData.dwScrollThresholdX = dwThreshold;
	m_ContextData.dwScrollThresholdY = dwThreshold;
	m_ContextData.dwScrollResetTimeX = dwTimeout;
	m_ContextData.dwScrollResetTimeY = dwTimeout;
	return TRUE;
}

BOOL CSSPScript::EnableTimer(BOOL bEnabled)
{
	m_bTimerEnabled = bEnabled;
	return TRUE;
}

BOOL CSSPScript::EnableWheel(BOOL bEnabled)
{
	m_bWheelEnabled = bEnabled;
	return TRUE;
}

BOOL CSSPScript::SetWheelDefault(BOOL bDefault)
{
	m_bDefaultWheel = bDefault;
	return TRUE;
}

BOOL CSSPScript::SetScrollDefault(BOOL bDefault)
{
	m_bDefaultScroll = bDefault;
	return TRUE;
}

HRESULT CSSPScript::Import(LPCTSTR lpszFilename, VARIANT *vRet)
{
	return DoImport(lpszFilename);
}

BOOL CSSPScript::GetContextScriptName(CString &str)
{
	CScriptFileInfo &f = m_ScriptFiles.GetHead();
	str = f.m_ScriptFilename;
	return TRUE;
}

BOOL CSSPScript::IsScriptFileExists()
{
	BOOL res = TRUE;
	for (POSITION pos = m_ScriptFiles.GetHeadPosition(); pos != NULL; m_ScriptFiles.GetNext(pos)) {
		CScriptFileInfo &f = m_ScriptFiles.GetAt(pos);
		res &= f.IsExists();
	}
	return res;
}

BOOL CSSPScript::IsScriptFileUpToDate()
{
	BOOL res = TRUE;
	for (POSITION pos = m_ScriptFiles.GetHeadPosition(); pos != NULL; m_ScriptFiles.GetNext(pos)) {
		CScriptFileInfo &f = m_ScriptFiles.GetAt(pos);
		res &= f.IsUpToDate();
	}
	return res;
}

LPCTSTR CSSPScript::GetMainScriptFilename()
{
	return m_ScriptFiles.GetHead().m_ScriptFilename;
}

LPCTSTR CSSPScript::GetMainScriptFilePath()
{
	return m_ScriptFiles.GetHead().m_ScriptFilePath;
}

UINT CSSPScript::GetImportPathes(CAtlList<CString> &list)
{
	POSITION pos = m_ScriptFiles.GetHeadPosition();
	if (pos != NULL) {
		m_ScriptFiles.GetNext(pos);
	}
	while (pos != NULL) {
		CScriptFileInfo &sfi = m_ScriptFiles.GetAt(pos);
		if (list.Find(sfi.m_ScriptFileDir) == NULL) {
			list.AddTail(sfi.m_ScriptFileDir);
		}
		m_ScriptFiles.GetNext(pos);
	}
	return list.GetCount();
}






void CSSPScript::OnClicked(DWORD iButton)
{
	//CString msg;
	//msg.Format(_T("BUTTON %d -- CLICKED"), iButton + 1);
	//SspTraceOut(CT2A(msg));
	HRESULT hr = Call_onButtonClick(m_hWnd_Mouse, iButton);
}

void CSSPScript::OnDoubleClicked(DWORD iButton)
{
	//CString msg;
	//msg.Format(_T("BUTTON %d -- DOUBLE CLICKED"), iButton + 1);
	//SspTraceOut(CT2A(msg));
	HRESULT hr = Call_onButtonDoubleClick(m_hWnd_Mouse, iButton);
}

void CSSPScript::OnTripleClicked(DWORD iButton)
{
	//CString msg;
	//msg.Format(_T("BUTTON %d -- TRIPLE CLICKED"), iButton + 1);
	//SspTraceOut(CT2A(msg));
	HRESULT hr = Call_onButtonTripleClick(m_hWnd_Mouse, iButton);
}

void CSSPScript::OnLongPressed(DWORD iButton, DWORD iDuration)
{
	//CString msg;
	//msg.Format(_T("BUTTON %d -- PRESSING %dms ..."), iButton + 1, iDuration);
	//SspTraceOut(CT2A(msg));
	HRESULT hr = Call_onButtonLongPress(m_hWnd_Mouse, iButton, iDuration);
}

void CSSPScript::OnReleaseLongPress(DWORD iButton, DWORD iDuration)
{
	//CString msg;
	//msg.Format(_T("BUTTON %d -- RELEASED (%dms)"), iButton + 1, iDuration);
	//SspTraceOut(CT2A(msg));
	HRESULT hr = Call_onButtonRelease(m_hWnd_Mouse, iButton, iDuration);
}











HRESULT CSSPScript::Call_ApplyProc(HWND hWnd, SSP_STATUS &ret)
{
	if (!m_bScriptEnabled) {
		m_bActive = FALSE;
		ret = SSP_FALSE;
		return S_OK;
	}
	
	HRESULT hr;
	
	BOOL bRet;
	hr = Call_isApplicable(hWnd, bRet);
	if (SUCCEEDED(hr)) {	
		m_bActive = bRet;
		ret = bRet ? SSP_TRUE : SSP_FALSE;
	}

	DISPID did = m_DispIds[L"ApplyProc"];
	if (did == DISPID_UNKNOWN) {
		if (SUCCEEDED(hr)) {
			return hr;
		}
		m_bActive = FALSE;
		return E_NOTIMPL;
	}

	VARIANT arg[1];
	arg[0].vt = VT_UI4;
	arg[0].intVal = reinterpret_cast<DWORD>(hWnd);
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 1;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);

	if (SUCCEEDED(hr) && vRet.vt == VT_I4) {
		DWORD iRet = vRet.intVal;
		ret = static_cast<SSP_STATUS>(iRet);
	}
	
	m_bActive = (ret == SSP_TRUE);
	return hr;
}

HRESULT CSSPScript::Call_InitProc(PINITPARAM pParam, SSP_STATUS &ret)
{
	m_ButtonStatus.Reset();
	
	HRESULT hr;
	
	BOOL bRet;
	hr = Call_onAttach(pParam->hwndApp, pParam->dwModel, pParam->dwVersion, bRet);
	if (SUCCEEDED(hr)) {
		ret = bRet ? SSP_TRUE : SSP_FALSE;
	}
	
	DISPID did = m_DispIds[L"InitProc"];
	if (did == DISPID_UNKNOWN) {
		if (SUCCEEDED(hr)) {
			return hr;
		}
		return E_NOTIMPL;
	}
	
	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(pParam->hwndApp);
	arg[1].vt = VT_I4;
	arg[1].intVal = pParam->dwModel;
	arg[0].vt = VT_I4;
	arg[0].intVal = pParam->dwVersion;
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);

	if (SUCCEEDED(hr) && vRet.vt == VT_I4) {
		DWORD iRet = vRet.intVal;
		ret = static_cast<SSP_STATUS>(iRet);
	}
	
	m_bTimerEnabled = (ret == SSP_TIMER);
	return hr;
}

HRESULT CSSPScript::Call_ReleaseProc(void)
{
	m_bActive = FALSE;
	m_ButtonStatus.Reset();

	HRESULT hr;
	hr = Call_onDetach();
	
	DISPID did = m_DispIds[L"ReleaseProc"];
	if (did == DISPID_UNKNOWN) {
		if (SUCCEEDED(hr)) {
			return hr;
		}
		return E_NOTIMPL;
	}
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	return hr;
}

HRESULT CSSPScript::Call_ButtonProc(PBUTTONPARAM pParam, SSP_STATUS &ret)
{
	ret = SSP_TRUE;
	HRESULT hr;
	
	m_hWnd_Mouse = pParam->hwndMouse;
	m_ButtonStatus.ButtonProc(pParam->dwType);
	
	hr = Call_onButton(pParam->hwndMouse, BUTTON_NO(pParam->dwType), m_ButtonStatus.GetButtonStates());
	if ((BUTTON_EVENT(pParam->dwType)) == 0) {
		// DOWN
		Call_onButtonDown(pParam->hwndMouse, BUTTON_NO(pParam->dwType), m_ButtonStatus.GetButtonStates());
	} else {
		// UP
		Call_onButtonUp(pParam->hwndMouse, BUTTON_NO(pParam->dwType), m_ButtonStatus.GetButtonStates());
	}
	
	DISPID did = m_DispIds[L"ButtonProc"];
	if (did == DISPID_UNKNOWN) {
		if (SUCCEEDED(hr)) {
			return hr;
		}
		return E_NOTIMPL;
	}
	
	VARIANT arg[2];
	arg[1].vt = VT_UI4;
	arg[1].intVal = reinterpret_cast<DWORD>(pParam->hwndMouse);
	arg[0].vt = VT_I4;
	arg[0].intVal = pParam->dwType;
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 2;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	if (SUCCEEDED(hr) && vRet.vt == VT_I4) {
		ret = static_cast<SSP_STATUS>(vRet.intVal);
	}

	return hr;
}

HRESULT CSSPScript::Call_GetWheelEffect(SSP_STATUS &ret)
{
	ret = m_bWheelEnabled ? SSP_TRUE : SSP_FALSE;
	
	HRESULT hr;
	DISPID did = m_DispIds[L"GetWheelEffect"];
	if (did == DISPID_UNKNOWN) {
		return S_OK;
	}

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	if (SUCCEEDED(hr) && vRet.vt == VT_I4) {
		ret = static_cast<SSP_STATUS>(vRet.intVal);
	}
	return hr;
}

HRESULT CSSPScript::Call_WheelProc(PWHEELPARAM pParam, SSP_STATUS &ret)
{
	ret = m_bDefaultWheel ? SSP_FALSE : SSP_TRUE;
	HRESULT hr;
	
	if (m_bDefaultWheel) {
		hr = S_FALSE;
	} else {
		hr = Call_onWheel(pParam->hwndMouse, pParam->lRoll, pParam->dwDirection);
	}
	
	DISPID did = m_DispIds[L"WheelProc"];
	if (did == DISPID_UNKNOWN) {
		if (SUCCEEDED(hr)) {
			return hr;
		}
		return E_NOTIMPL;
	}
	
	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(pParam->hwndMouse);
	arg[1].vt = VT_I4;
	arg[1].intVal = pParam->lRoll;
	arg[0].vt = VT_I4;
	arg[0].intVal = pParam->dwDirection;
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	if (SUCCEEDED(hr) && vRet.vt == VT_I4) {
		ret = static_cast<SSP_STATUS>(vRet.intVal);
	}

	return hr;
}

HRESULT CSSPScript::Call_ScrollProc(PSCROLLPARAM pParam, SSP_STATUS &ret)
{
	ret = m_bDefaultScroll ? SSP_FALSE : SSP_TRUE;
	
	HRESULT hr;
	
	if (m_bDefaultScroll) {
		hr = S_FALSE;
	} else {
		hr = Call_onScroll(pParam->hwndMouse, pParam->lX, pParam->lY, pParam->dwDirection);
	}
	
	DISPID did = m_DispIds[L"ScrollProc"];
	if (did == DISPID_UNKNOWN) {
		if (SUCCEEDED(hr)) {
			return hr;
		}
		return E_NOTIMPL;
	}
	
	VARIANT arg[6];
	arg[5].vt = VT_UI4;
	arg[5].intVal = reinterpret_cast<DWORD>(pParam->hwndMouse);
	arg[4].vt = VT_UI4;
	arg[4].intVal = reinterpret_cast<DWORD>(pParam->hwndOwner);
	arg[3].vt = VT_UI4;
	arg[3].intVal = reinterpret_cast<DWORD>(pParam->hwndHScroll);
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(pParam->hwndVScroll);
	arg[1].vt = VT_I4;
	arg[1].intVal = pParam->lX;
	arg[0].vt = VT_I4;
	arg[0].intVal = pParam->lY;
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 6;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	if (SUCCEEDED(hr) && vRet.vt == VT_I4) {
		ret = static_cast<SSP_STATUS>(vRet.intVal);
	}

	return hr;
}

HRESULT CSSPScript::Call_GetIcon(DWORD dwType, HICON &ret)
{
	if (m_pIconList == NULL) {
		ret = NULL;
		return S_FALSE;
	}
	ret = m_pIconList->GetIcon(m_ButtonIconImages[dwType]);
	return S_OK;
	
	//HRESULT hr;
	//DISPID did = m_DispIds[L"GetIcon"];
	//if (did == DISPID_UNKNOWN) {
	//	return E_NOTIMPL;
	//}

	//VARIANT arg[1];
	//arg[0].vt = VT_I4;
	//arg[0].intVal = dwType;
	//
	//UINT aErr;
	//DISPPARAMS dp = { NULL, NULL, 0, 0 };
	//dp.cArgs = 1;
	//dp.rgvarg = arg;
	//VARIANT vRet;
	//hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	//
	//if (SUCCEEDED(hr) && vRet.vt == VT_BSTR) {
	//	CComVariant v;
	//	v.Attach(&vRet);
	//	ret = v;
	//	v.Detach(&vRet);
	//}
	//return hr;
}

HRESULT CSSPScript::Call_GetString(DWORD dwType, CStringW &ret)
{
	if (dwType == SSP_NAME_EX) {
		ret = CT2A(m_Title);
	} else if ((dwType >= SSP_BUTTON1 && dwType <= SSP_BUTTON8) || dwType == SSP_BUTTON_WHEEL) {
		ret = CT2A(m_ButtonTexts[dwType]);
	}
	
	HRESULT hr;
	DISPID did = m_DispIds[L"GetString"];
	if (did == DISPID_UNKNOWN) {
		return S_OK;
	}

	VARIANT arg[1];
	arg[0].vt = VT_I4;
	arg[0].intVal = dwType;
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 1;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	if (SUCCEEDED(hr) && vRet.vt == VT_BSTR) {
		CComVariant v;
		v.Attach(&vRet);
		ret = v;
		v.Detach(&vRet);
	}
	return hr;
}

HRESULT CSSPScript::Call_TimerProc(void)
{
	HRESULT hr;
	
	hr = Call_onTimer();
	
	DISPID did = m_DispIds[L"TimerProc"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onLoad(void)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onLoad"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_isApplicable(HWND hWnd, BOOL &bRet)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"isApplicable"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	VARIANT arg[1];
	arg[0].vt = VT_UI4;
	arg[0].intVal = reinterpret_cast<DWORD>(hWnd);

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 1;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	if (vRet.vt == VT_BOOL) {
		bRet = (vRet.boolVal == VARIANT_TRUE);
	} else {
		bRet = FALSE;
	}
	return hr;
}

HRESULT CSSPScript::Call_onAttach(HWND hWnd, DWORD dwModel, DWORD dwVersion, BOOL &bRet)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onAttach"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[1].vt = VT_UI4;
	arg[1].intVal = dwModel;
	arg[0].vt = VT_UI4;
	arg[0].intVal = dwVersion;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	if (vRet.vt == VT_BOOL) {
		bRet = (vRet.boolVal == VARIANT_TRUE);
	} else {
		bRet = FALSE;
	}
	return hr;
}

HRESULT CSSPScript::Call_onDetach(void)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onDetach"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onButton(HWND hWnd, DWORD dwButton, LPCTSTR lpszButtonStates)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButton"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	CComBSTR bstr(lpszButtonStates);
	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[1].vt = VT_UI4;
	arg[1].intVal = dwButton;
	arg[0].vt = VT_BSTR;
	arg[0].bstrVal = bstr;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onButtonDown(HWND hWnd, DWORD dwButton, LPCTSTR lpszButtonStates)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButtonDown"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	CComBSTR bstr(lpszButtonStates);
	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[1].vt = VT_UI4;
	arg[1].intVal = dwButton;
	arg[0].vt = VT_BSTR;
	arg[0].bstrVal = bstr;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onButtonUp(HWND hWnd, DWORD dwButton, LPCTSTR lpszButtonStates)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButtonUp"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	CComBSTR bstr(lpszButtonStates);
	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[1].vt = VT_UI4;
	arg[1].intVal = dwButton;
	arg[0].vt = VT_BSTR;
	arg[0].bstrVal = bstr;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onWheel(HWND hWnd, LONG lRoll, DWORD dwDirection)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onWheel"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	BOOL eY = (lRoll != 0);
	
	DWORD dwNow = ::GetTickCount();
	if (eY) {
		if (m_ContextData.dwWheelResetTime != 0) {
			if (dwNow - m_ContextData.dwLastWheel > m_ContextData.dwWheelResetTime) {
				m_ContextData.dwWheeling = 0;
				eY = FALSE;
			}
			if (m_ContextData.dwWheeling < m_ContextData.dwWheelThreshold) {
				m_ContextData.dwWheeling += abs(lRoll);
				eY = m_ContextData.dwWheeling >= m_ContextData.dwWheelThreshold;
			}
		}
		m_ContextData.dwLastWheel = dwNow;
	}
	
	if (eY == FALSE) {
		return S_OK;
	}

	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[1].vt = VT_I4;
	arg[1].intVal = lRoll;
	arg[0].vt = VT_UI4;
	arg[0].intVal = dwDirection;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onScroll(HWND hWndMouse, LONG lX, LONG lY, DWORD dwDirection)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onScroll"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}
	
	BOOL eX = (lX != 0);
	BOOL eY = (lY != 0);
	
	DWORD dwNow = ::GetTickCount();
	if (eX) {
		if (m_ContextData.dwScrollThresholdX != 0) {
			if (dwNow - m_ContextData.dwLastScrollX > m_ContextData.dwScrollResetTimeX) {
				m_ContextData.dwScrollingX = 0;
				eX = FALSE;
			}
			if (m_ContextData.dwScrollingX < m_ContextData.dwScrollThresholdX) {
				m_ContextData.dwScrollingX += abs(lX);
				eX = m_ContextData.dwScrollingX >= m_ContextData.dwScrollThresholdX;
			}
		}
		m_ContextData.dwLastScrollX = dwNow;
	}
	if (eY) {
		if (m_ContextData.dwScrollThresholdY != 0) {
			if (dwNow - m_ContextData.dwLastScrollY > m_ContextData.dwScrollResetTimeY) {
				m_ContextData.dwScrollingY = 0;
				eY = FALSE;
			}
			if (m_ContextData.dwScrollingY < m_ContextData.dwScrollThresholdY) {
				m_ContextData.dwScrollingY += abs(lY);
				eY = m_ContextData.dwScrollingY >= m_ContextData.dwScrollThresholdY;
			}
		}
		m_ContextData.dwLastScrollY = dwNow;
	}
	
	if (eX == FALSE && eY == FALSE) {
		return S_OK;
	}

	VARIANT arg[4];
	arg[3].vt = VT_UI4;
	arg[3].intVal = reinterpret_cast<DWORD>(hWndMouse);
	arg[2].vt = VT_I4;
	arg[2].intVal = lX;
	arg[1].vt = VT_I4;
	arg[1].intVal = lY;
	arg[0].vt = VT_UI4;
	arg[0].ulVal = dwDirection;
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 4;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);

	return hr;
}

HRESULT CSSPScript::Call_onButtonClick(HWND hWnd, DWORD dwButton)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButtonClick"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	VARIANT arg[2];
	arg[1].vt = VT_UI4;
	arg[1].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[0].vt = VT_I4;
	arg[0].intVal = dwButton;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 2;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onButtonDoubleClick(HWND hWnd, DWORD dwButton)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButtonDoubleClick"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	VARIANT arg[2];
	arg[1].vt = VT_UI4;
	arg[1].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[0].vt = VT_I4;
	arg[0].intVal = dwButton;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 2;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onButtonTripleClick(HWND hWnd, DWORD dwButton)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButtonTripleClick"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	VARIANT arg[2];
	arg[1].vt = VT_UI4;
	arg[1].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[0].vt = VT_I4;
	arg[0].intVal = dwButton;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 2;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onButtonLongPress(HWND hWnd, DWORD dwButton, DWORD dwDuration)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButtonLongPress"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[1].vt = VT_I4;
	arg[1].intVal = dwButton;
	arg[0].vt = VT_I4;
	arg[0].intVal = dwDuration;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onButtonRelease(HWND hWnd, DWORD dwButton, DWORD dwDuration)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onButtonRelease"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	VARIANT arg[3];
	arg[2].vt = VT_UI4;
	arg[2].intVal = reinterpret_cast<DWORD>(hWnd);
	arg[1].vt = VT_I4;
	arg[1].intVal = dwButton;
	arg[0].vt = VT_I4;
	arg[0].intVal = dwDuration;

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	dp.cArgs = 3;
	dp.rgvarg = arg;
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}

HRESULT CSSPScript::Call_onTimer(void)
{
	HRESULT hr;
	DISPID did = m_DispIds[L"onTimer"];
	if (did == DISPID_UNKNOWN) {
		return E_NOTIMPL;
	}

	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	VARIANT vRet;
	hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vRet, NULL, &aErr);
	
	return hr;
}
