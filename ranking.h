//================================================================================================================
//
// DirectX�̃����L���O�\���̃w�b�_�[�t�@�C�� [ranking.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"

// �v���g�^�C�v�錾
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);

void SetRanking(void);
int GetRanking(void);
void AddRanking(int nScore);
void EnableScoreMove(bool bUseMove);

#endif
