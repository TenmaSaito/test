//================================================================================================================
//
// DirectX�̃Q�[���I�[�o�[��ʕ\������ [gameover.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "gameover.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "gameoverBg.h"
#include "gameoverEffect.h"
#include "ResultScore.h"
#include "score.h"
#include "scoreRank.h"
#include "moon.h"

// �}�N����`
#define FADE_COUNT		(600)						// ���U���g��ʂ֑J�ڂ���܂ł̎���

// �O���[�o���ϐ�
int g_nCurrentScore = 0;
int g_nCounterGameover = 0;

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitGameover(void)
{
	g_nCounterGameover = 0;

	InitGameoverBg();

	InitGameoverEffect();

	InitResultScore();

	InitScoreRank();

	InitMoon();

	SetResultScore(D3DXVECTOR3(1480.0f,500.0f,0.0f), GetScore());
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitGameover(void)
{
	UninitGameoverBg();

	UninitGameoverEffect();

	UninitResultScore();

	UninitScoreRank();

	UninitMoon();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateGameover(void)
{
	// Enter�Ń^�C�g����ʂ֐i��
	if ((GetJoypadPress(JOYKEY_START) == true 
		|| GetKeyboardTrigger(DIK_RETURN) == true
		|| g_nCounterGameover >= FADE_COUNT)
		&& (GetFade() == FADE_NONE && GetEnableGameoverEffect() == false))
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_RESULT, FADE_TYPE_NORMAL);
	}

	UpdateGameoverBg();

	UpdateGameoverEffect();

	UpdateResultScore();

	UpdateScoreRank();

	UpdateMoon();

	g_nCounterGameover++;
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawGameover(void)
{
	DrawGameoverBg();

	DrawMoon();

	DrawGameoverEffect();

	DrawResultScore();

	DrawScoreRank();
}