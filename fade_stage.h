//================================================================================================================
//
// DirectX�̃X�e�[�W�p�t�F�[�h�w�b�_�t�@�C�� [fade_stage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _FADE_STAGE_H_
#define _FADE_STAGE_H_

#include "main.h"
#include "stage.h"

// �t�F�[�h�̎��
typedef enum
{
	FADESTAGE_NONE = 0,			// �t�F�[�h�Ȃ�
	FADESTAGE_IN,				// �t�F�[�h�C��
	FADESTAGE_OUT,				// �t�F�[�h�A�E�g
	FADESTAGE_MAX
}FADESTAGE;

// �v���g�^�C�v�錾
void InitFadeStage(void);
void UninitFadeStage(void);
void UpdateFadeStage(void);
void DrawFadeStage(void);

void SetFadeStage(STAGE stage, FADESTAGE start);
FADESTAGE GetFadeStage(void);

#endif
