//================================================================================================================
//
// DirectX�̒e�̔����w�b�_�[�t�@�C�� [explosion.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "main.h"

// �v���g�^�C�v�錾�@
void InitExplosion(void);
void UninitExplosion(void);
void UpdateExplosion(void);
void DrawExplosion(void);
void SetExplosion(D3DXVECTOR3 pos,D3DXCOLOR col,bool bFadeout);

#endif // !_Explosion_H_
