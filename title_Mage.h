//================================================================================================================
//
// DirectX�̃^�C�g����ʂł̖��@�w�\�������̃w�b�_�t�@�C�� [title_Mage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TITLEMAGE_H_
#define _TITLEMAGE_H_

#include "main.h"

// ���@�w�̎��

typedef enum
{
	TITLE_MAGETYPE_SELECT = 0,			// �I�����̉��Ɍ���閂�@�w
	TITLE_MAGETYPE_TEXT,				// ������ł��镶���̉��ɂ��閂�@�w
	TITLE_MAGETYPE_MAX
} TITLE_MAGETYPE;

// �v���g�^�C�v�錾
void InitTitleMage(void);
void UninitTitleMage(void);
void UpdateTitleMage(void);
void DrawTitleMage(void);

/// <summary>
/// �^�C�g����ʂł̂ݎg�p�\�ȁA�w�肳�ꂽ���@�w�̃e�N�X�`��������ꂽ�|���S�����w��ʒu�ɕ`�悷��֐��ł��B
/// </summary>
/// <param name="pos">�ݒu�ʒu</param>
/// <param name="type">���@�w�̎��</param>
/// <param name="Caution!">���ӁI�P��ނɂ����@�w�͈�����ݒu�ł��܂���B</param>
void SetTitleMage(D3DXVECTOR3 pos, TITLE_MAGETYPE type);

/// <summary>
/// �w�肳�ꂽ���@�w��col���ő�l�܂ŏ��X�ɏ㏸�����܂��B
/// </summary>
/// <param name="type">���@�w�̎��</param>
void LightUpTitleMage(TITLE_MAGETYPE type);

#endif
