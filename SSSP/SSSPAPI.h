#pragma once

#include "buttonstatus.h"

#include "SSSP_i.h"
_COM_SMARTPTR_TYPEDEF(ISSSPAPI, IID_ISSSPAPI);

interface IScriptPluginContext
{
public:
	virtual BOOL SetTitle(LPCTSTR lpszText) = 0;
	virtual BOOL SetButtonParams(DWORD dwButton, DWORD dwDoubleClick, DWORD dwTripleClick, DWORD dwLongPress) = 0;
	virtual BOOL SetButtonText(DWORD dwButton, LPCTSTR lpszText) = 0;
	virtual BOOL SetButtonImage(DWORD dwButton, LPCTSTR lpszImageFilename) = 0;
	virtual BOOL SetWheelParams(DWORD dwThreshold, DWORD dwTimeout) = 0;
	virtual BOOL SetScrollParams(DWORD dwThreshold, DWORD dwTimeout) = 0;
	virtual BOOL EnableTimer(BOOL bEnabled) = 0;
	virtual BOOL EnableWheel(BOOL bEnabled) = 0;
	virtual BOOL SetWheelDefault(BOOL bDefault) = 0;
	virtual BOOL SetScrollDefault(BOOL bDefault) = 0;
	virtual HRESULT Import(LPCTSTR lpszFilename, VARIANT *vRet) = 0;
};

// CSSSPAPI
class __declspec(uuid("{7EE41386-367B-4342-8918-C7844F812CDD}")) ATL_NO_VTABLE CSSSPAPI :
	public CComObjectRoot,
	public CComCoClass<CSSSPAPI, &__uuidof(CSSSPAPI)>,
	public IDispatchImpl<ISSSPAPI, &IID_ISSSPAPI, &CAtlModule::m_libid, -1, -1>
{
public:
	CSSSPAPI(void);
	virtual ~CSSSPAPI(void);

BEGIN_COM_MAP(CSSSPAPI)
	COM_INTERFACE_ENTRY(ISSSPAPI)
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

public:
	IScriptPluginContext *m_pScriptPluginContext;
	
	// ISSSPAPI
	HRESULT __stdcall SetTitle(BSTR txt, VARIANT_BOOL *ret);
	HRESULT __stdcall SetButtonParams(DWORD b, DWORD dbl, DWORD tri, DWORD lng, VARIANT_BOOL *ret);
	HRESULT __stdcall SetButtonText(DWORD b, BSTR txt, VARIANT_BOOL *ret);
	HRESULT __stdcall SetButtonImage(DWORD b, BSTR fn, VARIANT_BOOL *ret);
	HRESULT __stdcall SetWheelParams(DWORD threshold, DWORD time, VARIANT_BOOL *ret);
	HRESULT __stdcall SetScrollParams(DWORD threshold, DWORD time, VARIANT_BOOL *ret);
	HRESULT __stdcall EnableTimer(VARIANT_BOOL bEnabled, VARIANT_BOOL *ret);
	HRESULT __stdcall EnableWheel(VARIANT_BOOL bEnabled, VARIANT_BOOL *ret);
	HRESULT __stdcall SetWheelDefault(VARIANT_BOOL bDefault, VARIANT_BOOL *ret);
	HRESULT __stdcall SetScrollDefault(VARIANT_BOOL bDefault, VARIANT_BOOL *ret);
	HRESULT __stdcall Import(BSTR filename, VARIANT *ret);
};
