// SSSP.cpp : DLL エクスポートの実装です。


#include "stdafx.h"

#include "sspscript.h"
#include "sspscripts.h"

CSSPScripts g_Scripts;

///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
//  プラグインファイルがドライバによりロードされた時に呼ばれる関数です
// < パラメータ >
//	pParam->dwModel		: SmartScrollのモデル番号
//	pParam->dwVersion	: SmartScrollドライバのバージョン
// < 戻り値 >
// SSP_TRUE		: このプラグインは常駐されます
// SSP_FALSE	: このプラグインの常駐は解除されます
SSP_STATUS WINAPI SspLoadPlugin(PLOADPARAM pParam)
{
	g_Scripts.AddSearchPath(CSIDL_APPDATA, _T("SmartScroll SSSP"));
	return SSP_TRUE;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
//  フォアグラウンドウィンドウに対して、このプラグインを適用するかどうかを判断する関数です
// < パラメータ >
// hWnd		: フォアグラウンドウィンドウのハンドル
// < 戻り値 >
// SSP_TRUE		: このプラグインが適用される
// SSP_FALSE	: このプラグインは適用されない
SSP_STATUS WINAPI SspApplyProc(HWND hWnd)
{
	SSP_STATUS ret;
	g_Scripts.Call_ApplyProc(hWnd, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// プラグインが適用される時、最初に呼び出される関数です
// < パラメータ >
//	pParam->hwndApp		: フォアグラウンドウィンドウのハンドル
//	pParam->dwModel		: SmartScrollのモデル番号
//	pParam->dwVersion	: SmartScrollドライバのバージョン
// < 戻り値 >
// SSP_TRUE		: 通常はこの値を返してください
// SSP_FALSE	: プラグインの適用が取り消されます
// SSP_TIMER    : SspTimerProc関数を使用する場合は、SSP_TIMERを戻り値として返す
SSP_STATUS WINAPI SspInitProc(PINITPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_InitProc(pParam, ret);
	return SSP_TIMER;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// 約100ms間隔で呼び出されるタイマ関数です。
//（ドライババージョン1.20以降で対応）
void WINAPI SspTimerProc(void)
{
	g_Scripts.Call_TimerProc();
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// プラグインの適用が終了する時に呼び出される関数です
void WINAPI SspReleaseProc(void)
{
	g_Scripts.Call_ReleaseProc();
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// ファンクションキーが操作された場合の処理を行う関数です
// < パラメータ >
//	pParam->hwndMouse	: マウスカーソルの真下にあるウィンドウのハンドル
//	pParam->dwType		: イベントタイプ
// < 戻り値 >
// SSP_TRUE		: 通常はこの値を返してください
// SSP_SHOW_HIDE: SmartScrollウィンドウの表示／非表示が切り替わります
SSP_STATUS WINAPI SspButtonProc(PBUTTONPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_ButtonProc(pParam, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// ズームホイール機能の有効／無効を判断する関数です
// < 戻り値 >
// SSP_TRUE		: ズームホイールは有効です
// SSP_FALSE	: ズームホイールは無効です
SSP_STATUS WINAPI SspGetWheelEffect(void)
{
	SSP_STATUS ret;
	g_Scripts.Call_GetWheelEffect(ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// ズームホイールが操作された場合の処理を行う関数です
// < パラメータ >
//	pParam->hwndMouse	: マウスカーソルの真下にあるウィンドウのハンドル
//	pParam->lRoll		: ズームホイールの回転量
//	pParam->dwDirection	: 回転方向フラグ
// < 戻り値 >
// SSP_TRUE		: デフォルトのズーム処理は行われません
// SSP_FALSE	: デフォルトのズーム処理が行われます
SSP_STATUS WINAPI SspWheelProc(PWHEELPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_WheelProc(pParam, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// スクロールボールが操作された場合の処理を行う関数です
// < パラメータ >
//	pParam->hwndMouse	: マウスカーソルの真下にあるウィンドウのハンドル
//	pParam->hwndOwner	: hwndMouseの親ウィンドウのハンドル
//	pParam->hwndHScroll	: 横スクロールバーコントロールのハンドル
//	pParam->hwndVScroll	: 縦スクロールバーコントロールのハンドル
//	pParam->lX			: スクロールボールのX方向の回転量
//	pParam->lY			: スクロールボールのY方向の回転量
//	pParam->dwDirection	: 回転方向フラグ
// < 戻り値 >
// SSP_TRUE		: デフォルトのスクロール処理は行われません
// SSP_FALSE	: デフォルトのスクロール処理が行われます
SSP_STATUS WINAPI SspScrollProc(PSCROLLPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_ScrollProc(pParam, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// SmartScrollパッドのボタンに表示されるアイコンハンドルを提供する関数です
// < パラメータ >
//	dwType	: ボタンの種類
// < 戻り値 >
// 各ボタン用のアイコンハンドルを返します
HICON WINAPI SspGetIcon(DWORD dwType)
{
	HICON ret;
	g_Scripts.Call_GetIcon(dwType, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// プラグインが持つ文字列リソースを提供する関数です
// < パラメータ >
//	dwType	: リソースの種類
//	lpStr	: 文字列を受け取るバッファを指すポインタ
//	uiSize	: バッファのサイズ
// < 戻り値 >
// バッファにコピーされた文字数を返します
int WINAPI SspGetString(DWORD dwType, LPSTR lpStr, UINT uiSize)
{
	int ret;
	if (dwType == SSP_NAME) {
		::strncpy_s(lpStr, uiSize, "スクリプトプラグイン", uiSize - 1);
		ret = ::strlen(lpStr);
	} else {
		g_Scripts.Call_GetString(dwType, lpStr, uiSize, ret);
	}
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < 機能 >
// 設定ダイアログ用のダイアログプロシージャです
BOOL CALLBACK SspDialogProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	::EndDialog(hWnd, 0);
	return TRUE;
}
