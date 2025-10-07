//================================================================================================================
//
// DirectX�̃^�C�g����ʕ\������ [title.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "title.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "game.h"
#include "titlebg.h"
#include "titleSelect.h"
#include "titlelogo.h"
#include "titleanimation.h"
#include "title_Mage.h"

// �^�C�g���\����
typedef struct
{
	int nCounterAnim;
	int nPatternAnim;
	bool bUse;
}Title;

// �O���[�o���ϐ�
Title g_Title;									// �^�C�g���\���̂̍쐬
DWORD g_dwCurrentTimeTitle;						// ���݂̎���
DWORD g_dwExacLastTimeTitle;					// �^�C�g����ʂɈڍs�����u�Ԃ̎���
bool g_bEnableTitleSelect = false;				// �Z���N�g��ʂ��\������Ă��邩�ǂ���
bool g_bEnableTitleLogo = false;

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitTitle(void)
{
	// �^�C�g���\���̂̏�����
	g_Title.nCounterAnim = 0;			
	g_Title.nPatternAnim = 0;
	g_Title.bUse = true;
	g_bEnableTitleSelect = false;
	g_bEnableTitleLogo = false;

	g_dwCurrentTimeTitle = timeGetTime();
	g_dwExacLastTimeTitle = timeGetTime();

	// �^�C�g����ʂ̔w�i�̏���������
	InitTitleBg();

	// �I����ʂ̏���������
	InitTitleSelect();

	InitTitleLogo();

	InitTitleAnimation();

	InitTitleMage();
}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitTitle(void)
{
	// �^�C�g����ʂ̔w�i�̏I������
	UninitTitleBg();

	// �I����ʂ̏I������
	UninitTitleSelect();

	UninitTitleLogo();

	UninitTitleAnimation();

	UninitTitleMage();
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateTitle(void)
{
	int nDifficulty = GAMEDIFFICULTY_EASY;

	if (GetFade() == FADE_NONE
		&& g_bEnableTitleLogo == false)
	{
		SetTitleLogo();
	}

	UpdateTitleLogo();
	UpdateTitleSelect();
	if (GetFade() == FADE_NONE 
		&& g_bEnableTitleSelect == false
		&& g_bEnableTitleLogo == true)
	{
		SetTitleSelect(true);
		g_bEnableTitleSelect = true;
	}

	g_dwCurrentTimeTitle = timeGetTime();
	if ((g_dwCurrentTimeTitle - g_dwExacLastTimeTitle) >= 15000)
	{
		if (GetFade() == FADE_NONE)
		{
			nDifficulty = rand() % GAMEDIFFICULTY_MAX;
			if (nDifficulty < GAMEDIFFICULTY_EASY)
			{
				nDifficulty = GAMEDIFFICULTY_EASY;
			}
			else if (nDifficulty >= GAMEDIFFICULTY_MAX)
			{
				nDifficulty = GAMEDIFFICULTY_HARD;
			}

			SetGameDifficulty((GAMEDIFFICULTY)nDifficulty);

			SetFade(MODE_RESULT, FADE_TYPE_NORMAL);
		}
	}

	if (GetKeyboardAny() == true ||
		GetJoypadAny() == true)
	{ // ������ł��L�[���������ꍇ
		g_dwExacLastTimeTitle = g_dwCurrentTimeTitle;
	}

	UpdateTitleAnimation();

	UpdateTitleMage();
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawTitle(void)
{
	// �^�C�g����ʂ̔w�i�̕`�揈��
	DrawTitleAnimation();

	DrawTitleBg();

	DrawTitleSelect();

	DrawTitleLogo();

	DrawTitleMage();
}

void SetEnableTitleLogo(bool bComplete)
{
	g_bEnableTitleLogo = bComplete;
}

bool GetEnableTitleSelect(void)
{
	return g_bEnableTitleSelect;
}