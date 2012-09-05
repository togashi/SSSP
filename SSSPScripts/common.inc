// 定数
TIME_DOUBLECLICK = 500;
TIME_TRIPLECLICK = 1000;
TIME_LONGPRESS = 1000;

TIME_LONGPRESS_1 = 1 * TIME_LONGPRESS;
TIME_LONGPRESS_2 = 2 * TIME_LONGPRESS;
TIME_LONGPRESS_3 = 3 * TIME_LONGPRESS;

// コンテクストデータ
var hWindow = 0;
var buttonStatus = 'uuuuuuuuu';

////////////////////////////////////////////////////////////////////////////////
//
// イベントハンドラー
//
////////////////////////////////////////////////////////////////////////////////

function onAttach(window, model, version) {
	//SmartScroll.TraceOut('onAttach');
	updateButtons();
	hWindow = window;
}

function onDetach() {
	//SmartScroll.TraceOut('onDetach');
	CollectGarbage();
}

function onWheel(window, delta, direction) {
	if (delta > 0) {
		wheelActions[0].exec(hWindow);
	} else {
		wheelActions[1].exec(hWindow);
	}
}

function onButton(window, button, states) {
	buttonStatus = states;
}

function onButtonClick(window, button) {
	clickActions[button].exec(window);
}

function onButtonDoubleClick(window, button) {
	doubleClickActions[button].exec(window);
}

function onButtonTripleClick(window, button) {
	tripleClickActions[button].exec(window);
}

function onButtonLongPress(window, button, duration) {
	if (duration >= TIME_LONGPRESS_1 && duration < TIME_LONGPRESS_2) {
		longPressActions[button].exec(window);
	} else if (duration >= TIME_LONGPRESS_2 && duration < TIME_LONGPRESS_3) {
		longLongPressActions[button].exec(window);
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// ユーティリティ
//
////////////////////////////////////////////////////////////////////////////////

function KEY_DOWN(vk) {
	return { type:INPUT_KEYBOARD, wVk:vk };
}

function KEY_UP(vk) {
	return { type:INPUT_KEYBOARD, wVk:vk, dwFlags:KEYEVENTF_KEYUP };
}

function MOUSE_WHEEL(d) {
	return { type:INPUT_MOUSE, mouseData:d, dwFlags:MOUSEEVENTF_WHEEL };
}

function isString(t) {
	return (t != null) && ((typeof(t) == "string") || (t instanceof String));
}

function updateButtons() {
	//SmartScroll.TraceOut('updateButtonParams');
	for (i = 0; i < 9; i++) {
		dc = 0;
		tc = 0;
		lp = 0;
		txt = '';
		ico = '';
		ici = 0;
		if (i == 8) {
			if (wheelActions[0] != undefined && wheelActions[0].text != false) {
				if (isString(wheelActions[0].text)) {
					if (txt.length > 0) {
						txt += '　';
					}
					txt += '↑=' + wheelActions[0].text;
				}
			}
			if (wheelActions[1] != undefined && wheelActions[1].text != false) {
				if (isString(wheelActions[1].text)) {
					if (txt.length > 0) {
						txt += '　';
					}
					txt += '↓=' + wheelActions[1].text;
				}
			}
		}
		if (clickActions[i] != undefined && clickActions[i].text != false) {
			if (isString(clickActions[i].text)) {
				if (txt.length > 0) {
					txt += '　';
				}
				txt += 'クリック=' + clickActions[i].text;
			}
			if (clickActions[i].icon != undefined) {
				ico = clickActions[i].icon;
			}
		}
		if (doubleClickActions[i] != undefined && doubleClickActions[i].text != false) {
			dc = TIME_DOUBLECLICK;
			if (isString(doubleClickActions[i].text)) {
				if (txt.length > 0) {
					txt += '　';
				}
				txt += 'ダブルクリック=' + doubleClickActions[i].text;
			}
		}
		if (tripleClickActions[i] != undefined && tripleClickActions[i].text != false) {
			dc = TIME_TRIPLECLICK;
			if (isString(tripleClickActions[i].text)) {
				if (txt.length > 0) {
					txt += '　';
				}
				txt += 'トリプルクリック=' + tripleClickActions[i].text;
			}
		}
		if (longPressActions[i] != undefined && longPressActions[i].text != false) {
			lp = TIME_LONGPRESS;
			if (isString(longPressActions[i].text)) {
				if (txt.length > 0) {
					txt += '　';
				}
				txt += '長押し=' + longPressActions[i].text;
			}
		}
		if (longLongPressActions[i] != undefined && longLongPressActions[i].text != false) {
			lp = TIME_LONGPRESS;
			if (isString(longLongPressActions[i].text)) {
				if (txt.length > 0) {
					txt += '　';
				}
				txt += '' + 2 * TIME_LONGPRESS / 1000 + '秒以上長押し=' + longLongPressActions[i].text;
			}
		}
		SSSP.SetButtonParams(i, dc, tc, lp);
		SSSP.SetButtonText(i, txt);
		SSSP.SetButtonImage(i, ico);
		//SmartScroll.TraceOut('ButtonText: ' + txt);
	}
}

function getHelp() {
	var s = '';
	for (i = 0; i < 9; i++) {
		txt = '';
		if (i == 8) {
			if (wheelActions[0] != undefined && wheelActions[0].text != false) {
				if (isString(wheelActions[0].text)) {
					if (txt.length > 0) {
						txt += '\r\n';
					}
					txt += '　　↑=' + wheelActions[0].text;
				}
			}
			if (wheelActions[1] != undefined && wheelActions[1].text != false) {
				if (isString(wheelActions[1].text)) {
					if (txt.length > 0) {
						txt += '\r\n';
					}
					txt += '　　↓=' + wheelActions[1].text;
				}
			}
		}
		if (clickActions[i] != undefined && clickActions[i].text != false) {
			if (isString(clickActions[i].text)) {
				if (txt.length > 0) {
					txt += '\r\n';
				}
				txt += '　　クリック=' + clickActions[i].text;
			}
		}
		if (doubleClickActions[i] != undefined && doubleClickActions[i].text != false) {
			if (isString(doubleClickActions[i].text)) {
				if (txt.length > 0) {
					txt += '\r\n';
				}
				txt += '　　ダブルクリック=' + doubleClickActions[i].text;
			}
		}
		if (tripleClickActions[i] != undefined && tripleClickActions[i].text != false) {
			if (isString(tripleClickActions[i].text)) {
				if (txt.length > 0) {
					txt += '\r\n';
				}
				txt += '　　トリプルクリック=' + tripleClickActions[i].text;
			}
		}
		if (longPressActions[i] != undefined && longPressActions[i].text != false) {
			if (isString(longPressActions[i].text)) {
				if (txt.length > 0) {
					txt += '\r\n';
				}
				txt += '　　長押し=' + longPressActions[i].text;
			}
		}
		if (longLongPressActions[i] != undefined && longLongPressActions[i].text != false) {
			if (isString(longLongPressActions[i].text)) {
				if (txt.length > 0) {
					txt += '\r\n';
				}
				txt += '　　' + 2 * TIME_LONGPRESS / 1000 + '秒以上長押し=' + longLongPressActions[i].text;
			}
		}
		s += '【' + ((i != 8) ? ('ボタン' + (i + 1)) : 'ホイール') + '】\r\n' + txt + '\r\n';
	}
	return s;
}

////////////////////////////////////////////////////////////////////////////////
//
// アクション
//
////////////////////////////////////////////////////////////////////////////////

// ダミーアクション
var noop = {
	text:false,
	icon:'',
	exec:function (w) {}
};

var sspad_toggle = {
	text:'SmartScrollパッド表示切り替え',
	icon:'',
	exec:function (w) {
		MessageBeep(MB_OK);
		SmartScroll.ShowSmartScrollPad(SP_TOGGLE);
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

var script_help = {
	text:'ヘルプ',
	icon:'',
	block:false,
	exec:function (w) {
		if (this.block == true) {
			return;
		}
		this.block = true;
		MessageBox(hWindow, getHelp(), 'ヘルプ - 機能の割り当て', MB_ICONINFORMATION | MB_OK);
		this.block = false;
	}
};

