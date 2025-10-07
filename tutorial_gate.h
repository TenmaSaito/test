//================================================================================================================
//
// DirectX�̃`���[�g���A���p��Փx�Q�[�g�̃w�b�_�t�@�C�� [tutorial_gate.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TUTORIAL_GATE_H_
#define _TUTORIAL_GATE_H_

#include "main.h"

// �}�N����`
#define GATE_WIDTH		(50)			// �u���b�N�̉��̃T�C�Y
#define GATE_HEIGHT		(50)			// �u���b�N�̍���

// �Q�[�g�̎��
typedef enum
{
	GATETYPE_EXIT_EASY = 0,				// �o��A(EASY)
	GATETYPE_EXIT_NORMAL,				// �o��B(NORMAL)
	GATETYPE_EXIT_HARD,					// �o��C(HARD)
	GATETYPE_MAX
}GATETYPE;

// �Q�[�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				// �u���b�N�̈ʒu
	D3DXCOLOR col;					// �u���b�N�̐F
	GATETYPE type;					// �u���b�N�̎��
	GAMEDIFFICULTY difficulty;		// ��Փx
	POLY_SIZE size;					// �u���b�N�̃T�C�Y
	bool bUse;						// �u���b�N���g���Ă��邩�ǂ���
}GATE;

// �v���g�^�C�v�錾
void InitTutorialGate(void);
void UninitTutorialGate(void);
void UpdateTutorialGate(void);
void DrawTutorialGate(void);

void LoadTutorialGate(char *pFileName);

int SetTutorialGate(GATETYPE type, D3DXVECTOR3 pos, POLY_SIZE size);
void DestroyTutorialGate(void);

#endif
