//================================================================================================================
//
// DirectX�̃X�e�[�W�w�b�_�t�@�C�� [stage.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "game.h"

//�}�N����`
#define STAGE_WIDTH			(2000.0f)					// �X�e�[�W�̉�
#define STAGE_HEIGHT		(1000.0f)					// �X�e�[�W�̏c
#define STAGE_LEFT			(SCREEN_WIDTH * 0.5f - (STAGE_WIDTH * 0.5f))		// �X�e�[�W�̍��[
#define STAGE_RIGHT			(SCREEN_WIDTH * 0.5f + (STAGE_WIDTH * 0.5f))		// �X�e�[�W�̉E�[
#define STAGE_CEILING		(SCREEN_HEIGHT * 0.5f - (STAGE_HEIGHT * 0.5f))		// �X�e�[�W�̏��
#define STAGE_FLOOR			(SCREEN_HEIGHT * 0.5f + (STAGE_HEIGHT * 0.5f))		// �X�e�[�W�̉���
#define ALREADY_CLEARED	(-1)			// �X�e�[�W���N���A���ꂽ�ꍇ�̃^�[��

#define FRAME_FILENAME		"data\\STAGE\\FRAME\\"						// �X�e�[�W�̃t���[���̃t�@�C���A�h���X
#define GRASS_FILENAME		"data\\STAGE\\GRASS\\"						// �����X�e�[�W�̃t�@�C���A�h���X
#define VOLCANO_FILENAME	"data\\STAGE\\VOLCANO\\"					// �ΎR�X�e�[�W�̃t�@�C���A�h���X
#define DESERT_FILENAME		"data\\STAGE\\DESERT\\"						// �����X�e�[�W�̃t�@�C���A�h���X
#define FOREST_FILENAME		"data\\STAGE\\FOREST\\"						// �X�уX�e�[�W�̃t�@�C���A�h���X
#define SEA_FILENAME		"data\\STAGE\\SEA\\"						// �C�X�e�[�W�̃t�@�C���A�h���X
#define ICE_FILENAME		"data\\STAGE\\ICE\\"						// �X�R�X�e�[�W�̃t�@�C���A�h���X
#define TUTORIAL_FILENAME	"data\\STAGE\\TUTORIAL\\"					// �`���[�g���A���̃t�@�C���A�h���X

#define BLOCK_FILETYPE		"BLOCK.txt"											// �u���b�N�p�A�h���X����
#define ENEMY_FILETYPE_EASY		"ENEMY_EASY.txt"								// �G�l�~�[�p�A�h���X����(EASY)
#define ENEMY_FILETYPE_NORMAL	"ENEMY_NORMAL.txt"								// �G�l�~�[�p�A�h���X����(NORMAL)
#define ENEMY_FILETYPE_HARD		"ENEMY_HARD.txt"								// �G�l�~�[�p�A�h���X����(HARD)
#define FRAME_LEFT_FILETYPE		"FRAME_LEFT.txt"								// ���t���[���p�A�h���X����
#define FRAME_RIGHT_FILETYPE	"FRAME_RIGHT.txt"								// �E�t���[���p�A�h���X����
#define FRAME_UP_FILETYPE		"FRAME_UP.txt"									// ��t���[���p�A�h���X����
#define FRAME_DOWN_FILETYPE		"FRAME_DOWN.txt"								// ���t���[���p�A�h���X����

// �v���g�^�C�v�錾
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void DrawStage(void);

void SetStage(STAGE stage);
STAGE GetStage(void);
int GetTurn();
void UniteFileName(const char* pFileName, const char* pFiletype);
void SetClearBossStage(bool bClear);
void SetEnableHaveKeyExac(bool bHave);
void SetEnableHaveSecret(bool bHave);
int GetClearStageNum(void);
bool GetHaveSecret(void);

#endif