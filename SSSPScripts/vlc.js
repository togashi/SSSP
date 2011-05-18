
function onLoad() {
	//SmartScroll.TraceOut('onLoad');
	SetTitle('[S]: VLC');
	SetWheelDefault(false);
	SetScrollDefault(false);
	SetScrollParams(20, 2000);
	SetWheelParams(3, 500);
	updateButtons();
}

function isApplicable(window) {
	//SmartScroll.TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/.+\\vlc.exe$/i)) {
		wt = GetWindowText(window);
		if (wt.match(/ - VLCメディアプレイヤー$/) || wt == 'VLCメディアプレイヤー') {
			return true;
		}
	}
	return false;
}

function onScroll(window, lx, ly, d) {
	if (lx != 0) {
		span = Math.abs(lx);
		vkdir = (lx > 0) ? VK_RIGHT : VK_LEFT;
		if (span > 5) {
			keys = [
				{ type:INPUT_KEYBOARD, wVk:VK_MENU },
				{ type:INPUT_KEYBOARD, wVk:vkdir },
				{ type:INPUT_KEYBOARD, wVk:vkdir, dwFlags:KEYEVENTF_KEYUP},
				{ type:INPUT_KEYBOARD, wVk:VK_MENU, dwFlags:KEYEVENTF_KEYUP }
			];
		} else {
			keys = [
				{ type:INPUT_KEYBOARD, wVk:VK_SHIFT },
				{ type:INPUT_KEYBOARD, wVk:vkdir },
				{ type:INPUT_KEYBOARD, wVk:vkdir, dwFlags:KEYEVENTF_KEYUP},
				{ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP }
			];
		}
		SendInput(keys);
	}
}

SSSP.Import("common.inc");

////////////////////////////////////////////////////////////////////////////////
//
// アクション
//
////////////////////////////////////////////////////////////////////////////////

function keyDown(vk) {
	return { type:INPUT_KEYBOARD, wVk:vk };
}

function keyUp(vk) {
	return { type:INPUT_KEYBOARD, wVk:vk, dwFlags:KEYEVENTF_KEYUP };
}

function keyPress(vk) {
	return [{ type:INPUT_KEYBOARD, wVk:vk }, { type:INPUT_KEYBOARD, wVk:vk, dwFlags:KEYEVENTF_KEYUP }];
}

var play_stop = {
	text:'停止',
	icon:'vlc.png,0',
	exec:function (w) {
		SendInput(keyPress(VK_S));
	}
};

var play_pause = {
	text:'再生/一時停止',
	icon:'vlc.png,1',
	exec:function (w) {
		SendInput(keyPress(VK_SPACE));
	}
};

var play_next = {
	text:'次へ',
	icon:'vlc.png,2',
	exec:function (w) {
		SendInput(keyPress(VK_N));
	}
};

var play_prev = {
	text:'前へ',
	icon:'vlc.png,3',
	exec:function (w) {
		SendInput(keyPress(VK_P));
	}
};

var vol_mute = {
	text:'ミュート',
	icon:'vlc.png,6',
	exec:function (w) {
		SendInput(keyPress(VK_M));
	}
};

var vol_up = {
	text:'音量を上げる',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_UP },
			{ type:INPUT_KEYBOARD, wVk:VK_UP, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var vol_down = {
	text:'音量を下げる',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_DOWN },
			{ type:INPUT_KEYBOARD, wVk:VK_DOWN, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var list_show = {
	text:'プレイリスト',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_L },
			{ type:INPUT_KEYBOARD, wVk:VK_L, dwFlags:KEYEVENTF_KEYUP },
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
	play_stop,		play_pause,		play_prev,		play_next,
	noop,			list_show,		noop,			noop,
	vol_mute
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
	vol_up,
	vol_down
];
