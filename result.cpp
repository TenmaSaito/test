//================================================================================================================
//
// DirectX�̃��U���g��ʕ\������ [result.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "result.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "resultbg.h"
#include "ranking.h"
#include "showDifficulty.h"

// �O���[�o���ϐ�
DWORD g_dwCurrentTimeResult;						// ���݂̎���
DWORD g_dwExacLastTimeTitleResult;					// �^�C�g����ʂɈڍs�����u�Ԃ̎���

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitResult(void)
{
	// ���U���g�w�i�̏���������
	InitResultBg();

	// �����L���O�̏���������
	InitRanking();

	InitShowDifficulty();

	g_dwCurrentTimeResult = timeGetTime();
	g_dwExacLastTimeTitleResult = timeGetTime();

	// �����L���O�̕\��
	SetRanking();

	SetShowDifficulty(D3DXVECTOR3(WINDOW_MID.x, 50.0f, 0.0f), GetGameDifficulty());
}

//================================================================================================================
// ���U���g�̏I������
//================================================================================================================
void UninitResult(void)
{
	// ���U���g�w�i�̏I������
	UninitResultBg();

	// �����L���O�̏I������
	UninitRanking();

	UninitShowDifficulty();
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateResult(void)
{
	// �����L���O�̍X�V����
	UpdateRanking();

	// ���U���g�w�i�̍X�V����
	UpdateResultBg();

	UpdateShowDifficulty();

	// Enter�Ń^�C�g����ʂ֐i��
	if ((GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true
		|| GetKeyboardTrigger(DIK_RETURN)) == true 
		&& GetFade() == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		SetFade(MODE_TITLE, FADE_TYPE_NORMAL);
		FadeSound(SOUND_LABEL_TITLE);
	}

	g_dwCurrentTimeResult = timeGetTime();
	if ((g_dwCurrentTimeResult - g_dwExacLastTimeTitleResult) >= 10000)
	{
		if (GetFade() == FADE_NONE)
		{
			SetFade(MODE_TITLE, FADE_TYPE_NORMAL);
			FadeSound(SOUND_LABEL_TITLE);
		}
	}
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawResult(void)
{
	// ���U���g�w�i�̕`�揈��
	DrawResultBg();

	// �����L���O�̕`�揈��
	DrawRanking();

	DrawShowDifficulty();
}