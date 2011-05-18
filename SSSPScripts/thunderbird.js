
function onLoad() {
	//SmartScroll.TraceOut('onLoad');
	SSSP.SetTitle('[S]: Thunderbird');
	SSSP.SetWheelDefault(false);
	SSSP.SetScrollDefault(false);
	SSSP.SetScrollParams(20, 2000);
	SSSP.SetWheelParams(3, 1000);
	updateButtons();
}

function isApplicable(window) {
	//SmartScroll.TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/.+\\thunderbird.exe/i)) {
		wt = GetWindowText(window);
		if (wt.match(/ - Mozilla Thunderbird$/) || wt.match(/^Mozilla Thunderbird$/)) {
			// �{�̃E�B���h�E�Ή�
			return true;
		}
	}
	return false;
}

function onScroll(window, lx, ly, d) {
	if (ly != 0) {
		SendMouseWheel(hWindow, 0, -ly * WHEEL_DELTA / 4);
	}
	if (lx != 0) {
		SendMouseHWheel(hWindow, 0, lx * WHEEL_DELTA / 4);
	}
}

SSSP.Import("common.inc");

////////////////////////////////////////////////////////////////////////////////
//
// �A�N�V����
//
////////////////////////////////////////////////////////////////////////////////

var mark_read = {
	text:'�}�[�N:���ǂɂ���',
	icon:'thunderbird.png,1',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_M },
			{ type:INPUT_KEYBOARD, wVk:VK_M, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var mark_readall = {
	text:'�}�[�N:���ׂĊ��ǂɂ���',
	icon:'thunderbird.png,2',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT },
			{ type:INPUT_KEYBOARD, wVk:VK_C },
			{ type:INPUT_KEYBOARD, wVk:VK_C, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var mark_junk = {
	text:'�}�[�N:���f���[��',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_J },
			{ type:INPUT_KEYBOARD, wVk:VK_J, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var msg_delete = {
	text:'���b�Z�[�W���폜',
	icon:'thunderbird.png,3',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_DELETE },
			{ type:INPUT_KEYBOARD, wVk:VK_DELETE, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
}

var msg_erase = {
	text:'���b�Z�[�W�����S�ɍ폜',
	icon:'thunderbird.png,4',
	exec:function (w) {
		MessageBeep(MB_ICONERROR);
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT },
			{ type:INPUT_KEYBOARD, wVk:VK_DELETE },
			{ type:INPUT_KEYBOARD, wVk:VK_DELETE, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
}

var sel_all = {
	text:'���ׂđI��',
	icon:'thunderbird.png,5',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_A },
			{ type:INPUT_KEYBOARD, wVk:VK_A, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
}

var sel_20 = {
	text:'20�I��',
	icon:'thunderbird.png,15',
	exec:function (w) {
		keys = new Array();
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_SHIFT });
		for (i = 0; i < 20; i++) {
			keys.push({ type:INPUT_KEYBOARD, wVk:VK_DOWN });
			keys.push({ type:INPUT_KEYBOARD, wVk:VK_DOWN, dwFlags:KEYEVENTF_KEYUP });
		}
		keys.push({ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP });
		SendInput(keys);
		WaitForInputIdle(hWindow, INFINITE);
	}
}

var tab_next = {
	text:'�^�u:����',
	icon:'thunderbird.png,7',
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
	text:'�^�u:�O��',
	icon:'thunderbird.png,6',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_PRIOR },
			{ type:INPUT_KEYBOARD, wVk:VK_PRIOR, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tab_close = {
	text:'�^�u:����',
	icon:'thunderbird.png,0',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_W },
			{ type:INPUT_KEYBOARD, wVk:VK_W, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var nav_next = {
	text:'�ړ�:�i��',
	icon:'thunderbird.png,9',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F },
			{ type:INPUT_KEYBOARD, wVk:VK_F, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var nav_nextnew = {
	text:'�ړ�:���̖���',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_N },
			{ type:INPUT_KEYBOARD, wVk:VK_N, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var nav_prev = {
	text:'�ړ�:�߂�',
	icon:'thunderbird.png,8',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_B },
			{ type:INPUT_KEYBOARD, wVk:VK_B, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var nav_prevnew = {
	text:'�ړ�:�O�̖���',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_P },
			{ type:INPUT_KEYBOARD, wVk:VK_P, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var key_pageup = {
	text:'PAGEUP',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_PRIOR },
			{ type:INPUT_KEYBOARD, wVk:VK_PRIOR, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var key_pagedown = {
	text:'PAGEDOWN',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_NEXT },
			{ type:INPUT_KEYBOARD, wVk:VK_NEXT, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
}

var beep_alert = {
	text:true,
	exec:function (w) {
		MessageBeep(MB_ICONWARNING);
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// �@�\���蓖��
//
////////////////////////////////////////////////////////////////////////////////

var clickActions = [
	tab_close,		mark_read,		tab_back,			tab_next,
	sel_20,			msg_delete,		nav_prev,			nav_next,
	noop
];

var doubleClickActions = [
	window_maximize,noop,			noop,				noop,
	noop,			mark_junk,		noop,				noop,
	script_help
];

var tripleClickActions = [
	noop,			noop,			noop,				noop,
	noop,			noop,			noop,				noop,
	noop
];

var longPressActions = [
	window_minimize,mark_readall,	noop,				noop,
	sel_all,		beep_alert,		nav_prevnew,		nav_nextnew,
	sspad_toggle
];

var longLongPressActions = [
	noop,			noop,			noop,				noop,
	noop,			msg_erase,		noop,				noop,
	noop
];

var wheelActions = [
	key_pageup,
	key_pagedown
];
