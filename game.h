//================================================================================================================
//
// DirectX�̃Q�[���w�b�_�[�t�@�C�� [game.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

// �}�N����`
#define NOMORE_TIME			(30)			// �c�莞�Ԃ����Ȃ��

// �X�e�[�W�̎��
typedef enum
{
	STAGE_GRASS = 0,		// �����n��		
	STAGE_DESERT,			// �����n��
	STAGE_ICE,				// �X�͒n��
	STAGE_FOREST, 			// �X�ђn��
	STAGE_VOLCANO,			// �ΎR�n��
	STAGE_SEA,				// �C�m�n��
	STAGE_MAX
}STAGE;

// �Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,
	GAMESTATE_NORMAL,
	GAMESTATE_CLEAREND,
	GAMESTATE_BADEND,
	GAMESTATE_MAX
}GAMESTATE;

// �v���g�^�C�v�錾
void InitGame(GAMEDIFFICULTY difficulty);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);

void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);

void SetGamePause(bool bUse);
bool GetGamePause(void);

void SetGameTutorial(bool bUse);
bool GetGameTutorial(void);

#endif // !_GAME_H_
