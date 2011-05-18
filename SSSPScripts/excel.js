function onLoad() {
	//SmartScroll.TraceOut('onLoad');
	SSSP.SetWheelDefault(true);
	SSSP.SetScrollDefault(false);
	
	SSSP.SetTitle('[S]:Excel');
	for (i = 0; i < SSP_BUTTON_NUM; i++) {
		SSSP.SetButtonText(i, 'ƒ{ƒ^ƒ“' + (i + 1));
	}
}

function isApplicable(window) {
	//SmartScroll.TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/.+\\excel.exe/i)) {
		ver = GetWindowFileVersion(window);
		//SmartScroll.TraceOut(ver);
		if (ver.match(/^12\.\d+.\d+.\d+/)) {
			return true;
		}
	}
	return false;
}

var hWindow = 0;

function onAttach(window, model, version) {
	hWindow = window;
}

//function onDetach() {
//}

function onScroll(window, x, y)
{
	w = hWindow;
	cn = GetClassName(w);
	hs = 0;
	vs = 0;
	if (cn == 'XLMAIN') {
		w = FindWindowEx(w, 0, 'XLDESK', null);
		if (w == 0) {
			SmartScroll.TraceOut('XLDESK was not found.');
		} else {
			cn = GetClassName(w);
		}
	}
	if (cn == 'XLDESK') {
		w = FindWindowEx(w, 0, 'EXCEL7', null);
		if (w == 0) {
			SmartScroll.TraceOut('EXCEL7 was not found.');
		} else {
			cn = GetClassName(w);
		}
	}
	if (cn == 'EXCEL7') {
		hs = FindWindowEx(w, 0, 'NUIScrollbar', '…•½•ûŒü');
		if (hs == 0) {
			SmartScroll.TraceOut('NUIScrollbar was not found.');
		}
		vs = FindWindowEx(w, 0, 'NUIScrollbar', '‚’¼•ûŒü');
		if (vs == 0) {
			SmartScroll.TraceOut('NUIScrollbar was not found.');
		}
	}
	
	if (x != 0) {
		if (x < 0) {
			SendHScroll(w, SB_LINELEFT, 0, hs);
		} else if (x > 0) {
			SendHScroll(w, SB_LINERIGHT, 0, hs);
		}
	}
	if (y != 0) {
		//SendWheelInput(0, -y * 30);
		if (y < 0) {
			SendVScroll(w, SB_LINEUP, 0, vs);
		} else if (y > 0) {
			SendVScroll(w, SB_LINEDOWN, 0, vs);
		}
	}
}

false;
