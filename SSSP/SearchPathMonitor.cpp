#include "StdAfx.h"
#include "SearchPathMonitor.h"

CSearchPathMonitor::CSearchPathMonitor(void)
{
	m_hT = NULL;
	m_pClient = NULL;
	m_hNotifyWindow = NULL;
	m_uNotifyMessage = 0;
	::InitializeCriticalSection(&m_cs);
}

CSearchPathMonitor::~CSearchPathMonitor(void)
{
	::DeleteCriticalSection(&m_cs);
}

int CSearchPathMonitor::AddPath(LPCTSTR lpszPath)
{
	CString tmpPath(lpszPath);
	if (tmpPath.Right(1) != _T("\\")) {
		tmpPath.Append(_T("\\"));
	}
	if (m_Paths.Find(tmpPath) < 0) {
		m_Paths.Add(tmpPath);
	}
	return m_Paths.GetSize();
}

void CSearchPathMonitor::RemoveAll(void)
{
	m_Paths.RemoveAll();
}

void CSearchPathMonitor::Start(ISearchPathMonitorClient *pClient)
{
	if (m_hT == NULL) {
		m_pClient = pClient;
		UINT tid;
		HANDLE m_hT = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, &ThreadProc, this, 0, &tid));
		m_bStopping = FALSE;
	}
}

void CSearchPathMonitor::Start(HWND hWnd, UINT uMsg)
{
	if (m_hT == NULL) {
		m_hNotifyWindow = hWnd;
		m_uNotifyMessage = uMsg;
		UINT tid;
		m_hT = reinterpret_cast<HANDLE>(::_beginthreadex(NULL, 0, &ThreadProc, this, 0, &tid));
		m_bStopping = FALSE;
	}
}

void CSearchPathMonitor::Stop(void)
{
	if (m_hT != NULL) {
		m_bStopping = TRUE;
		::WaitForSingleObject(m_hT, INFINITE);
		::CloseHandle(m_hT);
		m_hT = NULL;
	}
}

UINT __stdcall CSearchPathMonitor::ThreadProc(LPVOID pData) {
	
	CSearchPathMonitor *obj = reinterpret_cast<CSearchPathMonitor *>(pData);
	return obj->ThreadMethod();
}

UINT CSearchPathMonitor::ThreadMethod(void) {
	int count;
	HANDLE *pHandles = NULL;
	CSimpleArray<CString> paths;
	ISearchPathMonitorClient *client;
	HWND wnd;
	UINT msg;
	{
		paths = m_Paths;
		CCritSecLock lock(m_cs);
		count = m_Paths.GetSize();
		pHandles = new HANDLE[count];
		for (int i = 0; i < count; i++) {
			pHandles[i] = ::FindFirstChangeNotification(m_Paths[i], FALSE, FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_LAST_WRITE);
		}
		client = m_pClient;
		wnd = m_hNotifyWindow;
		msg = m_uNotifyMessage;
	}
	
	DWORD dwLastChange = 0;
	while (!m_bStopping) {
		DWORD wr = ::WaitForMultipleObjects(count, pHandles, FALSE, 1000);
		if (wr >= WAIT_OBJECT_0 && wr < WAIT_OBJECT_0 + count) {
			int i = wr - WAIT_OBJECT_0;
			DWORD dwNow = ::GetTickCount();
			if (dwNow - dwLastChange > 2000) {	// 2•bˆÈ“à‚Ì•ÏX‚Í‚Ð‚Æ‚Â‚É‚Ü‚Æ‚ß‚é
				if (client != NULL) {
					client->SearchPathChanged(paths[i]);
				} else if (wnd != NULL) {
					::PostMessage(wnd, msg, dwNow - dwLastChange, reinterpret_cast<LPARAM>(static_cast<LPCTSTR>(paths[i])));
				}
			}
			dwLastChange = dwNow;
			::FindNextChangeNotification(pHandles[i]);
		} else if (wr >= WAIT_ABANDONED_0 && wr < WAIT_ABANDONED_0 + count) {
			int i = wr - WAIT_ABANDONED_0;
			pHandles[i] = NULL;
		}
	}
	return 0;
}