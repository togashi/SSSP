#include "StdAfx.h"
#include "SSSPAPI.h"

CSSSPAPI::CSSSPAPI(void):
	m_pScriptPluginContext(NULL)
{
	//SspTraceOut(__FUNCTION__);
}

CSSSPAPI::~CSSSPAPI(void)
{
	//SspTraceOut(__FUNCTION__);
}

HRESULT __stdcall CSSSPAPI::SetTitle(BSTR txt, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	CString str = txt;
	*ret = m_pScriptPluginContext->SetTitle(txt) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::SetButtonParams(DWORD b, DWORD dbl, DWORD tri, DWORD lng, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	*ret = m_pScriptPluginContext->SetButtonParams(b, dbl, tri, lng) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::SetButtonText(DWORD b, BSTR txt, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	CString str = txt;
	*ret = m_pScriptPluginContext->SetButtonText(b, str) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::SetButtonImage(DWORD b, BSTR fn, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	CString str = fn;
	*ret = m_pScriptPluginContext->SetButtonImage(b, str) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::SetWheelParams(DWORD threshold, DWORD time, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	*ret = m_pScriptPluginContext->SetWheelParams(threshold, time) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::SetScrollParams(DWORD threshold, DWORD time, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	*ret = m_pScriptPluginContext->SetScrollParams(threshold, time) ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::EnableTimer(VARIANT_BOOL bEnabled, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	*ret = m_pScriptPluginContext->EnableTimer(bEnabled);
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::EnableWheel(VARIANT_BOOL bEnabled, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	*ret = m_pScriptPluginContext->EnableWheel(bEnabled);
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::SetWheelDefault(VARIANT_BOOL bDefault, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	*ret = m_pScriptPluginContext->SetWheelDefault(bDefault);
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::SetScrollDefault(VARIANT_BOOL bDefault, VARIANT_BOOL *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	*ret = m_pScriptPluginContext->SetScrollDefault(bDefault);
	return S_OK;
}

HRESULT __stdcall CSSSPAPI::Import(BSTR filename, VARIANT *ret)
{
	if (m_pScriptPluginContext == NULL) {
		return E_FAIL;
	}
	CString str = filename;
	return m_pScriptPluginContext->Import(str, ret);
}
