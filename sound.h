//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : TENMA SAITO (SPACIAL THANKS : AKIRA TANAKA)
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

// �}�N����`
#define SOUND_BGMSTART		(SOUND_LABEL_TITLE)			// BGM�̊J�n�ʒu
#define SOUND_SESTART		(SOUND_LABEL_SE_SHOT)		// SE�̊J�n�ʒu
#define SOUND_MENUSTART		(SOUND_LABEL_SE_ENTER)		// MENU�pSE�̊J�n�ʒu
#define SOUND_STANDARD		(0.75f)						// ��������

#ifdef _DEBUG
#undef SOUND_STANDARD
#define SOUND_STANDARD		(0.0f)						// ��������
#endif

//*****************************************************************************
// �T�E���h�ꗗ (sound.cpp��soundinfo�ɂ��ǉ�����I)
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_TITLE = 0,			// �^�C�g����ʂ�BGM
	SOUND_LABEL_GAME_NORMAL,		// �Q�[���v���C���̑���BGM
	SOUND_LABEL_GAME_DESERT,		// �Q�[���v���C���̍���BGM
	SOUND_LABEL_GAME_ICE,			// �Q�[���v���C���̕X��BGM
	SOUND_LABEL_GAME_FOREST,		// �Q�[���v���C���̐X��BGM
	SOUND_LABEL_GAME_VOLCANO,		// �Q�[���v���C���̉ΎRBGM
	SOUND_LABEL_GAME_SEA,			// �Q�[���v���C���̊C�mBGM
	SOUND_LABEL_GAME_NOMORETIME,	// �������Ԃ�����������BGM
	SOUND_LABEL_GAMECLEAR,			// �Q�[���N���A����BGM
	SOUND_LABEL_GAMEOVER,			// �Q�[���I�[�o�[����BGM
	SOUND_LABEL_GAME_TUTORIAL,		// �`���[�g���A������BGM
	SOUND_LABEL_CREDIT,				// �N���W�b�g�ŗ����BGM
	SOUND_LABEL_SE_SHOT,		// �e���ˉ�
	SOUND_LABEL_SE_HIT,			// �q�b�g��
	SOUND_LABEL_SE_EXPLOSION,	// ������
	SOUND_LABEL_SE_ENTER,		// ���艹
	SOUND_LABEL_SE_BARRIAR,		// �o���A������
	SOUND_LABEL_SE_GETCOIN,		// �R�C���l����
	SOUND_LABEL_SE_GETULTIMATECOIN,		// �����R�C���l����
	SOUND_LABEL_SE_GETKEY,		// �R�C���l����
	SOUND_LABEL_SE_OPEN,		// �h�A���J���鉹
	SOUND_LABEL_SE_SELECT,		// �h�A���J���鉹
	SOUND_LABEL_SE_CHARGE,		// �`���[�W�J�n��
	SOUND_LABEL_SE_FULLCHARGE,	// �`���[�W����
	SOUND_LABEL_SE_SHOT_1,	// �`���[�W����
	SOUND_LABEL_SE_CHARGE_SHOT,	// �`���[�W����
	SOUND_LABEL_SE_HEAL,		// �񕜉�
	SOUND_LABEL_SE_POWER_CHANGE,	// �`���[�W�Z�A�C�e������������̉�
	SOUND_LABEL_SE_TELEPORT,		// �e���|�[�g��
	SOUND_LABEL_SE_MIRROR,			// ���ˉ�
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �T�E���h�̎��
//*****************************************************************************
typedef enum
{
	SETSOUND_GAME = 0,			// �Q�[��������
	SETSOUND_MENU_SE,			// �V�X�e������
	SETSOUND_MAX
}SETSOUND;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

// �ǉ��v���g�^�C�v�錾
void SetVolume(float volume, SETSOUND sound);
float GetVolume(SETSOUND sound);
void FadeSound(SOUND_LABEL label);
SOUND_LABEL GetPlaySound(void);
void UpdateSound(void);

#endif
