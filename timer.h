//================================================================================================================
//
// DirectX�̃^�C�}�[�w�b�_�[�t�@�C�� [timer.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

// �v���g�^�C�v�錾
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);

void SetTimer(int nTimer);
int GetTimer(void);
void AddTimer(int nValue);

#endif
