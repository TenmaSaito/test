//================================================================================================================
//
// DirectX�̓��͏����w�b�_�t�@�C�� [input.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _INPUT_H_			// ���̃}�N����`������Ă��Ȃ����
#define _INPUT_H_			// 2�d�C���N���[�h�h�~�̃}�N�����`

#include "main.h"

// �}�N����`
#define REPEAT_COUNT		(60)		// ���s�[�g�����ŁA�g���K�[����v���X�Ɉڍs����܂ł̎���
#define MAX_VIBRATION		(60000)		// �W���C�p�b�h�̃o�C�u���[�V�����̍ő�l(�I�[�o�[��h�����߂�-5000�����l)

// �W���C�p�b�h�̃L�[�̎��
typedef enum
{
	JOYKEY_UP = 0,			// ����
	JOYKEY_DOWN,			// �����
	JOYKEY_LEFT,			// �����
	JOYKEY_RIGHT,			// �E���
	JOYKEY_START,			// start
	JOYKEY_BACK,			// back
	JOYKEY_LEFT_THUMB,		// L�X�e�B�b�N��������
	JOYKEY_RIGHT_THUMB,		// R�X�e�B�b�N��������
	JOYKEY_LEFT_SHOULDER,	// L
	JOYKEY_RIGHT_SHOULDER,	// R
	JOYKEY_NONE_STATE_1,
	JOYKEY_NONE_STATE_2,
	JOYKEY_A,				// A
	JOYKEY_B,				// B
	JOYKEY_X,				// X
	JOYKEY_Y,				// Y
	JOYKEY_MAX
}JOYKEY;

// �X�e�B�b�N�̎��
typedef enum
{
	JOYTHUMB_LX_UP = 0,		// ���X�e�B�b�N��X���� +
	JOYTHUMB_LX_DOWN,		// ���X�e�B�b�N��X���� -
	JOYTHUMB_LY_UP,			// ���X�e�B�b�N��Y���� +
	JOYTHUMB_LY_DOWN,		// ���X�e�B�b�N��Y���� -
	JOYTHUMB_RX_UP,			// �E�X�e�B�b�N��X���� +
	JOYTHUMB_RX_DOWN,		// �E�X�e�B�b�N��X���� -
	JOYTHUMB_RY_UP,			// �E�X�e�B�b�N��Y���� +
	JOYTHUMB_RY_DOWN,		// �E�X�e�B�b�N��Y���� -
	JOYTHUMB_MAX
}JOYTHUMB;

// �}�E�X�̃{�^���̎��
typedef enum
{
	MOUSEKEY_LEFT = 0,		// ���N���b�N
	MOUSEKEY_RIGHT,			// �E�N���b�N
	MOUSEKEY_WHEEL,			// ���N���b�N
	MOUSEKEY_MAX
}MOUSEKEY;

//�v���g�^�C�v�錾

//***************************************************
// �L�[�{�[�h�̃v���g�^�C�v�錾
//***************************************************
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
bool GetKeyboardRelease(int nKey);
bool GetKeyboardRepeat(int nKey);
bool GetKeyboardAny(void);
bool GetKeyboardWASD(void);

//***************************************************
// �W���C�p�b�h�̃v���g�^�C�v�錾
//***************************************************
HRESULT InitJoypad(void);
void UninitJoypad(void);
void UpdateJoypad(void);
bool GetJoypadPress(JOYKEY Key);
bool GetJoypadTrigger(JOYKEY Key);
bool GetJoypadRelease(JOYKEY Key);
bool GetJoypadRepeat(JOYKEY Key);
bool GetJoypadAny(void);
bool GetJoypadWASD(void);

XINPUT_VIBRATION *GetJoyVibration(void);
XINPUT_STATE *GetJoypadState(void);
bool GetJoyThumbLXState(void);
bool GetJoyThumbLYState(void);
bool GetJoyThumbRXState(void);
bool GetJoyThumbRYState(void);
bool GetJoyThumbSlow(JOYTHUMB Thumb);
bool GetJoyThumbRepeat(JOYTHUMB Thumb);
bool GetJoyThumbWASD(void);

//***************************************************
// �}�E�X�̃v���g�^�C�v�錾
//***************************************************
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);
bool GetMousePress(int nButton);
bool GetMouseTrigger(int nButton);
bool GetMouseRelease(int nButton);
POINT GetMousePos(void);

#endif