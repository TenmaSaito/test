//================================================================================================================
//
// DirectX�̃|�[�Y�w�b�_�t�@�C�� [pause.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"

// �|�[�Y�̎��
typedef enum
{
	PAUSE_NONE = 0,			// �|�[�Y�Ȃ�
	PAUSE_IN,				// �|�[�Y�C��
	PAUSE_OUT,				// �|�[�Y�A�E�g
	PAUSE_MAX
}PAUSE;

// �I�����̎��
typedef enum
{
	SELECTTYPE_CONTINUE = 0,	// �Q�[���ĊJ
	SELECTTYPE_SETTINGS,		// �ݒ�
	SELECTTYPE_RESTART,			// �ŏ�����
	SELECTTYPE_EXIT,			// �Q�[���I��
	SELECTTYPE_NONE,			// �������(���I��)
	SELECTTYPE_MAX
}SELECTTYPE;

// �v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);

void SetPause(bool bUse);
PAUSE GetPause(void);
SELECTTYPE *GetSelect(void);

#endif