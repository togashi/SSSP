
function onLoad() {
	//SmartScroll.TraceOut('onLoad');
	SSSP.SetTitle('[S]: Paint.NET');
	SSSP.SetWheelDefault(false);
	SSSP.SetScrollDefault(false);
	SSSP.SetScrollParams(20, 2000);
	SSSP.SetWheelParams(3, 500);
	updateButtons();
}

function isApplicable(window) {
	//SmartScroll.TraceOut('isApplicable');
	fn = GetWindowFilename(window);
	if (fn.match(/.+\\paintdotnet.exe$/i)) {
		wt = GetWindowText(window);
		if (wt.match(/.* - Paint.NET V\d+\.\d+\.\d+$/)) {
			return true;
		}
	}
	
	return false;
}

function onScroll(window, lx, ly, d) {
	if (ly != 0) {
		SendMouseWheel(window, 0, -ly * WHEEL_DELTA / 4);
	}
	if (lx != 0) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_LSHIFT },
			{ type:INPUT_MOUSE, mouseData:(-lx * WHEEL_DELTA / 4), dwFlags:MOUSEEVENTF_WHEEL },
			{ type:INPUT_KEYBOARD, wVk:VK_LSHIFT, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
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

var zoom_in = {
	text:'�\��:�Y�[���C��',
	icon:'paintdotnet.png,41',
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
	text:'�\��:�Y�[���A�E�g',
	icon:'paintdotnet.png,42',
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
	text:'�\��:���ۂ̃T�C�Y',
	icon:'paintdotnet.png,45',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT },
			{ type:INPUT_KEYBOARD, wVk:VK_A },
			{ type:INPUT_KEYBOARD, wVk:VK_A, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_SHIFT, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_select = {
	text:'�c�[��:�I��',
	icon:'paintdotnet.png,241',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_S },
			{ type:INPUT_KEYBOARD, wVk:VK_S, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_move = {
	text:'�c�[��:�ړ�',
	icon:'paintdotnet.png,242',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_M },
			{ type:INPUT_KEYBOARD, wVk:VK_M, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_zoom = {
	text:'�c�[��:�Y�[��',
	icon:'paintdotnet.png,246',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_Z },
			{ type:INPUT_KEYBOARD, wVk:VK_Z, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_pan = {
	text:'�c�[��:�p��',
	icon:'paintdotnet.png,248',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_H },
			{ type:INPUT_KEYBOARD, wVk:VK_H, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_ffill = {
	text:'�c�[��:�y�C���g��',
	icon:'paintdotnet.png,249',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F },
			{ type:INPUT_KEYBOARD, wVk:VK_F, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_grad = {
	text:'�c�[��:�O���f�[�V����',
	icon:'paintdotnet.png,250',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_G },
			{ type:INPUT_KEYBOARD, wVk:VK_G, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_erase = {
	text:'�c�[��:�����S��',
	icon:'paintdotnet.png,261',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_E },
			{ type:INPUT_KEYBOARD, wVk:VK_E, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_brush = {
	text:'�c�[��:�y�C���g�u���V',
	icon:'paintdotnet.png,251',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_B },
			{ type:INPUT_KEYBOARD, wVk:VK_B, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_pencil = {
	text:'�c�[��:���M',
	icon:'paintdotnet.png,262',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_P },
			{ type:INPUT_KEYBOARD, wVk:VK_P, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_colpick = {
	text:'�c�[��:�F�̑I��',
	icon:'paintdotnet.png,263',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_K },
			{ type:INPUT_KEYBOARD, wVk:VK_K, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_stamp = {
	text:'�c�[��:�����X�^���v',
	icon:'paintdotnet.png,264',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_L },
			{ type:INPUT_KEYBOARD, wVk:VK_L, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_repaint = {
	text:'�c�[��:�h�蒼��',
	icon:'paintdotnet.png,265',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_R },
			{ type:INPUT_KEYBOARD, wVk:VK_R, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_text = {
	text:'�c�[��:�e�L�X�g',
	icon:'paintdotnet.png,266',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_T },
			{ type:INPUT_KEYBOARD, wVk:VK_T, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var tool_obj = {
	text:'�c�[��:�}�`',
	icon:'paintdotnet.png,267',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_O },
			{ type:INPUT_KEYBOARD, wVk:VK_O, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var hist_undo = {
	text:'����:������',
	icon:'paintdotnet.png,21',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_Z },
			{ type:INPUT_KEYBOARD, wVk:VK_Z, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var hist_redo = {
	text:'����:��蒼��',
	icon:'paintdotnet.png,22',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_Y },
			{ type:INPUT_KEYBOARD, wVk:VK_Y, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var file_save = {
	text:'�t�@�C��:�ۑ�',
	icon:'paintdotnet.png,4',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_S },
			{ type:INPUT_KEYBOARD, wVk:VK_S, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var wnd_tool = {
	text:'�E�B���h�E:�c�[��',
	icon:'paintdotnet.png,201',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F5 },
			{ type:INPUT_KEYBOARD, wVk:VK_F5, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var wnd_hist = {
	text:'�E�B���h�E:����',
	icon:'paintdotnet.png,202',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F6 },
			{ type:INPUT_KEYBOARD, wVk:VK_F6, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var wnd_layer = {
	text:'�E�B���h�E:���C���[',
	icon:'paintdotnet.png,203',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F7 },
			{ type:INPUT_KEYBOARD, wVk:VK_F7, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var wnd_color = {
	text:'�E�B���h�E:�F',
	icon:'paintdotnet.png,204',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F8 },
			{ type:INPUT_KEYBOARD, wVk:VK_F8, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var wnd_tool_ex = {
	text:'�E�B���h�E:�c�[��',
	icon:'paintdotnet.png,201',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F5 },
			{ type:INPUT_KEYBOARD, wVk:VK_F5, dwFlags:KEYEVENTF_KEYUP }
		]);
		Sleep(200);
		wpal = FindWindow('WindowsForms10.Window.8.app.0.378734a', '�c�[��');
		if (wpal != 0) {
			if (GetWindowState(wpal) != SW_HIDE) {
				cp = GetCursorPos();
				wr = GetWindowRect(wpal);
				SetWindowPos(
					wpal,
					HWND_TOP,
					cp.x - (wr.right - wr.left) / 2,
					cp.y - (wr.bottom - wr.top) / 4,
					0, 0,
					SWP_NOSIZE
				);
			}
		}
	}
};

var wnd_hist_ex = {
	text:'�E�B���h�E:����',
	icon:'paintdotnet.png,202',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F6 },
			{ type:INPUT_KEYBOARD, wVk:VK_F6, dwFlags:KEYEVENTF_KEYUP }
		]);
		Sleep(200);
		wpal = FindWindow('WindowsForms10.Window.8.app.0.378734a', '����');
		if (wpal != 0) {
			if (GetWindowState(wpal) != SW_HIDE) {
				cp = GetCursorPos();
				wr = GetWindowRect(wpal);
				SetWindowPos(
					wpal,
					HWND_TOP,
					cp.x - (wr.right - wr.left) / 2,
					cp.y - (wr.bottom - wr.top) / 4,
					0, 0,
					SWP_NOSIZE
				);
			}
		}
	}
};

var wnd_layer_ex = {
	text:'�E�B���h�E:���C���[',
	icon:'paintdotnet.png,203',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F7 },
			{ type:INPUT_KEYBOARD, wVk:VK_F7, dwFlags:KEYEVENTF_KEYUP }
		]);
		Sleep(200);
		wpal = FindWindow('WindowsForms10.Window.8.app.0.378734a', '���C���[');
		if (wpal != 0) {
			if (GetWindowState(wpal) != SW_HIDE) {
				cp = GetCursorPos();
				wr = GetWindowRect(wpal);
				SetWindowPos(
					wpal,
					HWND_TOP,
					cp.x - (wr.right - wr.left) / 2,
					cp.y - (wr.bottom - wr.top) / 4,
					0, 0,
					SWP_NOSIZE
				);
			}
		}
	}
};

var wnd_color_ex = {
	text:'�E�B���h�E:�F',
	icon:'paintdotnet.png,204',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_F8 },
			{ type:INPUT_KEYBOARD, wVk:VK_F8, dwFlags:KEYEVENTF_KEYUP }
		]);
		Sleep(200);
		wpal = FindWindow('WindowsForms10.Window.8.app.0.378734a', '�F');
		if (wpal != 0) {
			if (GetWindowState(wpal) != SW_HIDE) {
				cp = GetCursorPos();
				wr = GetWindowRect(wpal);
				SetWindowPos(
					wpal,
					HWND_TOP,
					cp.x - (wr.right - wr.left) / 2,
					cp.y - (wr.bottom - wr.top) / 4,
					0, 0,
					SWP_NOSIZE
				);
			}
		}
	}
};

var edit_copy = {
	text:'�ҏW:�R�s�[',
	icon:'paintdotnet.png,24',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_C },
			{ type:INPUT_KEYBOARD, wVk:VK_C, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var edit_paste = {
	text:'�ҏW:�\��t��',
	icon:'paintdotnet.png,25',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_P },
			{ type:INPUT_KEYBOARD, wVk:VK_P, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var select_all = {
	text:'�ҏW:���ׂđI��',
	icon:'paintdotnet.png,31',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_A },
			{ type:INPUT_KEYBOARD, wVk:VK_A, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var select_inverse = {
	text:'�ҏW:�I��͈͂𔽓]',
	icon:'paintdotnet.png,30',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_I },
			{ type:INPUT_KEYBOARD, wVk:VK_I, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var select_del = {
	text:'�ҏW:�I��͈͂̃s�N�Z�����폜',
	icon:'paintdotnet.png,28',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_DELETE },
			{ type:INPUT_KEYBOARD, wVk:VK_DELETE, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var select_discard = {
	text:'�ҏW:�I������',
	icon:'paintdotnet.png,32',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_D },
			{ type:INPUT_KEYBOARD, wVk:VK_D, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var img_next = {
	text:'�E�B���h�E:���̃C���[�W',
	icon:'paintdotnet.png,206',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

var img_prev = {
	text:'�E�B���h�E:�O�̃C���[�W',
	icon:'paintdotnet.png,207',
	exec:function (w) {
		SendInput([
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL },
			{ type:INPUT_KEYBOARD, wVk:VK_LSHIFT },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB },
			{ type:INPUT_KEYBOARD, wVk:VK_TAB, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_LSHIFT, dwFlags:KEYEVENTF_KEYUP },
			{ type:INPUT_KEYBOARD, wVk:VK_CONTROL, dwFlags:KEYEVENTF_KEYUP }
		]);
	}
};

////////////////////////////////////////////////////////////////////////////////
//
// �@�\���蓖��
//
////////////////////////////////////////////////////////////////////////////////

var clickActions = [
	noop,			wnd_tool_ex,	img_prev,			img_next,
	noop,			wnd_color_ex,	tool_select,		tool_move,
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
	zoom_in,
	zoom_out
];
