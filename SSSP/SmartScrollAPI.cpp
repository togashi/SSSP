// SmartScroll.cpp : CSmartScrollAPI ‚ÌŽÀ‘•

#include "stdafx.h"
#include "SmartScrollAPI.h"

#include "common.h"
#include "sspfunc.h"

#pragma comment(lib, "sspfunc.lib")

// CSmartScrollAPI

CSmartScrollAPI::CSmartScrollAPI():
	m_pHost(NULL)
{
	//SspTraceOut(__FUNCTION__);
}

CSmartScrollAPI::~CSmartScrollAPI()
{
	//SspTraceOut(__FUNCTION__);
}

HRESULT __stdcall CSmartScrollAPI::TraceOut(BSTR msg)
{
	CString s = CW2T(msg);
	CString ctx;
	if (m_pHost != NULL) {
		m_pHost->GetContextScriptName(ctx);
		if (!ctx.IsEmpty()) {
			ctx.Append(_T(": "));
			s.Insert(0, ctx);
		}
	}
	SspTraceOut(CT2A(s));
	return S_OK;
}

HRESULT __stdcall CSmartScrollAPI::LoadPluginResource(void)
{
	SspLoadPluginResource();
	return S_OK;
}

HRESULT __stdcall CSmartScrollAPI::GetFileVersion(BSTR filename, BSTR *ret)
{
	if (filename == NULL || ret == NULL) {
		return E_INVALIDARG;
	}
	CStringA aPath(filename);
	DWORD dwHi, dwLo;
	CComBSTR bstr;
	bstr.Attach(*ret);
	if (!SspGetFileVersion(const_cast<LPSTR>(static_cast<LPCSTR>(aPath)), &dwHi, &dwLo)) {
		CComBSTR bstr("");
		bstr.CopyTo(ret);
	} else {
		CString tmp;
		tmp.Format(_T("%d.%d.%d.%d"), HIWORD(dwHi), LOWORD(dwHi), HIWORD(dwLo), LOWORD(dwLo));
		CComBSTR bstr(tmp);
		bstr.CopyTo(ret);
	}
	bstr.Detach();
	return S_OK;
}

HRESULT __stdcall CSmartScrollAPI::ShowSmartScrollPad(VARIANT *vShow, VARIANT *vRet)
{
	HWND hWndPad = ::FindWindow(_T("SMARTSCROLL"), NULL);
	if (hWndPad != NULL) {
		enum OPTYPE { OP_UNKNOWN, OP_SHOW, OP_HIDE, OP_TOGGLE } type = OP_UNKNOWN;
		if (vShow->vt == VT_BOOL) {
			type = (vShow->boolVal == VARIANT_TRUE) ? OP_SHOW : OP_HIDE;
		} else if (vShow->vt == VT_I4) {
			type = (vShow->intVal < OP_SHOW || vShow->intVal > OP_TOGGLE) ? OP_UNKNOWN : static_cast<OPTYPE>(vShow->intVal);
		}
		BOOL res;
		if (type == OP_SHOW || type == OP_HIDE) {
			res = ::ShowWindow(hWndPad, (type == OP_SHOW) ? SW_SHOW : SW_HIDE);
		} else if (type == OP_TOGGLE) {
			res = ::ShowWindow(hWndPad, (::GetWindowLong(hWndPad, GWL_STYLE) & WS_VISIBLE) ? SW_HIDE : SW_SHOW);
		} else {
			res = FALSE;
		}
		vRet->vt = VT_BOOL;
		vRet->boolVal = res ? VARIANT_TRUE : VARIANT_FALSE;
	} else {
		vRet->vt = VT_BOOL;
		vRet->boolVal = VARIANT_FALSE;
	}
	return S_OK;
}
