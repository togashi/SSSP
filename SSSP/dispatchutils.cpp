#include "stdafx.h"
#include "dispatchutils.h"

HRESULT GetProperty(IDispatch *pDisp, LPCTSTR name, VARIANT *vProp)
{
	HRESULT hr;
	LPOLESTR names[1];
	CComBSTR bstr(name);
	names[0] = bstr;
	DISPID did;
	hr = pDisp->GetIDsOfNames(IID_NULL, names, 1, LOCALE_USER_DEFAULT, &did);
	if (FAILED(hr)) {
		return hr;
	}
	
	UINT aErr;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	hr = pDisp->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &dp, vProp, NULL, &aErr);

	return hr;
}

HRESULT GetProperty(IDispatch *pDisp, LPCTSTR name, VARTYPE vt, VARIANT *vProp)
{
	HRESULT hr = GetProperty(pDisp, name, vProp);
	if (FAILED(hr)) {
		return hr;
	}
	if (vt != vProp->vt) {
		return DISP_E_TYPEMISMATCH;
	}
	return S_OK;
}

HRESULT PutProperty(IDispatch *pDisp, LPCTSTR name, VARIANT vProp)
{
	HRESULT hr;
	LPOLESTR names[1];
	CComBSTR bstr(name);
	names[0] = bstr;
	DISPID did;
	hr = pDisp->GetIDsOfNames(IID_NULL, names, 1, LOCALE_USER_DEFAULT, &did);
	if (FAILED(hr)) {
		return hr;
	}
	
	VARIANT vArgs[1] = { vProp };
	
	UINT aErr;
	VARIANT vRet;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID putid = DISPID_PROPERTYPUT;
	dp.cArgs = 1;
	dp.cNamedArgs = 1;
	dp.rgvarg = vArgs;
	dp.rgdispidNamedArgs = &putid;

	hr = pDisp->Invoke(did, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUTREF, &dp, &vRet, NULL, &aErr);

	return hr;
}

HRESULT PutProperty(IDispatchEx *pDispEx, LPCTSTR name, VARIANT vProp)
{
	HRESULT hr;
	CComBSTR bstr(name);
	DISPID did;
	hr = pDispEx->GetDispID(bstr, fdexNameEnsure | fdexNameCaseSensitive, &did);
	if (FAILED(hr)) {
		return hr;
	}
	
	VARIANT vArgs[1] = { vProp };
	
	VARIANT vRet;
	DISPPARAMS dp = { NULL, NULL, 0, 0 };
	DISPID putid = DISPID_PROPERTYPUT;
	dp.cArgs = 1;
	dp.cNamedArgs = 1;
	dp.rgvarg = vArgs;
	dp.rgdispidNamedArgs = &putid;

	hr = pDispEx->InvokeEx(did, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYPUTREF, &dp, &vRet, NULL, NULL);

	return hr;
}