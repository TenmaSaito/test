//================================================================================================================
//
// DirectX�̓�Փx�\���p�w�b�_�t�@�C�� [showDifficulty.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _SHOWDIFFICULTY_H_
#define _SHOWDIFFICULTY_H_

#include "main.h"

// �v���g�^�C�v�錾
void InitShowDifficulty(void);
void UninitShowDifficulty(void);
void UpdateShowDifficulty(void);
void DrawShowDifficulty(void);

void SetShowDifficulty(D3DXVECTOR3 pos, GAMEDIFFICULTY difficulty);
void DestroyShowDifficulty(void);
#endif