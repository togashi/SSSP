// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

#pragma once

#ifndef STRICT
#define STRICT
#endif

#include "targetver.h"

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �ꕔ�� CString �R���X�g���N�^�͖����I�ł��B

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <atlctl.h>
#include <atlstr.h>
#include <atltypes.h>
#include <atlcoll.h>
#include <atlpath.h>
#include <atlsimpcoll.h>
#include <atlfile.h>
//#include <atlrx.h>

using namespace ATL;

#include <comdef.h>
#include <initguid.h>
#include <activscp.h>
#include <shlobj.h>
#include <dispex.h>
_COM_SMARTPTR_TYPEDEF(IDispatchEx, IID_IDispatchEx);

// SSP_SDK
#include "common.h"
#include "sspfunc.h"

// utils
#include "dispatchutils.h"

#define ReleaseAll(pIUnk) for (ULONG rc = (pIUnk)->Release(); rc > 0; rc = (pIUnk)->Release())
