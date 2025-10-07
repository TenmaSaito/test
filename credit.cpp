//================================================================================================================
//
// DirectX�̃N���W�b�g��ʕ\������ [credit.cpp]
// Author : TENMA
//
//================================================================================================================
#include "credit.h"
#include "input.h"
#include "sound.h"
#include "fade.h"
#include "creditBg.h"
#include "creditLogo.h"
#include "creditPhoto.h"
#include "creditChar.h"

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitCredit(void)
{
	InitCreditBg();

	InitCreditChar();

	InitCreditLogo();

	InitCreditPhoto();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitCredit(void)
{
	UninitCreditBg();

	UninitCreditLogo();

	UninitCreditPhoto();

	UninitCreditChar();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateCredit(void)
{
	// Enter�Ń^�C�g����ʂ֐i��
	if ((GetJoypadTrigger(JOYKEY_START) == true 
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetKeyboardTrigger(DIK_RETURN) == true)
		&& GetFade() == FADE_NONE)
	{
		PlaySound(SOUND_LABEL_SE_ENTER);
		FadeSound(SOUND_LABEL_TITLE);
		SetFade(MODE_TITLE, FADE_TYPE_NORMAL);
	}

	UpdateCreditBg();

	UpdateCreditChar();

	UpdateCreditLogo();

	UpdateCreditPhoto();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawCredit(void)
{
	DrawCreditBg();

	DrawCreditPhoto();

	DrawCreditChar();

	DrawCreditLogo();
}