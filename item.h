//================================================================================================================
//
// DirectX�̃A�C�e���w�b�_�t�@�C�� [item.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _ITEM_H_
#define _ITEM_H_

#include "main.h"

// �}�N����`
#define ITEM_WIDTH		(50)			// �u���b�N�̉��̃T�C�Y
#define ITEM_HEIGHT		(50)			// �u���b�N�̍���

// �A�C�e���̎��
typedef enum
{
	ITEMTYPE_COIN = 0,			// �R�C��
	ITEMTYPE_HEAL,				// �v���C���[�̉񕜃A�C�e��
	ITEMTYPE_HOMING,			// �`���[�W�Z�̕ύX�A�C�e��(�z�[�~���O)
	ITEMTYPE_BOMB,				// �`���[�W�Z�̕ύX�A�C�e��(�{��)
	ITEMTYPE_LASER,				// �`���[�W�Z�̕ύX�A�C�e��(���[�U�[)
	ITEMTYPE_KEY,				// �A�C�e�� ��
	ITEMTYPE_ULTIMATE_COIN,		// �A�C�e�� �����R�C��
	ITEMTYPE_SECRET,			// �g�p�p�r��������Ȃ��A�C�e��(�X�R�A�{���A�b�v)
	ITEMTYPE_MAX
}ITEMTYPE;

// �u���b�N�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				// �u���b�N�̈ʒu
	D3DXCOLOR col;					// �F
	ITEMTYPE type;					// �u���b�N�̎��
	int nAlphaItem;					// ���n
	bool bUse;						// �u���b�N���g���Ă��邩�ǂ���
}ITEM;

// �v���g�^�C�v�錾
void InitItem(void);
void UninitItem(void);
void UpdateItem(void);
void DrawItem(void);

void SetItem(ITEMTYPE type, D3DXVECTOR3 pos);
void DestroyItem(void);
#endif
