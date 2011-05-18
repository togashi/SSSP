#pragma once

#define BUTTON_STATE_HISTORY_COUNT 64
#define BUTTON_STATE_BUTTON_COUNT SSP_BUTTON_NUM

#define USE_MSGWND

interface IButtonEvent
{
public:
	virtual void OnClicked(DWORD iButton) = 0;
	virtual void OnDoubleClicked(DWORD iButton) = 0;
	virtual void OnTripleClicked(DWORD iButton) = 0;
	virtual void OnLongPressed(DWORD iButton, DWORD iDuration) = 0;
	virtual void OnReleaseLongPress(DWORD iButton, DWORD iDuration) = 0;
};

#define BS_DOWNED		_T('d')
#define BS_UPPED		_T('u')
#define BS_DOWN			_T('D')
#define BS_UP			_T('U')
#define BF_SUSPENDED    _T(' ')
#define BF_CLICK        _T('C')
#define BF_DOUBLECLICK  _T('D')
#define BF_TRIPLECLICK  _T('T')
#define BF_LONGPRESS	_T('L')

interface ITimerEvent
{
public:
	virtual void __cdecl TimerMethod(DWORD idEvent) = 0;
};

class CButtonStatus:
	#ifdef USE_MSGWND
	public CWindowImpl<CButtonStatus>,
	#endif
	#ifdef USE_PROXYOBJ
	public ITimerEvent,
	#endif
	public IButtonEvent
{
private:
	LONGLONG m_Index;
	struct state_ {
		TCHAR States[BUTTON_STATE_BUTTON_COUNT + 1];
		TCHAR Flags[BUTTON_STATE_BUTTON_COUNT + 1];
		DWORD dwTickCount;
	} m_States[BUTTON_STATE_HISTORY_COUNT];
	
	TCHAR m_LastStates[BUTTON_STATE_BUTTON_COUNT + 1];	// GetButtonStates 用コピー
	
	struct setting_ {
		DWORD dwTripleClick;
		DWORD dwDoubleClick;
		DWORD dwLongPress;
	} m_Settings[BUTTON_STATE_BUTTON_COUNT];

	IButtonEvent *m_Client;

	#ifdef USE_PROXYOBJ
	TIMERPROC CreateTimerProcObject(DWORD arg);
	void DeleteTimerProcObject(TIMERPROC obj);
	TIMERPROC m_DownTimerProcs[BUTTON_STATE_BUTTON_COUNT];
	TIMERPROC m_PressTimerProcs[BUTTON_STATE_BUTTON_COUNT];
	#endif

	DWORD m_DownStartTimes[BUTTON_STATE_BUTTON_COUNT];
	UINT_PTR m_DownTimerIds[BUTTON_STATE_BUTTON_COUNT];
	
	DWORD m_PressStartTimes[BUTTON_STATE_BUTTON_COUNT];
	UINT_PTR m_PressTimerIds[BUTTON_STATE_BUTTON_COUNT];
#ifdef USE_MSGWND
private:
	BEGIN_MSG_MAP(CButtonStatus)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
	END_MSG_MAP()
	LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
#endif
private:
	// IButtonEvent デフォルトハンドラ
	void OnClicked(DWORD iButton) {};
	void OnDoubleClicked(DWORD iButton) {};
	void OnTripleClicked(DWORD iButton) {};
	void OnLongPressed(DWORD iButton, DWORD iDuration) {};
	void OnReleaseLongPress(DWORD iButton, DWORD iDuration) {};
public:
	CButtonStatus(void);
	virtual ~CButtonStatus(void);
	
	// ITimerEvent
	void __cdecl TimerMethod(DWORD arg);
	
	BOOL SetButtonParams(DWORD bi, DWORD dwDoubleClick, DWORD dwTripleClick, DWORD dwLongPress);
	
	void SetClient(IButtonEvent *client);
	void Reset(void);
	void ButtonProc(DWORD dwType);
	LPCTSTR GetButtonStates(void);
};
