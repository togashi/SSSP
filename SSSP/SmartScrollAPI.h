// SmartScroll.h : CSmartScrollAPI �̐錾

#pragma once

#include "SSSP_i.h"
_COM_SMARTPTR_TYPEDEF(ISmartScrollAPI, IID_ISmartScrollAPI);

interface ISmartScrollAPIHost
{
public:
	virtual BOOL GetContextScriptName(CString &str) = 0;
};

// CSmartScrollAPI
class __declspec(uuid("{55C6F185-07A9-4a86-A20E-E830E7E64C53}")) ATL_NO_VTABLE CSmartScrollAPI :
	public CComObjectRoot,
	public CComCoClass<CSmartScrollAPI, &__uuidof(CSmartScrollAPI)>,
	public IDispatchImpl<ISmartScrollAPI, &IID_ISmartScrollAPI, &CAtlModule::m_libid, -1, -1>
{
public:
	CSmartScrollAPI();
	virtual ~CSmartScrollAPI();

BEGIN_COM_MAP(CSmartScrollAPI)
	COM_INTERFACE_ENTRY(ISmartScrollAPI)
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
	
	ISmartScrollAPIHost *m_pHost;
public:
	virtual HRESULT __stdcall TraceOut(BSTR msg);
	virtual HRESULT __stdcall LoadPluginResource(void);
	virtual HRESULT __stdcall GetFileVersion(BSTR filename, BSTR *ret);
	virtual HRESULT __stdcall ShowSmartScrollPad(VARIANT *vShow, VARIANT *vRet);
};
