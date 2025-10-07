//================================================================================================================
//
// DirectX�̃t�F�[�h�w�b�_�t�@�C�� [Fade.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

// �}�N����`
#define NORMAL_FADE			D3DXCOLOR(0.0f,0.0f,0.0f,0.0f)		// �ʏ�̈Ó]�t�F�[�h

// �t�F�[�h�̎��
typedef enum
{
	FADE_NONE = 0,			// �t�F�[�h�Ȃ�
	FADE_IN,				// �t�F�[�h�C��
	FADE_OUT,				// �t�F�[�h�A�E�g
	FADE_MAX
}FADE;

// �t�F�[�h�̊T�v
typedef enum
{
	FADE_TYPE_NORMAL = 0,	// �ʏ�t�F�[�h
	FADE_TYPE_TEXTURE,		// �e�N�X�`���̒���ꂽ�|���S���ɂ��t�F�[�h
	FADE_TYPE_MAX
}FADE_TYPE;

// �v���g�^�C�v�錾
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);

/// <summary>
/// �w�肳�ꂽ��ނ̃|���S���ŉ�ʑS�̂��t�F�[�h�C��,�A�E�g������֐��ł��B
/// </summary>
/// <param name="modeNext">�t�F�[�h�A�E�g��Ɉڍs���Ă��郂�[�h</param>
/// <param name="type">�t�F�[�h�̎��</param>
void SetFade(MODE modeNext, FADE_TYPE type);

FADE GetFade(void);

#endif