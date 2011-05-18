
function onLoad() {
	//SmartScroll.TraceOut('onLoad');
	SetTitle('[S]: MPC');
	SetWheelDefault(false);
	SetScrollDefault(false);
	SetScrollParams(20, 2000);
	SetWheelParams(3, 500);
	updateButtons();
}

function isApplicable(window) {
	//SmartScroll.TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/.+\\mplayerc.exe$/i)) {
		wt = GetWindowText(window);
		if (wt.match(/ - Media Player Classic$/) || wt == 'Media Player Classic') {
			return true;
		}
	}
	return false;
}

function onScroll(window, lx, ly, d) {
	if (lx != 0) {
		span = Math.abs(lx);
		vkdir = (lx > 0) ? VK_RIGHT : VK_LEFT;
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:vkdir },
			{ type:INPUT_KEYBOARD, wVk:vkdir, dwFlags:KEYEVENTF_KEYUP},
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
}

SSSP.Import("common.inc");

////////////////////////////////////////////////////////////////////////////////
//
// �A�N�V����
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
	text:'��~',
	icon:'mpc.png,0',
	exec:function (w) {
		SendInput(keyPress(VK_OEM_PERIOD));
	}
};

var play_pause = {
	text:'�Đ�/�ꎞ��~',
	icon:'mpc.png,1',
	exec:function (w) {
		SendInput(keyPress(VK_SPACE));
	}
};

var play_next = {
	text:'����',
	icon:'mpc.png,2',
	exec:function (w) {
		SendInput(keyPress(VK_NEXT));
	}
};

var play_prev = {
	text:'�O��',
	icon:'mpc.png,3',
	exec:function (w) {
		SendInput(keyPress(VK_PRIOR));
	}
};

var vol_mute = {
	text:'�~���[�g',
	icon:'mpc.png,6',
	exec:function (w) {
		SendInput([
			keyDown(VK_CONTROL),
			keyDown(VK_M),
			keyUp(VK_M),
			keyUp(VK_CONTROL)
		]);
	}
};

var vol_up = {
	text:'���ʂ��グ��',
	exec:function (w) {
		SendInput(keyPress(VK_UP));
	}
};

var vol_down = {
	text:'���ʂ�������',
	exec:function (w) {
		SendInput(keyPress(VK_DOWN));
	}
};

var list_show = {
	text:'�v���C���X�g',
	exec:function (w) {
		SendInput([
			keyDown(VK_CONTROL),
			keyDown(VK_7),
			keyUp(VK_7),
			keyUp(VK_CONTROL)
		]);
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// �@�\���蓖��
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
