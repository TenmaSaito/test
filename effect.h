//================================================================================================================
//
// DirectX�̃G�t�F�N�g�w�b�_�[�t�@�C�� [effect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"

#if 0
// �G�t�F�N�g�̎��
typedef enum
{
	PARTICLETYPE_NORMAL = 0,		// ���U����p�[�e�B�N��
	PARTICLETYPE_MAX
}EFFECTYPE;
#endif

// �v���g�^�C�v�錾�@
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);

void SetEffect(D3DXVECTOR3 pos,D3DXVECTOR3 move,D3DXCOLOR col, float fRadius ,int nLife, bool bUseColRainbow);
void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col, D3DXVECTOR3 move, float fRadius, int nLife, float fRandMin, float fRandMax, bool bParticleMove);
void DestroyEffect(void);

#endif // !_BULLET_H_
