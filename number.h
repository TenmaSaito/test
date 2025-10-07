//================================================================================================================
//
// DirectX�̐����ݒu�w�b�_�[�t�@�C�� [number.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _NUMBER_H_
#define _NUMBER_H_

#include "main.h"

// �����\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			// �����̐ݒu�ʒu
	D3DXCOLOR col;				// �����̐F
	int nNumber;				// �ݒu���鐔���̒l
	float fLength;				// �Ίp���̒���
	bool bUse;					// �g���Ă��邩�ǂ���
}NUMBER;

// �v���g�^�C�v�錾
void InitNumber(void);
void UninitNumber(void);
void UpdateNumber(void);
void DrawNumber(void);

int SetNumber(int nNumber, D3DXVECTOR3 pos);
int GetNumber(int nID);
void AddNumber(int nID, int nValue);
void DestroyNumber(int nID);

#endif
