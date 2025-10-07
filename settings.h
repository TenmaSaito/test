//================================================================================================================
//
// DirectX�̐ݒ�w�b�_�t�@�C�� [settings.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "main.h"
#include "fade.h"

// �ݒ��ʂ̎��
typedef enum
{
	SETTINGS_TITLE = 0,			// �^�C�g����ʂ���̐ݒ�
	SETTINGS_PAUSE,				// �|�[�Y��ʂ���̐ݒ�
	SETTINGS_MAX
}SETTINGS;

// �I�����̎��
typedef enum
{
	SETTINGTYPE_NONE = 0,		// ���I��
	SETTINGTYPE_GAME_SOUND,		// �Q�[���������̐ݒ�
	SETTINGTYPE_SE_SOUND,		// SE�̐ݒ�
	SETTINGTYPE_EXIT,			// �ݒ�̏I��
	SETTINGTYPE_MAX
}SETTINGTYPE;

// �v���g�^�C�v�錾
void InitSettings(void);
void UninitSettings(void);
void UpdateSettings(void);
void DrawSettings(void);

void SetSettings(void);
FADE GetSettings(void);

#endif