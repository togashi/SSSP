#include "StdAfx.h"
#include "ButtonStatus.h"

#ifdef USE_PROXYOBJ
void TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	CButtonStatus *bs = reinterpret_cast<CButtonStatus *>(idEvent);
}
#endif

CButtonStatus::CButtonStatus(void):
	m_Client(this),
	m_Index(BUTTON_STATE_HISTORY_COUNT - 1)
{
	//SspTraceOut(__FUNCTION__);
	#ifdef USE_MSGWND
	Create(HWND_MESSAGE);
	#endif
	for (int i = 0; i < BUTTON_STATE_BUTTON_COUNT; i++) {
		m_Settings[i].dwDoubleClick = 0;
		m_Settings[i].dwTripleClick = 0;
		m_Settings[i].dwLongPress = 0;
		
		m_DownTimerIds[i] = 0;
		m_PressTimerIds[i] = 0;
		#ifdef USE_PROXYOBJ
		m_DownTimerProcs[i] = CreateTimerProcObject(MAKELONG(i, 0));
		m_PressTimerProcs[i] = CreateTimerProcObject(MAKELONG(i, 1));
		#endif
	}
	Reset();
}

CButtonStatus::~CButtonStatus(void)
{
	//SspTraceOut(__FUNCTION__);
	for (int i = 0; i < BUTTON_STATE_BUTTON_COUNT; i++) {
		#ifdef USR_MSGWND
		KillTimer(MAKELONG(i, 0));
		KillTimer(MAKELONG(i, 1));
		#endif
		#ifdef USR_PROXYOBJ
		::KillTimer(NULL, m_DownTimerIds[i]);
		DeleteTimerProcObject(m_DownTimerProcs[i]);
		::KillTimer(NULL, m_PressTimerIds[i]);
		DeleteTimerProcObject(m_PressTimerProcs[i]);
		#endif
	}
	#ifdef USE_MSGWND
	DestroyWindow();
	#endif
}

#ifdef USE_PROXYOBJ
TIMERPROC CButtonStatus::CreateTimerProcObject(DWORD arg)
{
	ITimerEvent *pI = this;
	LPVOID pO = pI;
	LPVOID lpVT = *static_cast<LPVOID *>(pO);
	DWORD dwMethodAddress = *static_cast<LPDWORD>(lpVT);
	
	#pragma pack(push,1)
	struct Template_ {
		BYTE Push2;
		DWORD Arg;
		BYTE Push1;
		DWORD This;
		BYTE Mov[1];
		DWORD Addr;
		BYTE Call[2];
		BYTE End[4];
	} Image = {
		0x68,
		arg,
		0x68,
		reinterpret_cast<DWORD>(pI),
		{ 0XBB },
		dwMethodAddress,
		{ 0xFF, 0xD3 },
		{ 0x83, 0xC4, 0x08, 0xC3 }
	};
	#pragma pack(pop)
	LPVOID obj = VirtualAlloc(NULL, (sizeof(Image) + 15) / 16 * 16, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	::CopyMemory(obj, &Image, sizeof(Image));
	return static_cast<TIMERPROC>(obj);
}

void CButtonStatus::DeleteTimerProcObject(TIMERPROC obj)
{
	::VirtualFree(static_cast<LPVOID>(obj), 0, MEM_DECOMMIT | MEM_RELEASE);
}
#endif

void CButtonStatus::Reset(void)
{
	m_Index = BUTTON_STATE_HISTORY_COUNT - 1;
	for (int i = 0; i < BUTTON_STATE_BUTTON_COUNT; i++) {
		#ifdef USE_PROXYOBJ
		::KillTimer(NULL, m_DownTimerIds[i]);
		::KillTimer(NULL, m_PressTimerIds[i]);
		#endif
		#ifdef USE_MSGWND
		KillTimer(MAKELONG(i, 0));
		KillTimer(MAKELONG(i, 1));
		#endif		
		m_DownTimerIds[i] = 0;
		m_PressTimerIds[i] = 0;
	}
	for (int i = 0; i < BUTTON_STATE_HISTORY_COUNT; i++) {
		::_tcscpy_s(m_States[i].States, BUTTON_STATE_BUTTON_COUNT + 1, _T("uuuuuuuuu"));
		::_tcscpy_s(m_States[i].Flags, BUTTON_STATE_BUTTON_COUNT + 1, _T("         "));
		m_States[i].dwTickCount = 0;
	}
}

void CButtonStatus::SetClient(IButtonEvent *client)
{
	if (client == NULL) {
		m_Client = this;
	} else {
		m_Client = client;
	}
}

void CButtonStatus::ButtonProc(DWORD dwType)
{
	int hil = m_Index % BUTTON_STATE_HISTORY_COUNT;
	int hi = ++m_Index % BUTTON_STATE_HISTORY_COUNT;
	::_tcscpy_s(m_States[hi].States, BUTTON_STATE_BUTTON_COUNT + 1, m_States[hil].States);
	::_tcslwr_s(m_States[hi].States, BUTTON_STATE_BUTTON_COUNT + 1);
	::_tcscpy_s(m_States[hi].Flags, BUTTON_STATE_BUTTON_COUNT + 1, _T("         "));
	int bi = dwType >> 4; 
	BOOL bDown = ((dwType & 0x0F) == 0);
	m_States[hi].States[bi] = bDown ? BS_DOWN : BS_UP;
	m_States[hi].dwTickCount = ::GetTickCount();
	DWORD dwMaxTime = max(m_Settings[bi].dwDoubleClick, m_Settings[bi].dwTripleClick);
	if (bDown) {
		if (dwMaxTime > 0 && m_DownTimerIds[bi] == 0) {
			m_DownStartTimes[bi] = m_States[hi].dwTickCount;
			#ifdef USE_PROXYOBJ
			m_DownTimerIds[bi] = ::SetTimer(NULL, 0, dwMaxTime, m_DownTimerProcs[bi]);
			#endif
			#ifdef USE_MSGWND
			m_DownTimerIds[bi] = SetTimer(MAKELONG(bi, 0), dwMaxTime);
			#endif
		}
		if (m_Settings[bi].dwLongPress > 0) {
			m_PressStartTimes[bi] = m_States[hi].dwTickCount;
			#ifdef USE_PROXYOBJ
			m_PressTimerIds[bi] = ::SetTimer(NULL, 0, m_Settings[bi].dwLongPress, m_PressTimerProcs[bi]);
			#endif
			#ifdef USE_MSGWND
			m_PressTimerIds[bi] = SetTimer(MAKELONG(bi, 1), m_Settings[bi].dwLongPress);
			#endif
		}
	} else {
		BOOL bLong = FALSE;
		if (m_Settings[bi].dwLongPress > 0) {
			for (int i = 0; i < BUTTON_STATE_HISTORY_COUNT; i++) {
				int ei = (m_Index - i + BUTTON_STATE_HISTORY_COUNT) % BUTTON_STATE_HISTORY_COUNT;
				if (m_States[ei].States[bi] == BS_DOWN) {
					DWORD dwTime = m_States[hi].dwTickCount - m_States[ei].dwTickCount;
					if (dwTime >= m_Settings[bi].dwLongPress) {
						m_Client->OnReleaseLongPress(bi, dwTime);
						bLong = TRUE;
					}
					break;
				}
			}
			#ifdef USE_PROXYOBJ
			::KillTimer(NULL, m_PressTimerIds[bi]);
			#endif
			#ifdef USE_MSGWND
			KillTimer(MAKELONG(bi, 1));
			#endif
		}
		if (!bLong) {
			DWORD dwMaxTime = max(m_Settings[bi].dwDoubleClick, m_Settings[bi].dwTripleClick);
			if (dwMaxTime == 0) {
				m_Client->OnClicked(bi);
			}
		}
	}
}

LRESULT CButtonStatus::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	bHandled = TRUE;
	TimerMethod(wParam);
	return ERROR_SUCCESS;
}

void __cdecl CButtonStatus::TimerMethod(DWORD arg)
{
	int si = HIWORD(arg);	
	int bi = LOWORD(arg);
	
	DWORD dwNow = ::GetTickCount();
	LONGLONG newest = m_Index;
	
	if (si == 0) {
		#ifdef USE_PROXYOBJ
		::KillTimer(NULL, m_DownTimerIds[bi]);
		#endif
		#ifdef USE_MSGWND
		KillTimer(MAKELONG(bi, 0));
		#endif
		m_DownTimerIds[bi] = 0;
		DWORD DCount = 0;
		DWORD UCount = 0;
		DWORD dwMaxTime = max(m_Settings[bi].dwDoubleClick, m_Settings[bi].dwTripleClick);
		int hi = (newest + BUTTON_STATE_HISTORY_COUNT) % BUTTON_STATE_HISTORY_COUNT;
		for (int i = 0; i < BUTTON_STATE_HISTORY_COUNT; i++) {
			int ei = (newest - i + BUTTON_STATE_HISTORY_COUNT) % BUTTON_STATE_HISTORY_COUNT;
			if (m_States[ei].Flags[bi] != BF_SUSPENDED) {
				break;
			}
			if (m_States[ei].dwTickCount < m_DownStartTimes[bi]) {
				break;
			}
			DWORD dwTime = m_States[ei].dwTickCount - m_DownStartTimes[bi];
			if (m_States[ei].States[bi] == BS_DOWN) {
				DCount++;
				if (DCount == 2 && dwTime <= m_Settings[bi].dwDoubleClick) {
					// ダブルクリック認識
					m_States[ei].Flags[bi] = BF_DOUBLECLICK;
				}
				if (DCount == 3 && dwTime <= m_Settings[bi].dwTripleClick) {
					// トリプルクリック認識
					m_States[ei].Flags[bi] = BF_TRIPLECLICK;
					break;
				}
			} else if (m_States[ei].States[bi] == BS_UP) {
				UCount++;
				m_States[ei].Flags[bi] = BF_CLICK;
			}
		}
		if (DCount == 1 && (m_States[hi].States[bi] == BS_UP || m_States[hi].States[bi] == BS_UPPED)) {
			// 現在はUPだが、DOWNが1回あった
			m_Client->OnClicked(bi);
		} else if (DCount == 2) {
			// DOWNが2回あった
			
			// ダブルクリック確定で長押しの検知はキャンセル
			#ifdef USE_PROXYOBJ
			::KillTimer(NULL, m_PressTimerIds[bi]);
			#endif
			#ifdef USE_MSGWND
			KillTimer(MAKELONG(bi, 1));
			#endif
			m_PressTimerIds[bi] = 0;
			
			m_Client->OnDoubleClicked(bi);
		} else if (DCount == 3) {
			// DOWNが3回あった
			
			// トリプルクリック確定で長押しの検知はキャンセル
			#ifdef USE_PROXYOBJ
			::KillTimer(NULL, m_PressTimerIds[bi]);
			#endif
			#ifdef USE_MSGWND
			KillTimer(MAKELONG(bi, 1));
			#endif
			m_PressTimerIds[bi] = 0;
			
			m_Client->OnTripleClicked(bi);
		}
	} else {
		m_Client->OnLongPressed(bi, dwNow - m_PressStartTimes[bi]);
	}
}

LPCTSTR CButtonStatus::GetButtonStates(void)
{
	int hil = m_Index % BUTTON_STATE_HISTORY_COUNT;
	::_tcscpy_s(m_LastStates, BUTTON_STATE_BUTTON_COUNT + 1, m_States[hil].States);
	return m_LastStates;
}

BOOL CButtonStatus::SetButtonParams(DWORD bi, DWORD dwDoubleClick, DWORD dwTripleClick, DWORD dwLongPress)
{
	if (bi >= BUTTON_STATE_BUTTON_COUNT) {
		return FALSE;
	}
	m_Settings[bi].dwDoubleClick = dwDoubleClick;
	m_Settings[bi].dwTripleClick = dwTripleClick;
	m_Settings[bi].dwLongPress = dwLongPress;
	return TRUE;
}
