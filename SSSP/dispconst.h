#pragma once

typedef struct DISP_CONST_MAP_ENTRY_ {
	LPOLESTR symbol;
	VARTYPE vt;
	DWORD dwData;
} DISP_CONST_MAP_ENTRY, *DISP_CONST_MAP;

#define BEGIN_CONST_MAP(s)		DISP_CONST_MAP_ENTRY s[] = {
#define DEF_CONST(s)			{ L#s, VT_UI4, (DWORD)(s) },
#define DEF_CONST_CUSTOM(s, v)	{ (s), VT_UI4, (DWORD)(v) },
#define END_CONST_MAP()			{ NULL, 0, 0 }};

class CDispatchConstProperties :
	public IDispatch
{
private:
	#ifdef _DEBUG
	LARGE_INTEGER m_liTime;
	DWORD m_dwCount;
	#endif
	DISP_CONST_MAP m_map;
	LONG m_count;
	ULONG m_ulRef;
public:
	// IUnknown
	HRESULT __stdcall QueryInterface(REFIID riid, void **ppvObject) {
		//return E_NOINTERFACE;
		if (IsEqualIID(riid, IID_IUnknown)) {
			*ppvObject = this;
			AddRef();
			return S_OK;
		} else if (IsEqualIID(riid, IID_IDispatch)) {
			*ppvObject = this;
			AddRef();
			return S_OK;
		}
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef(void) {
		
		//CString txt;
		//txt.Format(_T("%s \"%s\"  rc:%d"), _T(__FUNCTION__), m_map[0].symbol, m_ulRef + 1);
		//SspTraceOut(CT2A(txt));
		
		return ++m_ulRef;
	}

	ULONG __stdcall Release(void) {
		ULONG tmp = --m_ulRef;
		
		//CString txt;
		//txt.Format(_T("%s \"%s\"  rc:%d"), _T(__FUNCTION__), m_map[0].symbol, tmp);
		//SspTraceOut(CT2A(txt));
		
		if (tmp == 0) {
			#ifdef _DEBUG
			LARGE_INTEGER freq;
			::QueryPerformanceFrequency(&freq);
			#endif
			delete this;
		}
		return m_ulRef;
	}

	// IDispatch
	HRESULT __stdcall GetTypeInfoCount(UINT *pctinfo) {
		*pctinfo = 0;
		return S_OK;
	}

	HRESULT __stdcall GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo **ppTInfo)
	{
		*ppTInfo = NULL;
		return S_OK;
	}

	HRESULT __stdcall GetIDsOfNames(REFIID riid, LPOLESTR *rgszNames, UINT cNames, LCID lcid, DISPID *rgDispId)
	{
		#ifdef _DEBUG
		m_dwCount += cNames;
		LARGE_INTEGER liStart;
		::QueryPerformanceCounter(&liStart);
		#endif
		UINT found = 0;
		for (UINT i = 0; i < cNames; i++) {
			for (UINT j = 0; m_map[j].symbol != NULL; j++) {
				if (::wcscmp(m_map[j].symbol, rgszNames[i]) == 0) {
					rgDispId[i] = j + 1;
					found++;
					break;
				}
			}
		}
		#ifdef _DEBUG
		LARGE_INTEGER liEnd;
		::QueryPerformanceCounter(&liEnd);
		m_liTime.QuadPart = liEnd.QuadPart - liStart.QuadPart;
		#endif
		if (found != cNames) {
			return DISP_E_UNKNOWNNAME;
		}
		return S_OK;
	}

	HRESULT __stdcall Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS *pDispParams, VARIANT *pVarResult, EXCEPINFO *pExcepInfo, UINT *puArgErr)
	{
		if (dispIdMember == 0 || dispIdMember > m_count) {
			return DISP_E_MEMBERNOTFOUND;
		}
		pVarResult->vt = m_map[dispIdMember - 1].vt;
		if (pVarResult->vt == VT_BSTR) {
			pVarResult->bstrVal = reinterpret_cast<LPOLESTR>(m_map[dispIdMember - 1].dwData);
		} else {
			pVarResult->intVal = m_map[dispIdMember - 1].dwData;
		}
		return S_OK;
	}
public:
	CDispatchConstProperties(const DISP_CONST_MAP map) {
		#ifdef _DEBUG
		m_dwCount = 0;
		m_liTime.QuadPart = 0L;
		
#endif
		m_ulRef = 0;
		m_map = map;
		m_count = 0;
		while (m_map[m_count].symbol != NULL) m_count++;
	}

	~CDispatchConstProperties() {}
};