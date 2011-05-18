#pragma once

extern HRESULT GetProperty(IDispatch *pDisp, LPCTSTR name, VARIANT *vProp);
extern HRESULT GetProperty(IDispatch *pDisp, LPCTSTR name, VARTYPE vt, VARIANT *vProp);
extern HRESULT PutProperty(IDispatch *pDisp, LPCTSTR name, VARIANT vProp);
extern HRESULT PutProperty(IDispatchEx *pDispEx, LPCTSTR name, VARIANT vProp);
