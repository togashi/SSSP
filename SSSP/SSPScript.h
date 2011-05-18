#pragma once

_COM_SMARTPTR_TYPEDEF(IActiveScript, IID_IActiveScript);
_COM_SMARTPTR_TYPEDEF(IActiveScriptSite, IID_IActiveScriptSite);
_COM_SMARTPTR_TYPEDEF(IActiveScriptParse, IID_IActiveScriptParse);

DEFINE_GUID(CLSID_VBScript, 0xb54f3741, 0x5b07, 0x11cf, 0xa4, 0xb0, 0x0, 0xaa, 0x0, 0x4a, 0x55, 0xe8);
DEFINE_GUID(CLSID_JScript, 0xf414c260, 0x6ac0, 0x11cf, 0xb6, 0xd1, 0x00, 0xaa, 0x00, 0xbb, 0xbb, 0x58);

#include "smartscrollapi.h"
#include "ssspapi.h"
#include "win32api.h"
#include "buttonstatus.h"
#include "iconlist.h"

class CSSPScript:
	public IActiveScriptSite,
	public ISmartScrollAPIHost,
	public IScriptPluginContext,
	public IButtonEvent
{
private:
	class CScriptFileInfo {
	public:
		CString m_ScriptFilePath;
		CString m_ScriptFileDir;
		CString m_ScriptFilename;
		ULONG m_LineCount;
		FILETIME m_Timestamp;
		VARIANT m_Result;
		EXCEPINFO m_ExcepInfo;

		void SetFilePath(LPCTSTR lpszFullPath) {
			m_ScriptFilePath = lpszFullPath;
			CPath tmp = lpszFullPath;
			int fni = tmp.FindFileName();
			if (fni >= 0) {
				m_ScriptFileDir = m_ScriptFilePath.Left(fni);
				m_ScriptFilename = m_ScriptFilePath.Mid(fni, MAX_PATH);
			}
		}

		BOOL IsUpToDate() {
			HRESULT hr;
			CAtlFile file;
			hr = file.Create(m_ScriptFilePath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
			if (FAILED(hr)) {
				return TRUE;
			}
			FILETIME ft;
			::GetFileTime(file, NULL, NULL, &ft);
			file.Close();
			return ::CompareFileTime(&ft, &m_Timestamp) == 0;
		}

		BOOL IsExists() {
			CPath p(m_ScriptFilePath);
			return p.FileExists();
		}
	};
	CAtlList<CScriptFileInfo> m_ScriptFiles;

	class CStack: public CAtlList<CScriptFileInfo>
	{
	public:
		void Push(CScriptFileInfo &newone) {
			POSITION pos = AddHead();
			CScriptFileInfo &n = GetAt(pos);
			n = newone;
			//SspTraceOut(CT2A(_T("push: ") + GetTop().m_ScriptFilePath));
		}
		void Pop() {
			POSITION pos = GetHeadPosition();
			RemoveAt(pos);
			//if (GetCount() > 0) {
			//	SspTraceOut(CT2A(_T("pop: ") + GetTop().m_ScriptFilePath));
			//} else {
			//	SspTraceOut("pop: <empty>");
			//}
		}
		CScriptFileInfo &GetTop(void) {
			return GetHead();
		}
	} m_LoadingContext;

	CButtonStatus m_ButtonStatus;
	struct {
		DWORD dwLastScrollX;
		DWORD dwScrollingX;
		DWORD dwScrollThresholdX;
		DWORD dwScrollResetTimeX;
		
		DWORD dwLastScrollY;
		DWORD dwScrollingY;
		DWORD dwScrollThresholdY;
		DWORD dwScrollResetTimeY;
		
		DWORD dwLastWheel;
		DWORD dwWheeling;
		DWORD dwWheelThreshold;
		DWORD dwWheelResetTime;
	} m_ContextData;

	HWND m_hWnd_Mouse;
	
	DWORD m_dwRef;
	DWORD m_TotalLineCount;
	BOOL m_bScriptEnabled;
	
	IActiveScriptPtr m_pActiveScript;
	IActiveScriptParsePtr m_pActiveScriptParse;
	IDispatchPtr m_pDispScript;
	
	CAtlMap<CStringW, DISPID> m_DispIds;
private:
	// ISmartScrollAPIHost
	BOOL GetContextScriptName(CString &str);

	// IButtonEvent
	void OnClicked(DWORD iButton);
	void OnDoubleClicked(DWORD iButton);
	void OnTripleClicked(DWORD iButton);
	void OnLongPressed(DWORD iButton, DWORD iDuration);
	void OnReleaseLongPress(DWORD iButton, DWORD iDuration);

	// IScriptPluginContext
	BOOL SetTitle(LPCTSTR lpszText);
	BOOL SetButtonParams(DWORD dwButton, DWORD dwDoubleClick, DWORD dwTripleClick, DWORD dwLongPress);
	BOOL SetButtonText(DWORD dwButton, LPCTSTR lpszText);
	BOOL SetButtonImage(DWORD dwButton, LPCTSTR lpszImageFilename);
	BOOL SetWheelParams(DWORD dwThreshold, DWORD dwTimeout);
	BOOL SetScrollParams(DWORD dwThreshold, DWORD dwTimeout);
	BOOL EnableTimer(BOOL bEnabled);
	BOOL EnableWheel(BOOL bEnabled);
	BOOL SetWheelDefault(BOOL bDefault);
	BOOL SetScrollDefault(BOOL bDefault);
	HRESULT Import(LPCTSTR lpszFilename, VARIANT *vRet);

	CString m_Title;
	CString m_ButtonTexts[SSP_BUTTON_NUM];
	UINT m_ButtonIconImages[SSP_BUTTON_NUM];
	BOOL m_bWheelEnabled;
	BOOL m_bDefaultWheel;
	BOOL m_bDefaultScroll;

	void ReleaseAllIcons(void);
public:
	CSSPScript(void);
	virtual ~CSSPScript(void);

    // IUnknown
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void __RPC_FAR *__RPC_FAR *ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
	
	// IActiveScriptSite
	HRESULT STDMETHODCALLTYPE GetLCID(LCID *plcid);
    HRESULT STDMETHODCALLTYPE GetItemInfo(LPCOLESTR pstrName, DWORD dwReturnMask, IUnknown **ppiunkItem, ITypeInfo **ppti);    
    HRESULT STDMETHODCALLTYPE GetDocVersionString(BSTR *pbstrVersion);
    HRESULT STDMETHODCALLTYPE OnScriptTerminate(const VARIANT *pvarResult, const EXCEPINFO *pexcepinfo);
    HRESULT STDMETHODCALLTYPE OnStateChange(SCRIPTSTATE ssScriptState);
    HRESULT STDMETHODCALLTYPE OnScriptError(IActiveScriptError *pscripterror);
    HRESULT STDMETHODCALLTYPE OnEnterScript(void);
    HRESULT STDMETHODCALLTYPE OnLeaveScript(void);

public:
	BOOL m_bActive;
	BOOL m_bTimerEnabled;
	IIconList *m_pIconList;

	HRESULT Load(LPCTSTR lpszFile);
	HRESULT Reload(void);
	HRESULT DoImport(LPCTSTR lpszFile);

	HRESULT ParseScriptText(LPCWSTR lpszScript);
	HRESULT LoadScriptFile(LPCTSTR lpszFilename, CStringW &wstrScript, FILETIME &ts);

	HRESULT Start(void);
	HRESULT Stop(void);

	BOOL IsScriptFileExists(void);
	BOOL IsScriptFileUpToDate(void);
	LPCTSTR GetMainScriptFilename(void);
	LPCTSTR GetMainScriptFilePath(void);
	UINT GetImportPathes(CAtlList<CString> &list);
	
	HRESULT Call_ApplyProc(HWND hWnd, SSP_STATUS &ret);
	HRESULT Call_InitProc(PINITPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_ReleaseProc(void);
	HRESULT Call_ButtonProc(PBUTTONPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_GetWheelEffect(SSP_STATUS &ret);
	HRESULT Call_WheelProc(PWHEELPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_ScrollProc(PSCROLLPARAM pParam, SSP_STATUS &ret);
	HRESULT Call_GetIcon(DWORD dwType, HICON &ret);
	HRESULT Call_GetString(DWORD dwType, CStringW &ret);
	HRESULT Call_TimerProc(void);

	// Šg’£
	HRESULT Call_onLoad(void);
	HRESULT Call_isApplicable(HWND hWnd, BOOL &bRet);
	HRESULT Call_onAttach(HWND hWnd, DWORD dwModel, DWORD dwVersion, BOOL &bRet);
	HRESULT Call_onDetach(void);
	HRESULT Call_onButton(HWND hWnd, DWORD dwButton, LPCTSTR lpszButtonStates);
	HRESULT Call_onButtonDown(HWND hWnd, DWORD dwButton, LPCTSTR lpszButtonStates);
	HRESULT Call_onButtonUp(HWND hWnd, DWORD dwButton, LPCTSTR lpszButtonStates);
	HRESULT Call_onWheel(HWND hWnd, LONG lRoll, DWORD dwDirection);
	HRESULT Call_onScroll(HWND hWndMouse, LONG lX, LONG lY, DWORD dwDirection);
	
	HRESULT Call_onButtonClick(HWND hWnd, DWORD dwButton);
	HRESULT Call_onButtonDoubleClick(HWND hWnd, DWORD dwButton);
	HRESULT Call_onButtonTripleClick(HWND hWnd, DWORD dwButton);
	HRESULT Call_onButtonLongPress(HWND hWnd, DWORD dwButton, DWORD dwDuration);
	HRESULT Call_onButtonRelease(HWND hWnd, DWORD dwButton, DWORD dwDuration);
	HRESULT Call_onTimer(void);
};
