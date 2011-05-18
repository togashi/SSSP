
function onLoad() {
	//TraceOut('onLoad');
	SetTitle('[S]: Photo Viewer');
	SetWheelDefault(true);
	SetScrollDefault(false);
	//SetScrollParams(20, 2000);
	//SetWheelParams(3, 1000);
	updateButtons();
}

function isApplicable(window) {
	//TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/\\dllhost\.exe$/i)) {
		cn = GetClassName(window);
		if (cn == 'Photo_Lightweight_Viewer') {
			return true;
		}
	}
	return false;
}

function onScroll(window, lx, ly, d) {
	var keys = new Array();
	if (lx > 0) {
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_RIGHT });
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_RIGHT, dwFlags:KEYEVENTF_KEYUP });
	} else if (lx < 0) {
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_LEFT });
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_LEFT, dwFlags:KEYEVENTF_KEYUP });
	}
	if (ly > 0) {
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_DOWN });
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_DOWN, dwFlags:KEYEVENTF_KEYUP });
	} else if (ly < 0) {
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_UP });
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_UP, dwFlags:KEYEVENTF_KEYUP });
	}
	if (keys.length > 0) {
		keys.unshift({ type:INPUT_KEYBOARD, wVk:VK_MENU });
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_MENU, dwFlags:KEYEVENTF_KEYUP });
	}
	SendInput(keys);
}

Import("common.inc");

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

var wnd_close = {
	text:'閉じる',
	icon:'wpv.png,1',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_MENU },
			{ type:INPUT_KEYBOARD, wVk:VK_F4 },
			{ type:INPUT_KEYBOARD, wVk:VK_F4, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_MENU, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
}

var view_next = {
	text:'次のファイル',
	icon:'wpv.png,3',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_RIGHT },
			{ type:INPUT_KEYBOARD, wVk:VK_RIGHT, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var view_prev = {
	text:'前のファイル',
	icon:'wpv.png,2',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_LEFT },
			{ type:INPUT_KEYBOARD, wVk:VK_LEFT, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var zoom_reset = {
	text:'ウィンドウに合わせる',
	icon:'wpv.png,4',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_0 },
			{ type:INPUT_KEYBOARD, wVk:VK_0, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};


////////////////////////////////////////////////////////////////////////////////
//
// 機能割り当て
//
////////////////////////////////////////////////////////////////////////////////

var clickActions = [
	wnd_close,		noop,			noop,			noop,
	noop,			noop,			view_prev,		view_next,
	zoom_reset
];

var doubleClickActions = [
	noop,			noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	script_help
];

var tripleClickActions = [
	noop,			noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	noop
];

var longPressActions = [
	noop,			noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	sspad_toggle
];

var longLongPressActions = [
	noop,			noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	noop
];

var wheelActions = [
	noop,
	noop
];
