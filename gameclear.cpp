//================================================================================================================
//
// DirectX�̃Q�[���N���A��ʕ\������ [gameclear.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "gameclear.h"
#include "gameclearBg.h"
#include "gameclearEffect.h"
#include "sun.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "score.h"
#include "ResultScore.h"
#include "scoreRank.h"

// �}�N����`
#define FADE_COUNT		(600)						// ���U���g��ʂ֑J�ڂ���܂ł̎���

// �O���[�o���ϐ�
int g_nCounterGameclear = 0;						// ���U���g��ʂ֑J�ڂ���܂ł̃J�E���^�[

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitGameclear(void)
{
	g_nCounterGameclear = 0;

	InitGameclearBg();
	InitGameclearEffect();

	InitResultScore();

	InitSun();

	InitScoreRank();

	SetResultScore(D3DXVECTOR3(1480.0f, 500.0f, 0.0f), GetScore());
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitGameclear(void)
{
	UninitGameclearBg();
	UninitGameclearEffect();
	UninitSun();
	UninitResultScore();
	UninitScoreRank();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateGameclear(void)
{
	UpdateGameclearBg();
	UpdateGameclearEffect();
	UpdateSun();
	UpdateResultScore();
	UpdateScoreRank();

	// Enter�Ń^�C�g����ʂ֐i��
	if ((GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true
		|| GetKeyboardTrigger(DIK_RETURN) == true
		|| g_nCounterGameclear >= FADE_COUNT)
		&& (GetFade() == FADE_NONE && GetEnableClearEffect() == false))
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_RESULT, FADE_TYPE_NORMAL);
	}

	g_nCounterGameclear++;
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawGameclear(void)
{
	DrawGameclearBg();
	DrawGameclearEffect();
	DrawResultScore();
	DrawSun();
	DrawScoreRank();
}