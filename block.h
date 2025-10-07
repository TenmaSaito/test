//================================================================================================================
//
// DirectX�̃u���b�N�w�b�_�t�@�C�� [block.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"
#include "enemy.h"

// �}�N����`
#define BLOCK_WIDTH		(50)			// �u���b�N�̉��̃T�C�Y
#define BLOCK_HEIGHT	(50)			// �u���b�N�̍���

// �u���b�N�̎��
typedef enum
{
	BLOCKTYPE_WALL = 0,				// ��Q��
	BLOCKTYPE_BATTERY,				// �C��
	BLOCKTYPE_EXIT_A,				// �o��A(�����ƉΎR�Ƃ̓�)
	BLOCKTYPE_EXIT_B,				// �o��B(�����ƐX�Ƃ̓�)
	BLOCKTYPE_EXIT_C,				// �o��C(�����ƕX�͂Ƃ̓�)
	BLOCKTYPE_EXIT_D,				// �o��D(�ΎR�ƍ����Ƃ̓�)
	BLOCKTYPE_EXIT_E,				// �o��E(�����ƐX�Ƃ̓�)
	BLOCKTYPE_EXIT_F,				// �o��F(�X�ƊC�Ƃ̓�)
	BLOCKTYPE_EXIT_G,				// �o��G(�C�ƕX�͂Ƃ̓�)
	BLOCKTYPE_EX_EXIT_1,			// �A�C�e��(��)�ɂ���ĊJ�������u���b�N
	BLOCKTYPE_MAX
}BLOCKTYPE;

// �u���b�N�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				// �u���b�N�̈ʒu
	D3DXCOLOR col;					// �u���b�N�̐F
	BLOCKTYPE type;					// �u���b�N�̎��
	float fLength;					// �u���b�N�̑Ίp��
	float fAngle;					// �Ίp���̊p�x
	float fLaunchAngle;				// �e�̔��ˊp(BLOCKTYPE_BATTERY�Ŏg�p)
	float fWidth;					// �u���b�N�̉��̒���
	float fHeight;					// �u���b�N�̏c�̒���
	int nCounter;					// �ėp�J�E���^�[
	bool bUse;						// �u���b�N���g���Ă��邩�ǂ���
}BLOCK;

// �v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);

void LoadBlock(char *pFileName);

int SetBlock(BLOCKTYPE type, D3DXVECTOR3 pos, float fWidth, float fHeight);
BLOCK *GetBlock(int nID);
void DeleteBlock(int nID);
void ResetBlock(void);

#endif
