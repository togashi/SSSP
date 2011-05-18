function onLoad() {
	SetTitle('[S]: トラックボール');
	SetWheelDefault(false);
	SetScrollDefault(false);
	SetButtonParams(8, 0, 0, 1000);
}

function isApplicable(window) {
	//TraceOut('fileversion: ' + GetWindowFileVersion(window));
	//TraceOut('filename: ' + GetWindowFilename(window));
	//TraceOut('classname: ' + GetClassName(window));
	//TraceOut('text: ' + GetWindowText(window));
	return false;
}

function onScroll(window, x, y) {
	SendInput([
		{ type:INPUT_MOUSE, dx:x, dy:y, dwFlags:MOUSEEVENTF_MOVE }
	]);
}

function onWheel(window, d) {
	SendInput([
		{ type:INPUT_MOUSE, mouseData:(d * WHEEL_DELTA / 4), dwFlags:MOUSEEVENTF_WHEEL }
	]);
}

function onButtonDown(window, b) {
	var f = 0;
	if (b == 6) {
		f = MOUSEEVENTF_RIGHTDOWN;
	} else if (b == 7) {
		f = MOUSEEVENTF_LEFTDOWN;
	} else if (b == 8) {
		f = MOUSEEVENTF_MIDDLEDOWN;
	}
	if (f != 0) {
		SendInput([
			{ type:INPUT_MOUSE, dwFlags:f }
		]);
	}
}

function onButtonUp(window, b) {
	var f = 0;
	if (b == 6) {
		f = MOUSEEVENTF_RIGHTUP;
	} else if (b == 7) {
		f = MOUSEEVENTF_LEFTUP;
	} else if (b == 8) {
		f = MOUSEEVENTF_MIDDLEUP;
	}
	if (f != 0) {
		SendInput([
			{ type:INPUT_MOUSE, dwFlags:f }
		]);
	}
}

function onButtonLongPress(window, b, d) {
	MessageBeep(MB_OK);
	ShowSmartScrollPad(SP_TOGGLE);
}
