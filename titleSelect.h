//================================================================================================================
//
// DirectX�̃^�C�g���Z���N�g�̃w�b�_�t�@�C�� [titleSelect.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _TITLESELECT_H_
#define _TITLESELECT_H_

#include "main.h"

// �}�N����`
#define SELECTSIZE_WIDTH		(SCREEN_WIDTH * 0.4f)			// ���̃T�C�Y
#define SELECTSIZE_HEIGHT		(SCREEN_HEIGHT * 0.085f)		// �c�̃T�C�Y

#if 0
#define SELECTTYPETUTORIAL_TRUE
#else
#define SELECTTYPETUTORIAL_FALSE
#endif

// �I�����̎��
typedef enum
{
	TITLESELECTTYPE_START = 0,		// �Q�[���J�n(�`���[�g���A���X�L�b�v)
#ifdef SELECTTYPETUTORIAL_TRUE
	TITLESELECTTYPE_TUTORIAL,		// �`���[�g���A������
#endif
	TITLESELECTTYPE_CREDIT,			// �N���W�b�g
	TITLESELECTTYPE_EXIT,			// �Q�[���I��
	TITLESELECTTYPE_BOOKSTART,		// �I�΂�Ă��Ȃ����̃X�^�[�g
#ifdef SELECTTYPETUTORIAL_TRUE
	TITLESELECTTYPE_BOOKTUTORIAL,	// ��Ɠ����`���[�g���A��
#endif
	TITLESELECTTYPE_BOOKCREDIT,		// �N���W�b�g
	TITLESELECTTYPE_BOOKEXIT,		// �������I��
	TITLESELECTTYPE_START_SHADOW,	// �Q�[���J�n�̉e
#ifdef SELECTTYPETUTORIAL_TRUE
	TITLESELECTTYPE_TUTORIAL_SHADOW,// �`���[�g���A���̉e
#endif
	TITLESELECTTYPE_CREDIT_SHADOW,	// �N���W�b�g�̉e
	TITLESELECTTYPE_EXIT_SHADOW,	// �I���̉e
	TITLESELECTTYPE_MAX
}TITLESELECTTYPE;

#ifdef SELECTTYPETUTORIAL_TRUE
#define TITLE_FILENAME	"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_SELECT_V2.png"
#else
#define TITLE_FILENAME	"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_SELECT.png"
#endif

// �v���g�^�C�v�錾
void InitTitleSelect(void);
void UninitTitleSelect(void);
void UpdateTitleSelect(void);
void DrawTitleSelect(void);

TITLESELECTTYPE GetTitleSelect(void);
void SetTitleSelect(bool bUse);

#endif