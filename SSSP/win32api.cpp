#include "StdAfx.h"
#include "win32api.h"

CWin32API::CWin32API(void):
	m_pDispScript(NULL)
{
	//SspTraceOut(__FUNCTION__);
}

CWin32API::~CWin32API(void)
{
	//SspTraceOut(__FUNCTION__);
}

HRESULT __stdcall CWin32API::GetClassName(DWORD dwWnd, VARIANT *vRet)
{
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	HRESULT hr = S_OK;
	CComVariant v;
	v.Attach(vRet);
	try {
		if (!::IsWindow(hWnd)) {
			throw S_OK;
		}

		CString str;
		int iRet = ::GetClassName(hWnd , str.GetBufferSetLength(MAX_PATH), MAX_PATH);
		str.ReleaseBuffer();
		if (iRet == 0) {
			throw E_FAIL;
		}
		v = str;
	}
	catch (HRESULT e) {
		v = _T("");
		hr = e;
	}
		
	v.Detach(vRet);
	return hr;
}

HRESULT __stdcall CWin32API::GetWindowText(DWORD dwWnd, VARIANT *vRet)
{
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	HRESULT hr = S_OK;
	CComVariant v;
	v.Attach(vRet);
	try {
		if (!::IsWindow(hWnd)) {
			throw S_OK;
		}

		CString str;
		int len = ::GetWindowTextLength(hWnd);
		if (len == 0) {
			v = _T("");
		} else {
			::GetWindowText(reinterpret_cast<HWND>(dwWnd), str.GetBufferSetLength(len), len + 1);
			str.ReleaseBuffer();
			v = str;
		}
	}
	catch (HRESULT e) {
		v = _T("");
		hr = e;
	}

	v.Detach(vRet);	
	return hr;
}

HRESULT __stdcall CWin32API::SetWindowText(DWORD dwWnd, BSTR sTxt, VARIANT *vRet)
{
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	HRESULT hr = S_OK;
	CComVariant v;
	v.Attach(vRet);
	try {
		if (!::IsWindow(hWnd)) {
			throw S_OK;
		}

		CString str;
		str = sTxt;
		::SetWindowText(hWnd, str);
		v = VARIANT_TRUE;
	}
	catch (HRESULT e) {
		v = VARIANT_FALSE;
		hr = e;
	}
	v.Detach(vRet);
	return hr;
}

HRESULT __stdcall CWin32API::GetAncestor(DWORD dwWnd, DWORD gaFlags, VARIANT *vRet)
{
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	HRESULT hr = S_OK;
	CComVariant v;
	v.Attach(vRet);
	try {
		if (!::IsWindow(hWnd)) {
			throw S_OK;
		}

		HWND hWndResult = ::GetAncestor(hWnd, gaFlags);
		v = reinterpret_cast<DWORD>(hWndResult);
	}
	catch (HRESULT e) {
		hr = e;
		v = 0;
	}
	v.Detach(vRet);
	return hr;
}

HRESULT __stdcall CWin32API::FindWindow(VARIANT ClassName, VARIANT WindowName, VARIANT *vRet)
{
	LPCTSTR lpszCN = NULL;
	LPCTSTR lpszWN = NULL;
	CString wsCN;
	CString wsWN;
	
	if (ClassName.vt == VT_BSTR) {
		wsCN = ClassName.bstrVal;
		lpszCN = wsCN.GetBuffer();
	}
	if (WindowName.vt == VT_BSTR) {
		wsWN = WindowName.bstrVal;
		lpszWN = wsWN.GetBuffer();
	}
	
	HWND hWnd = ::FindWindow(lpszCN, lpszWN);

	vRet->vt = VT_UI4;
	vRet->uiVal = reinterpret_cast<UINT>(hWnd);

	return S_OK;
}

HRESULT __stdcall CWin32API::FindWindowEx(DWORD dwWndParent, DWORD dwWndChildAfter, VARIANT ClassName, VARIANT WindowName, VARIANT *vRet)
{
	LPCTSTR lpszCN = NULL;
	LPCTSTR lpszWN = NULL;
	CString wsCN;
	CString wsWN;
	
	if (ClassName.vt == VT_BSTR) {
		wsCN = ClassName.bstrVal;
		lpszCN = wsCN.GetBuffer();
	}
	if (WindowName.vt == VT_BSTR) {
		wsWN = WindowName.bstrVal;
		lpszWN = wsWN.GetBuffer();
	}
	
	HWND hWnd = ::FindWindowEx(reinterpret_cast<HWND>(dwWndParent), reinterpret_cast<HWND>(dwWndChildAfter), lpszCN, lpszWN);

	vRet->vt = VT_UI4;
	vRet->intVal = reinterpret_cast<UINT>(hWnd);

	return S_OK;
}

HRESULT __stdcall CWin32API::Sleep(DWORD dwTime)
{
	::Sleep(dwTime);
	return S_OK;
}

HRESULT __stdcall CWin32API::GetTickCount(VARIANT *vRet)
{
	vRet->vt = VT_UI4;
	vRet->ulVal = ::GetTickCount();
	return S_OK;
}

HRESULT __stdcall CWin32API::MapVirtualKey(DWORD dwCode, DWORD dwMapType, VARIANT *vRet)
{
	vRet->vt = VT_UI4;
	vRet->intVal = ::MapVirtualKey(dwCode, dwMapType);
	return S_OK;
}

HRESULT __stdcall CWin32API::MessageBox(DWORD dwWnd, BSTR msg, BSTR ttl, VARIANT flags, VARIANT *vRet)
{
	CString strMsg(msg);
	CString strTtl(ttl);
	UINT uFlags = 0;
	if (flags.vt == VT_I4) {
		uFlags = flags.intVal;
	} else if (flags.vt == VT_DISPATCH) {
		IDispatchPtr pDisp = flags.pdispVal;
		CComVariant vProp;
		ULONG count = 0;
		if (SUCCEEDED(GetProperty(pDisp, _T("length"), VT_I4, &vProp))) {
			count = vProp.intVal;
		}
		CString key;
		for (ULONG i = 0; i < count; i++) {
			key.Format(_T("%d"), i);
			if (SUCCEEDED(GetProperty(pDisp, key, VT_BSTR, &vProp))) {
				CString fstr(vProp.bstrVal);
				#define MAPFLAG(s) if (fstr == _T(#s)) uFlags |= (s)
				MAPFLAG(MB_OK);
				MAPFLAG(MB_OKCANCEL);
				MAPFLAG(MB_ABORTRETRYIGNORE);
				MAPFLAG(MB_YESNOCANCEL);
				MAPFLAG(MB_YESNO);
				MAPFLAG(MB_RETRYCANCEL);
				MAPFLAG(MB_CANCELTRYCONTINUE);
				MAPFLAG(MB_ICONHAND);
				MAPFLAG(MB_ICONQUESTION);
				MAPFLAG(MB_ICONEXCLAMATION);
				MAPFLAG(MB_ICONASTERISK);
				MAPFLAG(MB_ICONWARNING);
				MAPFLAG(MB_ICONERROR);
				MAPFLAG(MB_ICONINFORMATION);
				MAPFLAG(MB_ICONSTOP);
				MAPFLAG(MB_DEFBUTTON1);
				MAPFLAG(MB_DEFBUTTON2);
				MAPFLAG(MB_DEFBUTTON3);
				MAPFLAG(MB_DEFBUTTON4);
				MAPFLAG(MB_APPLMODAL);
				MAPFLAG(MB_SYSTEMMODAL);
				MAPFLAG(MB_TASKMODAL);
				MAPFLAG(MB_NOFOCUS);
				MAPFLAG(MB_SETFOREGROUND);
				MAPFLAG(MB_DEFAULT_DESKTOP_ONLY);
				#undef MAPFLAG
			}
		}
	}
	vRet->vt = VT_I4;
	vRet->intVal = ::MessageBox(reinterpret_cast<HWND>(dwWnd), strMsg, strTtl, uFlags);
	return S_OK;
}

HRESULT __stdcall CWin32API::MessageBeep(DWORD dwType, VARIANT *vRet)
{
	vRet->vt = VT_BOOL;
	vRet->boolVal = ::MessageBeep(dwType);
	return S_OK;
}

HRESULT __stdcall CWin32API::SendVScroll(DWORD dwWnd, DWORD order, DWORD thumb, DWORD dwWndScrollbar, VARIANT *vRet)
{
	::PostMessage(reinterpret_cast<HWND>(dwWnd), WM_VSCROLL, MAKEWPARAM(order, thumb), dwWndScrollbar);
	vRet->vt = VT_BOOL;
	vRet->boolVal = VARIANT_TRUE;
	return S_OK;
}

HRESULT __stdcall CWin32API::SendHScroll(DWORD dwWnd, DWORD order, DWORD thumb, DWORD dwWndScrollbar, VARIANT *vRet)
{
	::PostMessage(reinterpret_cast<HWND>(dwWnd), WM_HSCROLL, MAKEWPARAM(order, thumb), dwWndScrollbar);
	vRet->vt = VT_BOOL;
	vRet->boolVal = VARIANT_TRUE;
	return S_OK;
}

HRESULT __stdcall CWin32API::SendMouseWheel(DWORD dwWnd, DWORD dwFlags, LONG dy, VARIANT *vRet)
{
	POINT pt;
	::GetCursorPos(&pt);
	::PostMessage(reinterpret_cast<HWND>(dwWnd), WM_MOUSEWHEEL, MAKEWPARAM(dwFlags, dy), MAKELPARAM(pt.x, pt.y));
	vRet->vt = VT_EMPTY;
	return S_OK;
}

HRESULT __stdcall CWin32API::SendMouseHWheel(DWORD dwWnd, DWORD dwFlags, LONG dx, VARIANT *vRet)
{
	POINT pt;
	::GetCursorPos(&pt);
	::PostMessage(reinterpret_cast<HWND>(dwWnd), WM_MOUSEHWHEEL, MAKEWPARAM(dwFlags, dx), MAKELPARAM(pt.x, pt.y));
	vRet->vt = VT_EMPTY;
	return S_OK;
}

HRESULT __stdcall CWin32API::SendWheelInput(LONG dx, LONG dy, VARIANT *vRet)
{
	INPUT input[2] = {};
	int i = 0;
	
	if (dx != 0) {
		input[i].type = INPUT_MOUSE;
		input[i].mi.mouseData = static_cast<DWORD>(dx);
		input[i].mi.dwFlags = MOUSEEVENTF_HWHEEL | MOUSEEVENTF_WHEEL;
		i++;
	}
	if (dy != 0) {
		input[i].type = INPUT_MOUSE;
		input[i].mi.mouseData = static_cast<DWORD>(dy);
		input[i].mi.dwFlags = MOUSEEVENTF_WHEEL;
		i++;
	}
	
	vRet->vt = VT_UI4;
	if (i > 0) {
		vRet->intVal = ::SendInput(i, input, sizeof(INPUT));
	} else {
		vRet->vt = VT_UI4;
		vRet->intVal = 0;
	}
	
	return S_OK;
}

HRESULT __stdcall CWin32API::ShowWindow(DWORD dwWnd, DWORD dwShowCmd, VARIANT *vRet)
{
	vRet->vt = VT_BOOL;
	vRet->boolVal = ::ShowWindow(reinterpret_cast<HWND>(dwWnd), dwShowCmd);
	return S_OK;
}

HRESULT __stdcall CWin32API::GetWindowState(DWORD dwWnd, VARIANT *vRet)
{
	WINDOWPLACEMENT wp = { sizeof(wp) };
	if (::GetWindowPlacement(reinterpret_cast<HWND>(dwWnd), &wp)) {
		vRet->vt = VT_I4;
		vRet->intVal = wp.showCmd;
	} else {
		vRet->vt = VT_BOOL;
		vRet->boolVal = VARIANT_FALSE;
	}
	return S_OK;
}

HRESULT __stdcall CWin32API::GetAsyncKeyState(DWORD dwVK, VARIANT *vRet)
{
	vRet->vt = VT_BOOL;
	vRet->boolVal = (::GetAsyncKeyState(dwVK) < 0) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

HRESULT __stdcall CWin32API::SendKeybdInput(DWORD dwVk, VARIANT_BOOL bUp, VARIANT *vRet)
{
	INPUT input[1] = {};
	
	input[0].type = INPUT_KEYBOARD;
	input[0].ki.wVk = static_cast<WORD>(dwVk);
	input[0].ki.dwFlags = (bUp == VARIANT_TRUE) ? KEYEVENTF_KEYUP : 0;
	input[0].ki.wScan = ::MapVirtualKey(dwVk, 3);
	input[0].ki.dwExtraInfo = ::GetMessageExtraInfo();
	
	vRet->vt = VT_UI4;
	vRet->intVal = ::SendInput(1, input, sizeof(INPUT));
	
	return S_OK;
}

HRESULT __stdcall CWin32API::WaitForInputIdle(DWORD dwWnd, DWORD dwTimeout, VARIANT *vRet)
{
	DWORD pid;
	::GetWindowThreadProcessId(reinterpret_cast<HWND>(dwWnd), &pid);
	HANDLE hP = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (hP != NULL) {
		vRet->vt = VT_UI4;
		vRet->intVal = ::WaitForInputIdle(hP, dwTimeout);
		::CloseHandle(hP);
	} else {
		vRet->vt = VT_BOOL;
		vRet->boolVal = VARIANT_FALSE;
	}
	return S_OK;
}


HRESULT __stdcall CWin32API::SendInput(VARIANT vInputs, VARIANT *vRet)
{
	vRet->vt = VT_I4;
	try {
		if (vInputs.vt != VT_DISPATCH) {
			throw 0;
		}
		
		// 要素数取得
		IDispatchPtr pDisp = vInputs.pdispVal;
		CComVariant vProp;
		if (FAILED(GetProperty(pDisp, _T("length"), VT_I4, &vProp))) {
			throw 0;
		}
		UINT nCount = vProp.intVal;
		
		// INPUT配列生成
		INPUT *inputs = new INPUT[nCount];
		try {
			CString key;
			for (UINT i = 0; i < nCount; i++) {
				key.Format(_T("%d"), i);
				::ZeroMemory(&inputs[i], sizeof(INPUT));
				if (FAILED(GetProperty(pDisp, key, VT_DISPATCH, &vProp))) {
					throw 0;
				}
				IDispatchPtr pInput = vProp.pdispVal;
				if (SUCCEEDED(GetProperty(pInput, _T("type"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("0"), VT_I4, &vProp))) {
					inputs[i].type = vProp.intVal;
				} else {
					throw 0;
				}
				DWORD type = vProp.intVal;
				if (type == INPUT_MOUSE) {
					if (SUCCEEDED(GetProperty(pInput, _T("dx"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("1"), VT_I4, &vProp))) {
						inputs[i].mi.dx = vProp.intVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("dy"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("2"), VT_I4, &vProp))) {
						inputs[i].mi.dy = vProp.intVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("mouseData"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("3"), VT_I4, &vProp))) {
						inputs[i].mi.mouseData = vProp.intVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("dwFlags"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("4"), VT_I4, &vProp))) {
						inputs[i].mi.dwFlags = vProp.intVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("time"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("5"), VT_I4, &vProp))) {
						inputs[i].mi.time = vProp.intVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("dwExtraInfo"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("6"), VT_I4, &vProp))) {
						inputs[i].mi.time = vProp.intVal;
					}
				} else if (type == INPUT_KEYBOARD) {
					if (SUCCEEDED(GetProperty(pInput, _T("wVk"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("1"), VT_I4, &vProp))) {
						inputs[i].ki.wVk = vProp.uiVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("wScan"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("2"), VT_I4, &vProp))) {
						inputs[i].ki.wScan = vProp.uiVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("dwFlags"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("3"), VT_I4, &vProp))) {
						inputs[i].ki.dwFlags = vProp.intVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("time"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("4"), VT_I4, &vProp))) {
						inputs[i].ki.time = vProp.intVal;
					}
					if (SUCCEEDED(GetProperty(pInput, _T("dwExtraInfo"), VT_I4, &vProp)) || SUCCEEDED(GetProperty(pInput, _T("5"), VT_I4, &vProp))) {
						inputs[i].ki.time = vProp.intVal;
					}
				} else if (type == INPUT_HARDWARE) {
					throw 0;
				} else {
					throw 0;
				}
			}
			vRet->intVal = ::SendInput(nCount, inputs, sizeof(INPUT));
		}
		catch (...) {
			delete [] inputs;
			throw;
		}
		delete [] inputs;
	}
	catch (...) {
		vRet->intVal = 0;
	}
	return S_OK;
}

HRESULT __stdcall CWin32API::SetWindowPos(DWORD dwWnd, DWORD dwWndInsertAfter, LONG x, LONG y, LONG cx, LONG cy, DWORD dwFlags, VARIANT *vRet)
{
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	HRESULT hr = S_OK;
	CComVariant v;
	v.Attach(vRet);
	try {
		if (!::IsWindow(hWnd)) {
			throw S_OK;
		}
		v = ::SetWindowPos(hWnd, reinterpret_cast<HWND>(dwWndInsertAfter), x, y, cx, cy, dwFlags) ? VARIANT_TRUE : VARIANT_FALSE;
	}
	catch (HRESULT e) {
		v = VARIANT_FALSE;
		hr = e;
	}
	v.Detach(vRet);
	return hr;
}

HRESULT __stdcall CWin32API::GetWindowRect(DWORD dwWnd, VARIANT *vRet)
{
	HRESULT hr;
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	try {
		if (!::IsWindow(hWnd)) {
			return S_OK;
		}
	
		RECT rct;
		BOOL br = ::GetWindowRect(hWnd, &rct);
		if (!br) {
			throw S_OK;
		}
		
		if (m_pDispScript == NULL) {
			return E_FAIL;
		}
		
		VARIANT vObject;
		{	// Object メソッドをコール
			DISPID did;
			{
				LPOLESTR names[1] = { L"Object" };
				hr = m_pDispScript->GetIDsOfNames(IID_NULL, names, 1, LOCALE_USER_DEFAULT, &did);
			}
			if (FAILED(hr)) {
				return E_FAIL;
			}
			UINT aErr;
			DISPPARAMS dp = { NULL, NULL, 0, 0 };
			hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vObject, NULL, &aErr);
			if (FAILED(hr)) {
				return E_FAIL;
			}
		}
		
		{
			IDispatchExPtr pDispEx = vObject.pdispVal;
			if (pDispEx == NULL) {
				return E_FAIL;
			}
			CComVariant tmpVar;
			tmpVar = rct.left;
			hr = PutProperty(pDispEx, L"left", tmpVar);
			if (FAILED(hr)) {
				return hr;
			}
			tmpVar = rct.top;
			hr = PutProperty(pDispEx, L"top", tmpVar);
			if (FAILED(hr)) {
				return hr;
			}
			tmpVar = rct.right;
			hr = PutProperty(pDispEx, L"right", tmpVar);
			if (FAILED(hr)) {
				return hr;
			}
			tmpVar = rct.bottom;
			hr = PutProperty(pDispEx, L"bottom", tmpVar);
			if (FAILED(hr)) {
				return hr;
			}
		}
	
		vRet->vt = VT_DISPATCH;
		vRet->pdispVal = vObject.pdispVal;
	}
	catch (HRESULT e) {
		hr = e;
		if (vRet->vt == VT_DISPATCH) {
			vRet->pdispVal->Release();
		}
		vRet->vt = VT_BOOL;
		vRet->boolVal = VARIANT_FALSE;
	}
	return hr;
}

HRESULT __stdcall CWin32API::GetCursorPos(VARIANT *vRet)
{
	POINT pt;
	BOOL br = ::GetCursorPos(&pt);
	if (!br) {
		vRet->vt = VT_BOOL;
		vRet->boolVal = VARIANT_FALSE;
		return S_OK;
	}
	
	if (m_pDispScript == NULL) {
		return E_FAIL;
	}
	
	HRESULT hr;
	
	VARIANT vObject;
	{	// Object メソッドをコール
		DISPID did;
		{
			LPOLESTR names[1] = { L"Object" };
			hr = m_pDispScript->GetIDsOfNames(IID_NULL, names, 1, LOCALE_USER_DEFAULT, &did);
		}
		if (FAILED(hr)) {
			return E_FAIL;
		}
		UINT aErr;
		DISPPARAMS dp = { NULL, NULL, 0, 0 };
		hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vObject, NULL, &aErr);
		if (FAILED(hr)) {
			return E_FAIL;
		}
	}
	
	{
		IDispatchExPtr pDispEx = vObject.pdispVal;
		if (pDispEx == NULL) {
			return E_FAIL;
		}
		CComVariant tmpVar;
		hr = S_OK;
		tmpVar = pt.x;
		hr = PutProperty(pDispEx, L"x", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = pt.y;
		hr = PutProperty(pDispEx, L"y", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
	}
	
	vRet->vt = VT_DISPATCH;
	vRet->pdispVal = vObject.pdispVal;
	return hr;
}

HRESULT __stdcall CWin32API::SetCursorPos(LONG x, LONG y, VARIANT *vRet)
{
	CComVariant v;
	v.Attach(vRet);
	v = ::SetCursorPos(x, y);
	v.Detach(vRet);
	return S_OK;
}

HRESULT __stdcall CWin32API::GetVersionEx(VARIANT *vRet)
{
	OSVERSIONINFOEX osvix = { sizeof(OSVERSIONINFOEX) };
	BOOL br = ::GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&osvix));
	if (!br) {
		osvix.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		br = ::GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&osvix));
	}
	if (!br) {
		vRet->vt = VT_EMPTY;
		return S_OK;
	}
	
	if (m_pDispScript == NULL) {
		return E_FAIL;
	}

	HRESULT hr;
	
	VARIANT vObject;
	{	// Object メソッドをコール
		DISPID did;
		{
			LPOLESTR names[1] = { L"Object" };
			hr = m_pDispScript->GetIDsOfNames(IID_NULL, names, 1, LOCALE_USER_DEFAULT, &did);
		}
		if (FAILED(hr)) {
			return E_FAIL;
		}
		UINT aErr;
		DISPPARAMS dp = { NULL, NULL, 0, 0 };
		hr = m_pDispScript->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_METHOD, &dp, &vObject, NULL, &aErr);
		if (FAILED(hr)) {
			return E_FAIL;
		}
	}
	
	{
		IDispatchExPtr pDispEx = vObject.pdispVal;
		if (pDispEx == NULL) {
			return E_FAIL;
		}
		CComVariant tmpVar;
		hr = S_OK;
		tmpVar = osvix.dwMajorVersion;
		hr = PutProperty(pDispEx, L"dwMajorVersion", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.dwMinorVersion;
		hr = PutProperty(pDispEx, L"dwMinorVersion", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.dwBuildNumber;
		hr = PutProperty(pDispEx, L"dwBuildNumber", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.dwPlatformId;
		hr = PutProperty(pDispEx, L"dwPlatformId", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.szCSDVersion;
		hr = PutProperty(pDispEx, L"szCSDVersion", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.wServicePackMajor;
		hr = PutProperty(pDispEx, L"wServicePackMajor", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.wServicePackMinor;
		hr = PutProperty(pDispEx, L"wServicePackMinor", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.wSuiteMask;
		hr = PutProperty(pDispEx, L"wSuiteMask", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
		tmpVar = osvix.wProductType;
		hr = PutProperty(pDispEx, L"wProductType", tmpVar);
		if (FAILED(hr)) {
			return hr;
		}
	}
	
	vRet->vt = VT_DISPATCH;
	vRet->pdispVal = vObject.pdispVal;
	return hr;
}

HRESULT __stdcall CWin32API::SendCommand(DWORD dwWnd, DWORD wHi, DWORD wLo, DWORD dwWndCtrl, VARIANT *vRet)
{
	vRet->vt = VT_BOOL;
	vRet->boolVal = ::PostMessage(reinterpret_cast<HWND>(dwWnd), WM_COMMAND, MAKEWPARAM(wLo, wHi), dwWndCtrl) ? VARIANT_TRUE : VARIANT_FALSE;
	
	return S_OK;
}

HRESULT __stdcall CWin32API::SendSysCommand(DWORD dwWnd, DWORD dwID, DWORD lParam, VARIANT *vRet)
{
	vRet->vt = VT_BOOL;
	vRet->boolVal = ::PostMessage(reinterpret_cast<HWND>(dwWnd), WM_SYSCOMMAND, dwID, lParam) ? VARIANT_TRUE : VARIANT_FALSE;

	return S_OK;
}

HRESULT __stdcall CWin32API::SendMessage(DWORD dwWnd, DWORD dwMsg, DWORD wParam, DWORD lParam, VARIANT *vRet)
{
	vRet->vt = VT_UI4;
	vRet->intVal = ::SendMessage(reinterpret_cast<HWND>(dwWnd), dwMsg, wParam, lParam);
	
	return S_OK;
}

HRESULT __stdcall CWin32API::PostMessage(DWORD dwWnd, DWORD dwMsg, DWORD wParam, DWORD lParam, VARIANT *vRet)
{
	vRet->vt = VT_UI4;
	vRet->intVal = ::PostMessage(reinterpret_cast<HWND>(dwWnd), dwMsg, wParam, lParam);
	
	return S_OK;
}

HRESULT __stdcall CWin32API::GetWindowFilename(DWORD dwWnd, VARIANT *vRet)
{
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	HRESULT hr = S_OK;
	CComVariant v;
	v.Attach(vRet);
	try {
		if (!::IsWindow(hWnd)) {
			throw S_OK;
		}
		DWORD pid;
		::GetWindowThreadProcessId(hWnd, &pid);
		if (pid == 0) {
			throw S_OK;
		}
		HANDLE hProc = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (hProc == NULL) {
			throw S_OK;
		}
		DWORD dwSize = MAX_PATH + 1;
		CString Path;
		BOOL bOK = QueryFullProcessImageName(hProc, 0, Path.GetBufferSetLength(MAX_PATH), &dwSize);
		Path.ReleaseBuffer();
		if (bOK) {
			v = Path;
		} else {
			v = _T("");
		}
		::CloseHandle(hProc);
	}
	catch (HRESULT e) {
		v = _T("");
		hr = e;
	}
	v.Detach(vRet);
	return hr;
}

HRESULT __stdcall CWin32API::GetWindowFileVersion(DWORD dwWnd, VARIANT *vRet)
{
	HWND hWnd = reinterpret_cast<HWND>(dwWnd);
	HRESULT hr = S_OK;
	CComVariant v;
	v.Attach(vRet);
	try {
		if (!::IsWindow(hWnd)) {
			throw S_OK;
		}

		CStringA aPath;
		CComVariant varFilename;
		GetWindowFilename(dwWnd, &varFilename);
		aPath = varFilename;
		
		DWORD dwHi, dwLo;
		if (SspGetFileVersion(const_cast<LPSTR>(static_cast<LPCSTR>(aPath)), &dwHi, &dwLo)) {
			CString tmp;
			tmp.Format(_T("%d.%d.%d.%d"), HIWORD(dwHi), LOWORD(dwHi), HIWORD(dwLo), LOWORD(dwLo));
			v = tmp;
		} else {	
			v = _T("");
		}
	}
	catch (HRESULT e) {
		v = _T("");
		hr = e;
	}
	
	v.Detach(vRet);
	return hr;
}
