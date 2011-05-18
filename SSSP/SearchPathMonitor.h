#pragma once

interface ISearchPathMonitorClient
{
public:
	virtual void SearchPathChanged(LPCTSTR lpszPath) = 0;
};

class CSearchPathMonitor
{
private:
	BOOL m_bStopping;
	CSimpleArray<CString> m_Paths;
	CRITICAL_SECTION m_cs;
	
	ISearchPathMonitorClient *m_pClient;
	HWND m_hNotifyWindow;
	UINT m_uNotifyMessage;
	
	HANDLE m_hT;
public:
	CSearchPathMonitor(void);
	virtual ~CSearchPathMonitor(void);

	int AddPath(LPCTSTR lpszPath);
	void RemoveAll(void);
	
	void Start(ISearchPathMonitorClient *pClient);
	void Start(HWND hWnd, UINT uMsg);
	void Stop(void);

	static UINT __stdcall ThreadProc(LPVOID pData);
	UINT ThreadMethod(void);
};