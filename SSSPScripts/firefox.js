
function onLoad() {
	//SmartScroll.TraceOut('onLoad');
	SSSP.SetTitle('[S]: Firefox');
	SSSP.SetWheelDefault(false);
	SSSP.SetScrollDefault(false);
	SSSP.SetScrollParams(20, 2000);
	SSSP.SetWheelParams(3, 1000);
	updateButtons();
}

function isApplicable(window) {
	//SmartScroll.TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/.+\\firefox.exe/i)) {
		wt = GetWindowText(window);
		if (wt.match(/ - Mozilla Firefox$/) || wt.match(/^Mozilla Firefox$/)) {
			// ブラウザウィンドウのみ対応
			return true;
		}
	}
	return false;
}

var WHEEL_FACTOR = WHEEL_DELTA / 8;

function onScroll(window, lx, ly, d) {
	if (ly != 0) {
		SendMouseWheel(hWindow, 0, -ly * WHEEL_FACTOR);
	}
	if (lx != 0) {
		SendMouseHWheel(hWindow, 0, lx * WHEEL_FACTOR);
	}
}

SSSP.Import("common.inc");

////////////////////////////////////////////////////////////////////////////////
//
// アクション
//
////////////////////////////////////////////////////////////////////////////////

var key_enter = {
	text:'キー:Enter',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_RETURN },
			{ type:INPUT_KEYBOARD, wVk:VK_RETURN, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var focus_next = {
	text:'フォーカスを次へ',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_TAB },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var focus_back = {
	text:'フォーカスを前へ',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var zoom_in = {
	text:'ズーム:拡大',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_OEM_PLUS },
			{ type:INPUT_KEYBOARD, wVk:VK_OEM_PLUS, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var zoom_out = {
	text:'ズーム:縮小',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_OEM_MINUS },
			{ type:INPUT_KEYBOARD, wVk:VK_OEM_MINUS, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var zoom_reset = {
	text:'ズーム:リセット',
	icon:'firefox.png,7',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_0 },
			{ type:INPUT_KEYBOARD, wVk:VK_0, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_next = {
	text:'タブ:次へ',
	icon:'firefox.png,3',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_NEXT },
			{ type:INPUT_KEYBOARD, wVk:VK_NEXT, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_back = {
	text:'タブ:前へ',
	icon:'firefox.png,2',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_PRIOR },
			{ type:INPUT_KEYBOARD, wVk:VK_PRIOR, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_new = {
	text:'タブ:新しいタブ',
	icon:'firefox.png,1',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_T },
			{ type:INPUT_KEYBOARD, wVk:VK_T, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_close = {
	text:'タブ:閉じる',
	icon:'firefox.png,0',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_W },
			{ type:INPUT_KEYBOARD, wVk:VK_W, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_revive = {
	text:'タブ:最後に閉じたタブ',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT },
			{ type:INPUT_KEYBOARD, wVk:VK_T },
			{ type:INPUT_KEYBOARD, wVk:VK_T, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var window_minimize = {
	text:'ウィンドウ:最小化',
	exec:function (w) {
		SendSysCommand(hWindow, SC_MINIMIZE, 0);
	}
};

var window_maximize = {
	text:'ウィンドウ:最大化',
	exec:function (w) {
		ws = GetWindowState(hWindow);
		if (ws == SW_SHOWMAXIMIZED) {
			SendSysCommand(hWindow, SC_RESTORE, 0);
		} else {
			SendSysCommand(hWindow, SC_MAXIMIZE, 0);
		}
	}
};

var browser_forward = {
	text:'ブラウザ:進む',
	icon:'firefox.png,6',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FORWARD },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FORWARD, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_back = {
	text:'ブラウザ:戻る',
	icon:'firefox.png,5',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_BACK },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_BACK, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_reload = {
	text:'ブラウザ:再読み込み',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_REFRESH },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_REFRESH, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_search = {
	text:'ブラウザ:検索',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_SEARCH },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_SEARCH, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_bookmarks = {
	text:'ブラウザ:ブックマーク',
	icon:'firefox.png,4',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FAVORITES },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FAVORITES, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// 機能割り当て
//
////////////////////////////////////////////////////////////////////////////////

var clickActions = [
	tab_close,		tab_new,		tab_back,			tab_next,
	noop,			browser_bookmarks,browser_back,		browser_forward,
	zoom_reset
];

var doubleClickActions = [
	window_maximize,noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	script_help
];

var tripleClickActions = [
	noop,			noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	noop
];

var longPressActions = [
	window_minimize,tab_revive,		noop,				noop,
	noop,			browser_reload,	noop,				noop,
	sspad_toggle
];

var longLongPressActions = [
	noop,			noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	noop
];

var wheelActions = [
	zoom_in,
	zoom_out
];
