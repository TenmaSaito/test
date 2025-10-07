//================================================================================================================
//
// DirectX�̃Q�[�����̘g�̕\�������̃w�b�_�t�@�C�� [playerframe.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLAYERFRAME_H_
#define _PLAYERFRAME_H_

#include "main.h"

// �g�̏��
typedef enum
{
	FRAMESTATE_NORMAL = 0,		// �ʏ�
	FRAMESTATE_DAMAGE,			// ��e
	FRAMESTATE_DANGER,			// �m��
	FRAMESTATE_DEATH,			// ���S
	FRAMESTATE_APPEAR,			// �o��
	FRAMESTATE_BARRIAR,			// �o���A
	FRAMESTATE_HEAL,			// ��
	FRAMESTATE_POWER_ITEM,		// �p���[�A�b�v
	FRAMESTATE_MAX
}FRAMESTATE;

// �v���g�^�C�v�錾
void InitPlayerFrame(void);
void UninitPlayerFrame(void);
void UpdatePlayerFrame(void);
void DrawPlayerFrame(void);

void ChangeModeFrame(FRAMESTATE state);

#endif
