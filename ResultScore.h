//================================================================================================================
//
// DirectX�̈ړ��X�R�A�w�b�_�[�t�@�C�� [scoreRank.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include "main.h"

// �v���g�^�C�v�錾
void InitResultScore(void);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);

void SetResultScore(D3DXVECTOR3 pos, int nResultScore);
int GetResultScore(void);
void MoveResuktScore(D3DXVECTOR3 move);


#endif
