//================================================================================================================
//
// DirectX�̃G�f�B�^�̃w�b�_�t�@�C�� [editer.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _EDITER_H_
#define _EDITER_H_

#include "main.h"

// �}�N����`
#define MAX_EDITER_BLOCK	(128)		// �G�f�B�^�u���b�N�̍ő�ݒu�\��

void InitEditer(void);
void UninitEditer(void);
void UpdateEditer(void);
void DrawEditer(void);

void SetEnableEditer(bool bUse, int nID);
HRESULT SaveBlock(void);
HRESULT SaveEnemy(void);

#endif // !_EDITER_H_

