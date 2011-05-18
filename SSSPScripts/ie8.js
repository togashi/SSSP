
function onLoad() {
	//SmartScroll.TraceOut('onLoad');
	SSSP.SetTitle('[S]: IE8');
	SSSP.SetWheelDefault(false);
	SSSP.SetScrollDefault(false);
	SSSP.SetScrollParams(20, 2000);
	SSSP.SetWheelParams(3, 1000);
	updateButtons();
}

function isApplicable(window) {
	//SmartScroll.TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/.+\\iexplore.exe/i)) {
		wt = GetWindowText(window);
		if (wt.match(/ - Windows Internet Explorer$/)) {
			// �u���E�U�E�B���h�E�̂ݑΉ�
			return true;
		}
	}
	return false;
}

function onScroll(window, lx, ly, d) {
	if (ly != 0) {
		SendInput([
			{ type:INPUT_MOUSE, mouseData:(-ly * WHEEL_DELTA / 4), dwFlags:MOUSEEVENTF_WHEEL }
		]);
	}
	//if (lx != 0) {
	//	SendMouseHWheel(window, 0, lx * WHEEL_DELTA / 4);
	//}
}

SSSP.Import("common.inc");

////////////////////////////////////////////////////////////////////////////////
//
// �A�N�V����
//
////////////////////////////////////////////////////////////////////////////////

var key_enter = {
	text:'�L�[:Enter',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_RETURN },
			{ type:INPUT_KEYBOARD, wVk:VK_RETURN, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var focus_next = {
	text:'�t�H�[�J�X������',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_TAB },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var focus_back = {
	text:'�t�H�[�J�X��O��',
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
	text:'�Y�[��:�g��',
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
	text:'�Y�[��:�k��',
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
	text:'�Y�[��:���Z�b�g',
	icon:'ie8.png,7',
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
	text:'�^�u:����',
	icon:'ie8.png,3',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_back = {
	text:'�^�u:�O��',
	icon:'ie8.png,2',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_new = {
	text:'�^�u:�V�����^�u',
	icon:'ie8.png,1',
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
	text:'�^�u:����',
	icon:'ie8.png,0',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_W },
			{ type:INPUT_KEYBOARD, wVk:VK_W, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var window_minimize = {
	text:'�E�B���h�E:�ŏ���',
	exec:function (w) {
		SendSysCommand(hWindow, SC_MINIMIZE, 0);
	}
};

var window_maximize = {
	text:'�E�B���h�E:�ő剻',
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
	text:'�u���E�U:�i��',
	icon:'ie8.png,6',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FORWARD },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FORWARD, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_back = {
	text:'�u���E�U:�߂�',
	icon:'ie8.png,5',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_BACK },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_BACK, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_reload = {
	text:'�u���E�U:�ēǂݍ���',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_REFRESH },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_REFRESH, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_search = {
	text:'�u���E�U:����',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_SEARCH },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_SEARCH, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var browser_bookmarks = {
	text:'�u���E�U:�u�b�N�}�[�N',
	icon:'ie8.png,4',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FAVORITES },
			{ type:INPUT_KEYBOARD, wVk:VK_BROWSER_FAVORITES, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// �@�\���蓖��
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
	window_minimize,noop,			noop,				noop,
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
