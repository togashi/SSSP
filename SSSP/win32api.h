#pragma once

#include "SSSP_i.h"
_COM_SMARTPTR_TYPEDEF(IWin32API, IID_IWin32API);

// CSSSPAPI
class __declspec(uuid("{6D019B15-DE40-4982-93A9-5BDA332FE958}")) ATL_NO_VTABLE CWin32API :
	public CComObjectRoot,
	public CComCoClass<CWin32API, &__uuidof(CWin32API)>,
	public IDispatchImpl<IWin32API, &IID_IWin32API, &CAtlModule::m_libid, -1, -1>
{
public:
	CWin32API(void);
	virtual ~CWin32API(void);

	BEGIN_COM_MAP(CWin32API)
		COM_INTERFACE_ENTRY(IWin32API)
		COM_INTERFACE_ENTRY(IDispatch)
	END_COM_MAP()

	DECLARE_NO_REGISTRY()
	DECLARE_CLASSFACTORY()
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	IDispatchPtr m_pDispScript;
	
	// IWin32API
	HRESULT __stdcall GetClassName(DWORD dwWnd, VARIANT *vRet);
	HRESULT __stdcall GetWindowText(DWORD dwWnd, VARIANT *vRet);
	HRESULT __stdcall SetWindowText(DWORD dwWnd, BSTR sTxt, VARIANT *vRet);
	HRESULT __stdcall GetAncestor(DWORD dwWnd, DWORD gaFlags, VARIANT *vRet);
	
	HRESULT __stdcall FindWindow(VARIANT ClassName, VARIANT WindowName, VARIANT *vRet);
	HRESULT __stdcall FindWindowEx(DWORD dwWndParent, DWORD dwWndChildAfter, VARIANT ClassName, VARIANT WindowName, VARIANT *vRet);

	HRESULT __stdcall Sleep(DWORD dwTime);
	HRESULT __stdcall GetTickCount(VARIANT *vRet);
	HRESULT __stdcall MapVirtualKey(DWORD dwCode, DWORD dwMapType, VARIANT *vRet);

	HRESULT __stdcall MessageBox(DWORD dwWnd, BSTR msg, BSTR ttl, VARIANT flags, VARIANT *vRet);
	HRESULT __stdcall MessageBeep(DWORD dwType, VARIANT *vRet);


	HRESULT __stdcall SendVScroll(DWORD dwWnd, DWORD order, DWORD thumb, DWORD dwWndScrollbar, VARIANT *vRet);
	HRESULT __stdcall SendHScroll(DWORD dwWnd, DWORD order, DWORD thumb, DWORD dwWndScrollbar, VARIANT *vRet);
	HRESULT __stdcall SendMouseWheel(DWORD dwWnd, DWORD dwFlags, LONG dy, VARIANT *vRet);
	HRESULT __stdcall SendMouseHWheel(DWORD dwWnd, DWORD dwFlags, LONG dx, VARIANT *vRet);
	HRESULT __stdcall SendWheelInput(LONG dx, LONG dy, VARIANT *vRet);
	
	HRESULT __stdcall ShowWindow(DWORD dwWnd, DWORD dwShowCmd, VARIANT *vRet);
	HRESULT __stdcall GetWindowState(DWORD dwWnd, VARIANT *vRet);	// GetWindowPlacement のサブセット

	HRESULT __stdcall GetAsyncKeyState(DWORD dwVK, VARIANT *vRet);
	HRESULT __stdcall SendKeybdInput(DWORD dwVk, VARIANT_BOOL bUp, VARIANT *vRet);
	
	HRESULT __stdcall WaitForInputIdle(DWORD dwWnd, DWORD dwTimeout, VARIANT *vRet);
	
	HRESULT __stdcall SendInput(VARIANT inputs, VARIANT *vRet);

	HRESULT __stdcall SetWindowPos(DWORD dwWnd, DWORD dwWndInsertAfter, LONG x, LONG y, LONG cx, LONG cy, DWORD dwFlags, VARIANT *vRet);
	HRESULT __stdcall GetWindowRect(DWORD dwWnd, VARIANT *vRet);
	HRESULT __stdcall GetCursorPos(VARIANT *vRet);
	HRESULT __stdcall SetCursorPos(LONG x, LONG y, VARIANT *vRet);
	
	HRESULT __stdcall GetVersionEx(VARIANT *vRet);

	HRESULT __stdcall SendCommand(DWORD dwWnd, DWORD wHi, DWORD wLo, DWORD hWndCtrl, VARIANT *vRet);
	HRESULT __stdcall SendSysCommand(DWORD dwWnd, DWORD dwID, DWORD lParam, VARIANT *vRet);
	HRESULT __stdcall SendMessage(DWORD dwWnd, DWORD dwMsg, DWORD wParam, DWORD lParam, VARIANT *vRet);
	HRESULT __stdcall PostMessage(DWORD dwWnd, DWORD dwMsg, DWORD wParam, DWORD lParam, VARIANT *vRet);


	HRESULT __stdcall GetWindowFilename(DWORD dwWnd, VARIANT *vRet);
	HRESULT __stdcall GetWindowFileVersion(DWORD dwWnd, VARIANT *vRet);
};
