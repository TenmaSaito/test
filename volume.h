//================================================================================================================
//
// DirectX�̉��ʒ����w�b�_�[�t�@�C�� [volume.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _VOLUME_H_
#define _VOLUME_H_

#include "main.h"
#include "sound.h"

// �v���g�^�C�v�錾
void InitVolume(void);
void UninitVolume(void);
void UpdateVolume(void);
void DrawVolume(void);

void SetVolume(void);
void DeleteVolume(void);
void AddVolume(int nValue, SETSOUND type);

#endif
