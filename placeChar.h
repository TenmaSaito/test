//================================================================================================================
//
// DirectX�̎��R���Ńe�N�X�`���̐ݒu�p�w�b�_�[�t�@�C�� [placeChar.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLACECHAR_H_
#define _PLACECHAR_H_

#include "main.h"

// �e�N�X�`���̎��
typedef enum
{
	CHARTEX_FULLCHARGE = 0,			// �t���`���[�W�\��
	CHARTEX_BARRIAR,				// �o���A�\��
	CHARTEX_TUTORIAL_START,			// �`���[�g���A���J�n�̍��}
	CHARTEX_TUTORIAL_START_BRIEF,	// �`���[�g���A���̐���
	CHARTEX_STEP1,					// 1 �ړ� �ȉ��`���[�g���A���p�����e�N�X�`��
	CHARTEX_STEP1_BRIEF,			// 1�̐�����
	CHARTEX_STEP2,					// 2 �ˌ�
	CHARTEX_STEP2_BRIEF,			// 2�̐�����
	CHARTEX_STEP3,					// 3 �`���[�W
	CHARTEX_STEP3_BRIEF,			// 3�̐�����
	CHARTEX_STEP4,					// 4 ����Z
	CHARTEX_STEP4_BRIEF,			// 4�̐�����
	CHARTEX_STEP5,					// 5 �o���A
	CHARTEX_STEP5_BRIEF,			// 5�̐�����
	CHARTEX_STEP6,					// 6 �G���j
	CHARTEX_STEP6_BRIEF,			// 6�̐�����
	CHARTEX_STEP7,					// 7 �A�C�e��
	CHARTEX_STEP7_BRIEF,			// 7�̐�����
	CHARTEX_TUTORIAL_COMPLETE,		// �`���[�g���A���I��
	CHARTEX_SUCCESS,				// �������\��
	CHARTEX_ABLE_TELEPORT,			// �e���|�[�g�\
	CHARTEX_MAX
}CHARTEX;

// �v���g�^�C�v�錾�@
void InitPlaceChar(void);
void UninitPlaceChar(void);
void UpdatePlaceChar(void);
void DrawPlaceChar(void);

void SetPlaceChar(D3DXVECTOR3 pos, POLY_SIZE size, CHARTEX tex, int nCounter);
void DeleteChar(CHARTEX tex);

#endif // !_PlaceChar_H_
