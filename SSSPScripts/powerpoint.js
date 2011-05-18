
function onLoad() {
	//TraceOut('onLoad');
	SetTitle('[S]:PowerPoint');
	SetWheelDefault(false);
	SetScrollDefault(false);
	//SetScrollParams(20, 2000);
	//SetWheelParams(3, 1000);
	updateButtons();
}

function isApplicable(window) {
	fn = GetWindowFilename(window);
	if (fn.match(/\\POWERPNT\.EXE$/i)) {
		wt = GetWindowText(window);
		if (wt.match(/^Microsoft PowerPoint - /) || wt == "Microsoft PowerPoint") {
			return true;
		}
	}
	return false;
}

function onDetach(w) {
	TraceOut('onDetach');
	ppa = null;
}

var actionQueue = new Array;

function onScroll(window, x, y)
{
	var action = new Array;
	action.push("scroll");
	// below
	action.push((y > 0) ? y : 0);
	// above
	action.push((y < 0) ? -y : 0);
	// right
	action.push((x > 0) ? x : 0);
	// left
	action.push((x < 0) ? -x : 0);
	actionQueue.push(action);
	EnableTimer(true);
}

Import("common.inc");

var ppa = null;

var onAttach_super = onAttach;
onAttach = function (window, model, version) {
	ppa = new ActiveXObject("PowerPoint.Application.12");
	return onAttach_super(window, model, version);
};

var onDetach_super = onDetach;
onDetach = function () {
	ppa = null;
	return onDetach_super();
};

function onTimer()
{
	EnableTimer(false);
	var ppdw = ppa.ActiveWindow;
	if (ppdw == null) {
		return;
	}
	var action = null;
	for (action = actionQueue.shift(); action != null; action = actionQueue.shift()) {
		if (action[0] == "scroll") {
			ppdw.SmallScroll(action[1], action[2], action[3], action[4]);
		} else if (action[0] == "fitToPage") {
			ppdw.View.ZoomToFit = true;
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// �A�N�V����
//
////////////////////////////////////////////////////////////////////////////////

var key_enter = {
	text:'�L�[:Enter',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_RETURN),
			KEY_UP(VK_RETURN)
		]);
	}
};

var zoom_in = {
	text:'�Y�[��:�g��',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_CONTROL),
			MOUSE_WHEEL(WHEEL_DELTA),
			KEY_UP(VK_CONTROL)
		]);
	}
};

var zoom_out = {
	text:'�Y�[��:�k��',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_CONTROL),
			MOUSE_WHEEL(-WHEEL_DELTA),
			KEY_UP(VK_CONTROL)
		]);
	}
};

var zoom_fit = {
	text:'�Y�[��:�E�B���h�E�ɍ��킹��',
	icon:'powerpoint.png,11',
	exec:function (w) {
		actionQueue.push(["fitToPage"]);
		EnableTimer(true);
	}
};

var slide_prev = {
	text:'�O�̃X���C�h',
	icon:'powerpoint.png,14',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_UP),
			KEY_UP(VK_UP)
		]);
	}
};

var slide_next = {
	text:'���̃X���C�h',
	icon:'powerpoint.png,15',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_DOWN),
			KEY_UP(VK_DOWN)
		]);
	}
};

var slide_first = {
	text:'�ŏ��̃X���C�h',
	icon:'powerpoint.png,12',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_HOME),
			KEY_UP(VK_HOME)
		]);
	}
};

var slide_last = {
	text:'�Ō�̃X���C�h',
	icon:'powerpoint.png,13',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_END),
			KEY_UP(VK_END)
		]);
	}
};

var doc_close = {
	text:'����',
	icon:'powerpoint.png,21',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_CONTROL),
			KEY_DOWN(VK_W),
			KEY_UP(VK_W),
			KEY_UP(VK_CONTROL)
		]);
	}
};

var app_exit = {
	text:'�I��',
	icon:'powerpoint.png,21',
	exec:function (w) {
		SendInput([
			KEY_DOWN(VK_MENU),
			KEY_DOWN(VK_F4),
			KEY_UP(VK_F4),
			KEY_UP(VK_MENU)
		]);
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// �@�\���蓖��
//
////////////////////////////////////////////////////////////////////////////////

var clickActions = [
	doc_close,		slide_first,	slide_last,			slide_prev,
	noop,			slide_last,		slide_prev,			slide_next,
	zoom_fit
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
	app_exit,		noop,			noop,				noop,
	noop,			noop,			noop,				noop,
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
