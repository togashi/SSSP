function onLoad() {
	TraceOut('onLoad');
}

function onEvent() {
	TraceOut('onEvent');
}

function isApplicable(w) {
	return false;
}

function _ApplyProc(w) {
	return false;
	
	fn = GetWindowFilename(w);
	if (fn.match(/.+\\(excel|firefox|thunderbird).exe/i)) {
		return 0;
	}
	
	SmartScroll.TraceOut('Now, \"ApplyProc\" is running.');
	s = '  HWND: ' + w;
	SmartScroll.TraceOut(s);
	s = '  ClassName: ' + GetClassName(w);
	SmartScroll.TraceOut(s);
	s = '  Text: ' + GetWindowText(w);
	SmartScroll.TraceOut(s);
	s = '  filename: ' + GetWindowFilename(w);
	SmartScroll.TraceOut(s);
	//return 1;
	
	a = new Array;
	a['“ú–{Œê'] = 1;
	a['‚¤‚Ö‚Ö'] = 2;
	a = new Object;
	a.Property = 10;
	a.Method = function(x){return x*2;}
	a['Method2'] = function(y){return y/2;}
	//delete a.“ú–{Œê;
	return 1;
}

function InitProc(w, m, v) {
	//SmartScroll.TraceOut('InitProc');
	var b = SSSP.SetButtonParams(0, 500, 1000, 1000);
	//SmartScroll.TraceOut('  b:' + b);

	SSSP.SetButtonImage(0, "C:\\usr\\lib\\icons\\diskstate\\hdd_00.ico");
	SSSP.SetButtonImage(1, "C:\\usr\\lib\\icons\\diskstate\\hdd_01.ico");
	SSSP.SetButtonImage(2, "C:\\usr\\lib\\icons\\diskstate\\hdd_10.ico");
	SSSP.SetButtonImage(3, "C:\\usr\\lib\\icons\\diskstate\\hdd_11.ico");
	SSSP.SetButtonImage(4, "C:\\usr\\lib\\icons\\diskstate\\hdd_00.ico");
	SSSP.SetButtonImage(5, "C:\\usr\\lib\\icons\\diskstate\\hdd_01.ico");
	SSSP.SetButtonImage(6, "C:\\usr\\lib\\icons\\diskstate\\hdd_10.ico");
	SSSP.SetButtonImage(7, "C:\\usr\\lib\\icons\\diskstate\\hdd_11.ico");
	SSSP.SetButtonImage(8, "C:\\usr\\lib\\icons\\diskstate\\hdd_11.ico");
	
	return 1;
}

function ReleaseProc() {
	//SmartScroll.TraceOut('ReleaseProc');
}

function ButtonProc(w, t) {
	//SmartScroll.TraceOut('ButtonProc');
	//f = GetWindowFileName(w);
	//SmartScroll.TraceOut('  w: ' + f);
	//SmartScroll.TraceOut('  w: ' + GetWindowFileVersion(w));
	//SmartScroll.TraceOut('  !: ' + SmartScroll.getFileVersion(f));
	//SmartScroll.TraceOut('  t: ' + t);
	
	if ((t & 0xf) != 1) {
		wr = GetAncestor(w, GA_ROOT);
		ws = GetWindowState(wr);
		if (ws == SW_SHOWMAXIMIZED) {
			SendSysCommand(wr, SC_RESTORE, 0);
		} else {
			SendSysCommand(wr, SC_MAXIMIZE, 0);
		}
	}
	return 1;
	
	if ((t & 0xF) != 1) {
		SendKeybdInput(VK_0, 0);
		SendKeybdInput(VK_0, 1);
	}
	return 1;
}

function WheelProc(wm, dlt, dir)
{
	return 0;
	
	delta = -dlt * WHEEL_DELTA;
	SendKeybdInput(VK_CONTROL, 0);
	WaitForInputIdle(wm, 1000);
	
	if (GetAsyncKeyState(VK_CONTROL)) {
		SendWheelInput(0, delta);
		WaitForInputIdle(wm, 1000);
	}
	if (GetAsyncKeyState(VK_CONTROL)) {
		SendWheelInput(0, delta);
		WaitForInputIdle(wm, 1000);
	}
	if (GetAsyncKeyState(VK_CONTROL)) {
		SendWheelInput(0, delta);
		WaitForInputIdle(wm, 1000);
	}
	
	SendKeybdInput(VK_CONTROL, 1);
	WaitForInputIdle(wm, 1000);
	return 1;
}

var dwLastScroll = 0;
var dwScrollStart = 0;

function ScrollProc(wm, wo, wh, wv, x, y)
{
	now = GetTickCount();
	f = now - dwLastScroll;
	dwLastScroll = now;
	if (f > 1000) {
		dwScrollStart = now;
		f = 0;
	} else {
		f = now - dwScrollStart;
	}
	var delta;
	f = Math.floor(f / 10);
	if (f >= 40) {
		delta = 40;
	} else {
		delta = f;
	}
	SmartScroll.TraceOut(delta);
	
	if (x != 0) {
		if (x < 0) {
			SendHScroll(wm, 0, 0, 0);
		} else if (x > 0) {
			SendHScroll(wm, 1, 0, 0);
		}
	}
	if (y != 0) {
		SendWheelInput(0, -y * delta);
	}
	
	return 1;
}

var bWheelEnabled = 1;

var stra = "<>";
var strb = "[]";

function GetString(t) {
	switch (t) {
		case 101:
			return bWheelEnabled ? stra : strb;
			return 'TEST0.js';
		default:
			return String(t);
	}
}

function GetIcon(t) {
	
	//SmartScroll.TraceOut('GetIcon');
	return '';
}

function onButton(w, bi, bs) {
	SmartScroll.TraceOut(bs);
}

function onClick(w, b) {
	if (b == 8) {
		bWheelEnabled = 1 - bWheelEnabled;
		SSSP.SetWheelParams(0, 0, bWheelEnabled);
		SmartScroll.LoadPluginResource();
		return;
	}
	
	//SmartScroll.TraceOut('  w: ' + GetClassName(w) + ' (' + w + ')');
	//var wa = GetAncestor(w, 2);
	//SmartScroll.TraceOut('  wa: ' + GetClassName(wa) + ' (' + wa + ')');
}

function onLongPress(w, b, d) {
	s = '  b:' + b + ' d:' + d;
	SmartScroll.TraceOut(s);
	
	if (b == 0 && d >= 1000 && d < 2000) {
		SmartScroll.ShowSmartScrollPad(3);
	}
}
