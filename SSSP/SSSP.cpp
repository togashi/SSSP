// SSSP.cpp : DLL �G�N�X�|�[�g�̎����ł��B


#include "stdafx.h"

#include "sspscript.h"
#include "sspscripts.h"

CSSPScripts g_Scripts;

///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
//  �v���O�C���t�@�C�����h���C�o�ɂ�胍�[�h���ꂽ���ɌĂ΂��֐��ł�
// < �p�����[�^ >
//	pParam->dwModel		: SmartScroll�̃��f���ԍ�
//	pParam->dwVersion	: SmartScroll�h���C�o�̃o�[�W����
// < �߂�l >
// SSP_TRUE		: ���̃v���O�C���͏풓����܂�
// SSP_FALSE	: ���̃v���O�C���̏풓�͉�������܂�
SSP_STATUS WINAPI SspLoadPlugin(PLOADPARAM pParam)
{
	g_Scripts.AddSearchPath(CSIDL_APPDATA, _T("SmartScroll SSSP"));
	return SSP_TRUE;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
//  �t�H�A�O���E���h�E�B���h�E�ɑ΂��āA���̃v���O�C����K�p���邩�ǂ����𔻒f����֐��ł�
// < �p�����[�^ >
// hWnd		: �t�H�A�O���E���h�E�B���h�E�̃n���h��
// < �߂�l >
// SSP_TRUE		: ���̃v���O�C�����K�p�����
// SSP_FALSE	: ���̃v���O�C���͓K�p����Ȃ�
SSP_STATUS WINAPI SspApplyProc(HWND hWnd)
{
	SSP_STATUS ret;
	g_Scripts.Call_ApplyProc(hWnd, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �v���O�C�����K�p����鎞�A�ŏ��ɌĂяo�����֐��ł�
// < �p�����[�^ >
//	pParam->hwndApp		: �t�H�A�O���E���h�E�B���h�E�̃n���h��
//	pParam->dwModel		: SmartScroll�̃��f���ԍ�
//	pParam->dwVersion	: SmartScroll�h���C�o�̃o�[�W����
// < �߂�l >
// SSP_TRUE		: �ʏ�͂��̒l��Ԃ��Ă�������
// SSP_FALSE	: �v���O�C���̓K�p����������܂�
// SSP_TIMER    : SspTimerProc�֐����g�p����ꍇ�́ASSP_TIMER��߂�l�Ƃ��ĕԂ�
SSP_STATUS WINAPI SspInitProc(PINITPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_InitProc(pParam, ret);
	return SSP_TIMER;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// ��100ms�Ԋu�ŌĂяo�����^�C�}�֐��ł��B
//�i�h���C�o�o�[�W����1.20�ȍ~�őΉ��j
void WINAPI SspTimerProc(void)
{
	g_Scripts.Call_TimerProc();
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �v���O�C���̓K�p���I�����鎞�ɌĂяo�����֐��ł�
void WINAPI SspReleaseProc(void)
{
	g_Scripts.Call_ReleaseProc();
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �t�@���N�V�����L�[�����삳�ꂽ�ꍇ�̏������s���֐��ł�
// < �p�����[�^ >
//	pParam->hwndMouse	: �}�E�X�J�[�\���̐^���ɂ���E�B���h�E�̃n���h��
//	pParam->dwType		: �C�x���g�^�C�v
// < �߂�l >
// SSP_TRUE		: �ʏ�͂��̒l��Ԃ��Ă�������
// SSP_SHOW_HIDE: SmartScroll�E�B���h�E�̕\���^��\�����؂�ւ��܂�
SSP_STATUS WINAPI SspButtonProc(PBUTTONPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_ButtonProc(pParam, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �Y�[���z�C�[���@�\�̗L���^�����𔻒f����֐��ł�
// < �߂�l >
// SSP_TRUE		: �Y�[���z�C�[���͗L���ł�
// SSP_FALSE	: �Y�[���z�C�[���͖����ł�
SSP_STATUS WINAPI SspGetWheelEffect(void)
{
	SSP_STATUS ret;
	g_Scripts.Call_GetWheelEffect(ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �Y�[���z�C�[�������삳�ꂽ�ꍇ�̏������s���֐��ł�
// < �p�����[�^ >
//	pParam->hwndMouse	: �}�E�X�J�[�\���̐^���ɂ���E�B���h�E�̃n���h��
//	pParam->lRoll		: �Y�[���z�C�[���̉�]��
//	pParam->dwDirection	: ��]�����t���O
// < �߂�l >
// SSP_TRUE		: �f�t�H���g�̃Y�[�������͍s���܂���
// SSP_FALSE	: �f�t�H���g�̃Y�[���������s���܂�
SSP_STATUS WINAPI SspWheelProc(PWHEELPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_WheelProc(pParam, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �X�N���[���{�[�������삳�ꂽ�ꍇ�̏������s���֐��ł�
// < �p�����[�^ >
//	pParam->hwndMouse	: �}�E�X�J�[�\���̐^���ɂ���E�B���h�E�̃n���h��
//	pParam->hwndOwner	: hwndMouse�̐e�E�B���h�E�̃n���h��
//	pParam->hwndHScroll	: ���X�N���[���o�[�R���g���[���̃n���h��
//	pParam->hwndVScroll	: �c�X�N���[���o�[�R���g���[���̃n���h��
//	pParam->lX			: �X�N���[���{�[����X�����̉�]��
//	pParam->lY			: �X�N���[���{�[����Y�����̉�]��
//	pParam->dwDirection	: ��]�����t���O
// < �߂�l >
// SSP_TRUE		: �f�t�H���g�̃X�N���[�������͍s���܂���
// SSP_FALSE	: �f�t�H���g�̃X�N���[���������s���܂�
SSP_STATUS WINAPI SspScrollProc(PSCROLLPARAM pParam)
{
	SSP_STATUS ret;
	g_Scripts.Call_ScrollProc(pParam, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// SmartScroll�p�b�h�̃{�^���ɕ\�������A�C�R���n���h����񋟂���֐��ł�
// < �p�����[�^ >
//	dwType	: �{�^���̎��
// < �߂�l >
// �e�{�^���p�̃A�C�R���n���h����Ԃ��܂�
HICON WINAPI SspGetIcon(DWORD dwType)
{
	HICON ret;
	g_Scripts.Call_GetIcon(dwType, ret);
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �v���O�C�����������񃊃\�[�X��񋟂���֐��ł�
// < �p�����[�^ >
//	dwType	: ���\�[�X�̎��
//	lpStr	: ��������󂯎��o�b�t�@���w���|�C���^
//	uiSize	: �o�b�t�@�̃T�C�Y
// < �߂�l >
// �o�b�t�@�ɃR�s�[���ꂽ��������Ԃ��܂�
int WINAPI SspGetString(DWORD dwType, LPSTR lpStr, UINT uiSize)
{
	int ret;
	if (dwType == SSP_NAME) {
		::strncpy_s(lpStr, uiSize, "�X�N���v�g�v���O�C��", uiSize - 1);
		ret = ::strlen(lpStr);
	} else {
		g_Scripts.Call_GetString(dwType, lpStr, uiSize, ret);
	}
	return ret;
}


///////////////////////////////////////////////////////////////////////////////////
// < �@�\ >
// �ݒ�_�C�A���O�p�̃_�C�A���O�v���V�[�W���ł�
BOOL CALLBACK SspDialogProc(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	::EndDialog(hWnd, 0);
	return TRUE;
}
