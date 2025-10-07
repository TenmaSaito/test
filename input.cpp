//================================================================================================================
//
// DirectX�̓��͏��� [input.cpp]
// Author : TENMA
//
//================================================================================================================
#include "input.h"

// �}�N����`
#define NUM_KEY_MAX			(256)			// �L�[�̍ő吔
#define NUM_MOUSE_MAX		(3)				// �{�^���̍ő吔
#define THUMB_SLOW			(12)			// GetJoyThumbSlow�ł̒x�����鐔
#define STROKE_KEY_MAX		(288)			// �L�[�y�у{�^���A�X�e�B�b�N�̍ő吔
#define REPEAT_TIME			(30)			// ���s�[�g�̑ҋ@����
#define UP_DEADZONE			(5000)			// �l�����̎��̃f�b�h�]�[��
#define DOWN_DEADZONE		(-5000)			// �l�����̎��̃f�b�h�]�[��

// �O���[�o���ϐ�
LPDIRECTINPUT8 g_pInput = NULL;					// DirectInput�I�u�W�F�N�g�̃|�C���^
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;		// ���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
BYTE g_aKeyState[NUM_KEY_MAX];					// �L�[�{�[�h�̃v���X���
BYTE g_aKeyStateTrigger[NUM_KEY_MAX];			// �L�[�{�[�h�̃g���K�[���
BYTE g_aKeyStateRelease[NUM_KEY_MAX];			// �L�[�{�[�h�̃����[�X����
int g_nCounterRepeat[NUM_KEY_MAX] = {};			// ���s�[�g�̃J�E���g
bool g_bUseKeyAny;								// �L�[�{�[�h�̉������

XINPUT_STATE g_joykeyState;						// �W���C�p�b�h�̃v���X���
XINPUT_STATE g_joykeyStateTrigger;				// �W���C�p�b�h�̃g���K�[���
XINPUT_STATE g_joykeyStateRelease;				// �W���C�p�b�h�̃����[�X���
int g_nCounterRepeatJoypad[JOYKEY_MAX] = {};	// �W���C�p�b�h�̃��s�[�g�̃J�E���g
bool g_bUseJoykeyAny;							// �W���C�L�[�̉������
int g_nCounterJoyThumbRepeat[JOYTHUMB_MAX] = {};// �W���C�p�b�h�X�e�B�b�N�̃��s�[�g�J�E���g
int g_nCounterJoyThumb[JOYTHUMB_MAX] = {};		// �W���C�p�b�h�̃X�e�B�b�N�̒x���J�E���g
XINPUT_VIBRATION g_joyVibration;				// �W���C�p�b�h�̃o�C�u���[�V�������
_XINPUT_KEYSTROKE g_keyStroke;					// ���z�L�[�̃X�g���[�N���
int g_aStroke[STROKE_KEY_MAX];					// �X�g���[�N�J�E���g

LPDIRECTINPUTDEVICE8 g_pDevMouse = NULL;		// ���̓f�o�C�X(�}�E�X)�ւ̃|�C���^

DIMOUSESTATE g_CurrentMouseState;				// �}�E�X�̓��͏��
DIMOUSESTATE g_PrevMouseState;					// �}�E�X�̉ߋ��̓��͏��
BYTE g_aMouseState[NUM_MOUSE_MAX];				// �}�E�X�̃v���X���
BYTE g_aMouseStateTrigger[NUM_MOUSE_MAX];		// �}�E�X�̃g���K�[���
BYTE g_aMouseStateRelease[NUM_MOUSE_MAX];		// �}�E�X�̃����[�X���
POINT g_mousePos = {};							// �}�E�X�̈ʒu

HWND g_InputhWnd = NULL;						// ���W�ϊ��Ɏg�p����E�B���h�E�̃n���h��

//================================================================================================================
//�L�[�{�[�h�̏���������
//================================================================================================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance,
								  DIRECTINPUT_VERSION,
								  IID_IDirectInput8,
								  (void**)&g_pInput,
								  NULL)))
	{
		return E_FAIL;
	}

	// ���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard,
									  &g_pDevKeyboard,
									  NULL)))
	{
		return E_FAIL;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd,
												   (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	// �E�B���h�E�n���h����ۑ�
	g_InputhWnd = hWnd;

	return S_OK;
}

//================================================================================================================
//�L�[�{�[�h�̏I������
//================================================================================================================
void UninitKeyboard(void)
{
	// ���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}

	// DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//================================================================================================================
//�L�[�{�[�h�̍X�V����
//================================================================================================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];			// �L�[�{�[�h�̓��͏��
	int nCntKey;
	g_bUseKeyAny = false;

	// ���̓f�o�C�X����f�[�^���擾
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyStateRelease[nCntKey] = (g_aKeyState[nCntKey] & (g_aKeyState[nCntKey] ^ aKeyState[nCntKey]));			// �L�[�{�[�h�̃����[�X����ۑ�
			g_aKeyStateTrigger[nCntKey] = ((aKeyState[nCntKey] ^ g_aKeyState[nCntKey]) & aKeyState[nCntKey]);			// �L�[�{�[�h�̃g���K�[����ۑ�
			if (g_aKeyState[nCntKey] != aKeyState[nCntKey])
			{
				g_nCounterRepeat[nCntKey] = 0;
			}
			g_aKeyState[nCntKey] = aKeyState[nCntKey];																	// �L�[�{�[�h�̃v���X����ۑ�
			if (g_aKeyState[nCntKey] & 0x80)
			{
				g_bUseKeyAny = true;
			}
		}			
	}
	else
	{
		g_pDevKeyboard->Acquire();			// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
	}
}                                              

//================================================================================================================
// �L�[�{�[�h�̃v���X�����擾
//================================================================================================================
bool GetKeyboardPress(int nKey)
{
	// 3�����Z�q
	return (g_aKeyState[nKey] & 0x80) ? true : false;
}

//================================================================================================================
// �L�[�{�[�h�̃g���K�[�����擾
//================================================================================================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//================================================================================================================
// �L�[�{�[�h�̃����[�X�����擾
//================================================================================================================
bool GetKeyboardRelease(int nKey)
{
	return (g_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//================================================================================================================
// �L�[�{�[�h�̃��s�[�g�����擾
//================================================================================================================
bool GetKeyboardRepeat(int nKey)
{
	g_nCounterRepeat[nKey]++;
	if (g_nCounterRepeat[nKey] < 30)
	{
		return (g_aKeyStateTrigger[nKey] & 0x80) ? true : false;
	}
	else
	{
		return (g_aKeyState[nKey] & 0x80) ? true : false;
	}
}

//================================================================================================================
// �L�[�{�[�h�̉��������擾
//================================================================================================================
bool GetKeyboardAny(void)
{
	return g_bUseKeyAny;
}

//================================================================================================================
// �L�[�{�[�h�̊�{������(WASD)���擾
//================================================================================================================
bool GetKeyboardWASD(void)
{
	if ((g_aKeyState[DIK_W] & 0x80))
	{
		return true;
	}
	else if ((g_aKeyState[DIK_A] & 0x80))
	{
		return true;
	}
	else if ((g_aKeyState[DIK_S] & 0x80))
	{
		return true;
	}
	else if ((g_aKeyState[DIK_D] & 0x80))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//================================================================================================================
// �W���C�p�b�h�̏���������
//================================================================================================================
HRESULT InitJoypad(void)
{
	// �������̃N���A
	memset(&g_joykeyState, 0, sizeof(XINPUT_STATE));

	memset(&g_joyVibration, 0, sizeof(XINPUT_VIBRATION));

	// XInput�̃X�e�[�g�ݒ�(�L��)
	XInputEnable(true);

	return S_OK;
}

//================================================================================================================
// �W���C�p�b�h�̏I������
//================================================================================================================
void UninitJoypad(void)
{
	// XInput�̃X�e�[�g�ݒ�(����)
	XInputEnable(false);
}

//================================================================================================================
// �W���C�p�b�h�̍X�V����
//================================================================================================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;			// ���͏��
	g_bUseJoykeyAny = false;

	// �W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		if (g_joykeyState.Gamepad.sThumbLX <= 100.0f && g_joykeyState.Gamepad.sThumbLX >= -100.0f)
		{
			g_nCounterJoyThumb[JOYTHUMB_LX_UP] = 0;
			g_nCounterJoyThumb[JOYTHUMB_LX_DOWN] = 0;
		}

		if (g_joykeyState.Gamepad.sThumbLY <= 100.0f && g_joykeyState.Gamepad.sThumbLY >= -100.0f)
		{
			g_nCounterJoyThumb[JOYTHUMB_LY_UP] = 0;
			g_nCounterJoyThumb[JOYTHUMB_LY_DOWN] = 0;
		}

		if (g_joykeyState.Gamepad.sThumbRX <= 100.0f && g_joykeyState.Gamepad.sThumbRX >= -100.0f)
		{
			g_nCounterJoyThumb[JOYTHUMB_RX_UP] = 0;
			g_nCounterJoyThumb[JOYTHUMB_RX_DOWN] = 0;
		}

		if (g_joykeyState.Gamepad.sThumbRY <= 100.0f && g_joykeyState.Gamepad.sThumbRY >= -100.0f)
		{
			g_nCounterJoyThumb[JOYTHUMB_RY_UP] = 0;
			g_nCounterJoyThumb[JOYTHUMB_RY_DOWN] = 0;
		}

		g_joykeyStateTrigger.Gamepad.wButtons = ((joykeyState.Gamepad.wButtons ^ g_joykeyState.Gamepad.wButtons) & joykeyState.Gamepad.wButtons);
		g_joykeyStateRelease.Gamepad.wButtons = (g_joykeyState.Gamepad.wButtons & (g_joykeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons));
		g_joykeyState = joykeyState;	// �v���X����ۑ�
		
		for (int nCntJoykey = 0; nCntJoykey < JOYKEY_MAX; nCntJoykey++)
		{
			if ((g_joykeyState.Gamepad.wButtons & (0x01 << nCntJoykey)) == false)
			{
				g_nCounterRepeatJoypad[nCntJoykey] = 0;
			}
			else
			{
				g_bUseJoykeyAny = true;
			}
		}

		
		if (g_joykeyState.Gamepad.sThumbLX >= DOWN_DEADZONE && g_joykeyState.Gamepad.sThumbLX <= UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[JOYTHUMB_LX_UP] = 0;
			g_nCounterJoyThumbRepeat[JOYTHUMB_LX_DOWN] = 0;
		}
		
		if (g_joykeyState.Gamepad.sThumbLY >= DOWN_DEADZONE && g_joykeyState.Gamepad.sThumbLY <= UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[JOYTHUMB_LY_UP] = 0;
			g_nCounterJoyThumbRepeat[JOYTHUMB_LY_DOWN] = 0;
		}
		
		if (g_joykeyState.Gamepad.sThumbRX >= DOWN_DEADZONE && g_joykeyState.Gamepad.sThumbRX <= UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[JOYTHUMB_RX_UP] = 0;
			g_nCounterJoyThumbRepeat[JOYTHUMB_RX_DOWN] = 0;
		}
		
		if (g_joykeyState.Gamepad.sThumbRY >= DOWN_DEADZONE && g_joykeyState.Gamepad.sThumbRY <= UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[JOYTHUMB_RY_UP] = 0;
			g_nCounterJoyThumbRepeat[JOYTHUMB_RY_DOWN] = 0;
		}
		
	}
}

//================================================================================================================
// �W���C�p�b�h�̃v���X�����擾
//================================================================================================================
bool GetJoypadPress(JOYKEY Key)
{
	return (g_joykeyState.Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//================================================================================================================
// �W���C�p�b�h�̃g���K�[�����擾
//================================================================================================================
bool GetJoypadTrigger(JOYKEY Key)
{
	return (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//================================================================================================================
// �W���C�p�b�h�̃����[�X�����擾
//================================================================================================================
bool GetJoypadRelease(JOYKEY Key)
{
	return (g_joykeyStateRelease.Gamepad.wButtons & (0x01 << Key)) ? true : false;
}

//================================================================================================================
// �W���C�p�b�h�̃��s�[�g�����擾
//================================================================================================================
bool GetJoypadRepeat(JOYKEY Key)
{
	g_nCounterRepeatJoypad[Key]++;
	if (g_nCounterRepeatJoypad[Key] <= 30)
	{
		return (g_joykeyStateTrigger.Gamepad.wButtons & (0x01 << Key)) ? true : false;
	}
	else
	{
		return (g_joykeyState.Gamepad.wButtons & (0x01 << Key)) ? true : false;
	}
}

//================================================================================================================
// �W���C�p�b�h�̉��������擾
//===============================================================================================================
bool GetJoypadAny(void)
{
	return g_bUseJoykeyAny;
}

//================================================================================================================
// �W���C�p�b�h�̊�{������(WASD)���擾
//================================================================================================================
bool GetJoypadWASD(void)
{
	if (g_joykeyState.Gamepad.wButtons & (0x01 << JOYKEY_UP))
	{
		return true;
	}
	else if (g_joykeyState.Gamepad.wButtons & (0x01 << JOYKEY_DOWN))
	{
		return true;
	}
	else if (g_joykeyState.Gamepad.wButtons & (0x01 << JOYKEY_LEFT))
	{
		return true;
	}
	else if (g_joykeyState.Gamepad.wButtons & (0x01 << JOYKEY_RIGHT))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//================================================================================================================
// �W���C�p�b�h�̃o�C�u���[�V�����̃|�C���^���擾
//================================================================================================================
XINPUT_VIBRATION *GetJoyVibration(void)
{
	return &g_joyVibration;
}

//================================================================================================================
// �W���C�p�b�h�̃|�C���^���擾
//================================================================================================================
XINPUT_STATE *GetJoypadState(void)
{
	return &g_joykeyState;
}

//================================================================================================================
// �W���C�p�b�h�̍��X�e�B�b�N[X����]�̔���
//================================================================================================================
bool GetJoyThumbLXState(void)
{
	return (g_joykeyState.Gamepad.sThumbLX != 0) ? true : false;
}

//================================================================================================================
// �W���C�p�b�h�̍��X�e�B�b�N[Y����]�̔���
//================================================================================================================
bool GetJoyThumbLYState(void)
{
	return (g_joykeyState.Gamepad.sThumbLY != 0) ? true : false;
}

//================================================================================================================
// �W���C�p�b�h�̉E�X�e�B�b�N[X����]�̔���
//================================================================================================================
bool GetJoyThumbRXState(void)
{
	return (g_joykeyState.Gamepad.sThumbRX != 0) ? true : false;
}

//================================================================================================================
// �W���C�p�b�h�̉E�X�e�B�b�N[Y����]�̔���
//================================================================================================================
bool GetJoyThumbRYState(void)
{
	return (g_joykeyState.Gamepad.sThumbRY != 0) ? true : false;
}

//================================================================================================================
// �W���C�p�b�h�̃X�e�B�b�N�̓��͒x��
//================================================================================================================
bool GetJoyThumbSlow(JOYTHUMB Thumb)
{
	g_nCounterJoyThumb[Thumb]++;

	switch (Thumb)
	{
	case JOYTHUMB_LX_UP:
		if (g_joykeyState.Gamepad.sThumbLX > 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	case JOYTHUMB_LX_DOWN:
		if (g_joykeyState.Gamepad.sThumbLX < 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	case JOYTHUMB_LY_UP:
		if (g_joykeyState.Gamepad.sThumbLY > 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	case JOYTHUMB_LY_DOWN:
		if (g_joykeyState.Gamepad.sThumbLY < 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	case JOYTHUMB_RX_UP:
		if (g_joykeyState.Gamepad.sThumbRX > 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	case JOYTHUMB_RX_DOWN:
		if (g_joykeyState.Gamepad.sThumbRX < 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	case JOYTHUMB_RY_UP:
		if (g_joykeyState.Gamepad.sThumbRY > 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	case JOYTHUMB_RY_DOWN:
		if (g_joykeyState.Gamepad.sThumbRY < 0
			&& (g_nCounterJoyThumb[Thumb] % THUMB_SLOW) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}

		break;

	default:

		return false;

		break;
	}
}

//================================================================================================================
// �W���C�p�b�h�̃X�e�B�b�N�̃��s�[�g����
//================================================================================================================
bool GetJoyThumbRepeat(JOYTHUMB Thumb)
{
	switch (Thumb)
	{
	case JOYTHUMB_LX_UP:
		if (g_joykeyState.Gamepad.sThumbLX > UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{
				return true;
			}
		}

		break;

	case JOYTHUMB_LX_DOWN:
		if (g_joykeyState.Gamepad.sThumbLX < DOWN_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{
				return true;
			}
		}

		break;

	case JOYTHUMB_LY_UP:
		if (g_joykeyState.Gamepad.sThumbLY > UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{
				return true;
			}
		}

		break;

	case JOYTHUMB_LY_DOWN:
		if (g_joykeyState.Gamepad.sThumbLY < DOWN_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{				
				return true;
			}
		}

		break;

	case JOYTHUMB_RX_UP:
		if (g_joykeyState.Gamepad.sThumbRX > UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{
				return true;
			}
		}

		break;

	case JOYTHUMB_RX_DOWN:
		if (g_joykeyState.Gamepad.sThumbRX < DOWN_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{
				return true;
			}
		}

		break;

	case JOYTHUMB_RY_UP:
		if (g_joykeyState.Gamepad.sThumbRY > UP_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{
				return true;
			}
		}

		break;

	case JOYTHUMB_RY_DOWN:
		if (g_joykeyState.Gamepad.sThumbRY < DOWN_DEADZONE)
		{
			g_nCounterJoyThumbRepeat[Thumb]++;
			if (g_nCounterJoyThumbRepeat[Thumb] == 1)
			{
				return true;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > 1 && g_nCounterJoyThumbRepeat[Thumb] <= REPEAT_TIME)
			{
				return false;
			}
			else if (g_nCounterJoyThumbRepeat[Thumb] > REPEAT_TIME)
			{
				return true;
			}
		}

		break;

	default:

		return false;

		break;
	}

	return false;
}

//================================================================================================================
// �W���C�p�b�h�̃X�e�B�b�N��{������(WASD)���擾
//================================================================================================================
bool GetJoyThumbWASD(void)
{
	if (g_joykeyState.Gamepad.sThumbLX > UP_DEADZONE)
	{
		return true;
	}
	else if (g_joykeyState.Gamepad.sThumbLX < DOWN_DEADZONE)
	{
		return true;
	}
	else if (g_joykeyState.Gamepad.sThumbLY > UP_DEADZONE)
	{
		return true;
	}
	else if (g_joykeyState.Gamepad.sThumbLY < DOWN_DEADZONE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//================================================================================================================
// �}�E�X�̏���������
//================================================================================================================
HRESULT InitMouse(HWND hWnd)
{
	// ���̓f�o�C�X(�}�E�X)�̐���
	g_pInput->CreateDevice(
		GUID_SysMouse,
		&g_pDevMouse,
		NULL
	);

	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(g_pDevMouse->SetCooperativeLevel(hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// �}�E�X�ւ̃A�N�Z�X�����l��
	g_pDevMouse->Acquire();

	return S_OK;
}

//================================================================================================================
// �}�E�X�̏I������
//================================================================================================================
void UninitMouse(void)
{
	// ���̓f�o�C�X(�}�E�X)�̔j��
	if (g_pDevMouse != NULL)
	{
		g_pDevMouse->Unacquire();
		g_pDevMouse->Release();
		g_pDevMouse = NULL;
	}
}

//================================================================================================================
// �}�E�X�̍X�V����
//================================================================================================================
void UpdateMouse(void)
{
	BYTE aMouseState[NUM_MOUSE_MAX];			// �}�E�X�̓��͏��

	// �X�V�O�ɉߋ��̓��͏��(�}�E�X)��ۑ�
	g_PrevMouseState = g_CurrentMouseState;

	// ���݂̃}�E�X�̓��͏����擾
	if (SUCCEEDED(g_pDevMouse->GetDeviceState(sizeof(DIMOUSESTATE), &g_CurrentMouseState)))
	{
		for (int nCntMouse = 0; nCntMouse < NUM_MOUSE_MAX; nCntMouse++)
		{
			aMouseState[nCntMouse] = g_CurrentMouseState.rgbButtons[nCntMouse];
			g_aMouseStateTrigger[nCntMouse] = ((aMouseState[nCntMouse] ^ g_aMouseState[nCntMouse]) & aMouseState[nCntMouse]);
			g_aMouseStateRelease[nCntMouse] = (g_aMouseState[nCntMouse] & (g_aMouseState[nCntMouse] ^ aMouseState[nCntMouse]));
			g_aMouseState[nCntMouse] = aMouseState[nCntMouse];
		}
	}
	else
	{
		g_pDevMouse->Acquire();			// �}�E�X�ւ̃A�N�Z�X�����擾
	}
}

//================================================================================================================
// �}�E�X�̃v���X�����擾
//================================================================================================================
bool GetMousePress(int nButton)
{
	return (g_aMouseState[nButton] & 0x80) ? true : false;
}

//================================================================================================================
// �}�E�X�̃g���K�[�����擾
//================================================================================================================
bool GetMouseTrigger(int nButton)
{
	return (g_aMouseStateTrigger[nButton] & 0x80) ? true : false;
}

//================================================================================================================
// �}�E�X�̃����[�X�����擾
//================================================================================================================
bool GetMouseRelease(int nButton)
{
	return (g_aMouseStateRelease[nButton] & 0x80) ? true : false;
}

//================================================================================================================
// �}�E�X�̈ʒu�����擾
//================================================================================================================
POINT GetMousePos(void)
{
	POINT mousePos;

	// �}�E�X�̈ʒu���擾
	if (GetCursorPos(&mousePos) != TRUE)
	{// �擾���s
		g_pDevMouse->Acquire();			// �}�E�X�ւ̃A�N�Z�X�����擾
	}
	else
	{// �擾����
		// �擾�������W���E�B���h�E���̍��W�ɕϊ�
		ScreenToClient(g_InputhWnd, &mousePos);
		g_mousePos = mousePos;			// ���݂̈ʒu��ۑ�
	}

	return g_mousePos;
}