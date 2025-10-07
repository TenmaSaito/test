//================================================================================================================
//
// DirectX�̃Q�[����ʏ��� [game.cpp]
// Author : TENMA
//
//================================================================================================================
#include "game.h"
#include "input.h"
#include "player.h"
#include "bg.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "fade.h"
#include "effect.h"
#include "gauge.h"
#include "timer.h"
#include "stage.h"
#include "fade_stage.h"
#include "block.h"
#include "pause.h"
#include "particle.h"
#include "heart.h"
#include "stock.h"
#include "ranking.h"
#include "placeChar.h"
#include "tutorial.h"
#include "tutorial_gate.h"
#include "playerframe.h"
#include "mage.h"
#include "item.h"
#include "nomoretime.h"
#include "teleport.h"
#include "showDifficulty.h"
#include "caution.h"

// �v���g�^�C�v�錾
int AddBonusScore(void);

// �O���[�o���ϐ�
bool g_bPause;										// �|�[�Y��Ԃ�ON/OFF
int g_nCounter;
float g_fVolume ;									// ����
GAMESTATE g_gameState = GAMESTATE_NONE;				// �Q�[�����
GAMEDIFFICULTY g_gameDifficulty;					// �Q�[���̓�Փx
int g_nCounterState;								// ��ԃJ�E���^�[
bool g_bUseTutorial;								// �`���[�g���A���̗L��

//================================================================================================================
// �Q�[���̏���������
//================================================================================================================
void InitGame(GAMEDIFFICULTY difficulty)
{
	/*�O���[�o���ϐ��̏�����*/
	g_bPause = false;
	g_fVolume = 0.0f;
	g_nCounter = 0;
	g_gameState = GAMESTATE_NORMAL;
	g_nCounterState = 0;
	g_gameDifficulty = difficulty;

	/*�e��I�u�W�F�N�g�̏���������*/
	
	// 1�u���b�N�̏���������
	InitBlock();

	// 2Mage
	InitMage();

	// 3Item
	InitItem();

	// 4�w�i�̏���������
	InitBG();				

	// 5�X�e�[�W�̏���������
	InitStage();

	// 6�v���C���[�̏���������
	InitPlayer();

	// 7�̗͕\���̏���������
	InitHeart();

	// 8�c�@�\���̏���������
	InitStock();

	// 9�e�̏�����
	InitBullet();		
	
	// 10�����̏���������
	InitExplosion();	

	// 11�G�̏���������
	InitEnemy();			

	// 12�X�R�A�̏���������
	InitScore();

	// 13�G�t�F�N�g�̏���������
	InitEffect();

	// 14�e�̏�����
	InitParticle();

	// 15�Q�[�W�̏���������
	InitGauge();

	// 16�^�C�}�[�̏���������
	InitTimer();

	// 17�|�[�Y��ʂ̏���������
	InitPause();

	// 18���R���Ńe�N�X�`���̏���������
	InitPlaceChar();

	//19
	InitNomoretime();

	//20
	InitShowDifficulty();

	SetScore(NULL);

	SetTimer(GAUGE_MAX);

	// 21�Q�[�����g�̏���������
	InitPlayerFrame();

	//22
	InitFadeStage();

	//23
	InitTeleport();

	//24
	InitCaution();

	if (g_bUseTutorial == true)
	{
		//25
		InitTutorial();

		//26
		InitTutorialGate();

		SetTutorial();
	}
	else
	{
		// ���[�h�ݒ�
		SetStage(STAGE_GRASS);
		SetBG(STAGE_GRASS);
	}
}

//================================================================================================================
// �Q�[���̏I������
//================================================================================================================
void UninitGame(void)
{
	XINPUT_VIBRATION* pState;

	// �e��I�u�W�F�N�g�̏I������

	// 1�Q�[�W�̏I������
	UninitGauge();

	// 2�w�i�̏I������
	UninitBG();

	// 3Mage
	UninitMage();

	// 4Item
	UninitItem();

	// 5�X�e�[�W�̏I������
	UninitStage();

	// 6�v���C���[�̏I������
	UninitPlayer();

	// 7�̗͕\���̏I������
	UninitHeart();

	// 8�c�@�\���̏I������
	UninitStock();

	// 9�e�̏I������
	UninitBullet();

	// 10�p�[�e�B�N���̏�����
	UninitParticle();

	// 11�G�̏I������
	UninitEnemy();			

	// 12�����̏I������
	UninitExplosion();		

	// 13�G�t�F�N�g�̏I������
	UninitEffect();

	// 14�X�R�A�̏I������
	UninitScore();			

	// 15�^�C�}�[�̏I������
	UninitTimer();

	// 16�u���b�N�̏I������
	UninitBlock();

	// 17�X�e�[�W�t�F�[�h�̏I������
	UninitFadeStage();

	// 18�|�[�Y��ʂ̏I������
	UninitPause();

	// 19���R���Ńe�N�X�`���̏I������
	UninitPlaceChar();

	//20
	UninitShowDifficulty();

	//21
	UninitTutorial();

	//22
	UninitTutorialGate();

	// 23�Q�[�����g�̏I������
	UninitPlayerFrame();

	//24
	UninitNomoretime();

	//25
	UninitTeleport();

	//26
	UninitCaution();

	// �T�E���h�̒�~
	StopSound();

	// �o�C�u���[�V�����̒�~
	pState = GetJoyVibration();
	pState->wLeftMotorSpeed = 0;
	pState->wRightMotorSpeed = 0;
	XInputSetState(0, pState);
}

//================================================================================================================
// �Q�[���̍X�V����
//================================================================================================================
void UpdateGame(void)
{
	switch (g_gameState)
	{
	case GAMESTATE_NORMAL:

		break;

	case GAMESTATE_BADEND:

		g_nCounterState--;
		if (g_nCounterState <= 0)
		{
			if (GetFade() == FADE_NONE)
			{
				AddBonusScore();
				SetFade(MODE_GAMEOVER, FADE_TYPE_NORMAL);
				FadeSound(SOUND_LABEL_GAMEOVER);
			}
		}

		break;

	case GAMESTATE_CLEAREND:

		g_nCounterState--;
		if (g_nCounterState <= 0)
		{
			if (GetFade() == FADE_NONE)
			{
				AddBonusScore();
				SetFade(MODE_GAMECLEAR, FADE_TYPE_NORMAL);
				FadeSound(SOUND_LABEL_GAMECLEAR);
			}
		}
		
		break;
	}

	if (g_bPause == false)
	{// g_bPause��false�ł͂Ȃ��ƁA�|�[�Y
		// �e��I�u�W�F�N�g�̍X�V����

		// �v���C���[�̍X�V����
		UpdatePlayer();

		// Mage
		UpdateMage();

		// Item
		UpdateItem();

		// �w�i�̍X�V����
		UpdateBG();

		if (g_bUseTutorial == true)
		{
			UpdateTutorial();

			UpdateTutorialGate();
		}
		else
		{
			// �X�e�[�W�̍X�V����
			UpdateStage();

			// �X�e�[�W�t�F�[�h�̍X�V����
			UpdateFadeStage();

			g_nCounter++;
			if (((g_nCounter % 60) == 0) 
				&& GetFadeStage() == FADESTAGE_NONE
				&& g_gameState == GAMESTATE_NORMAL)
			{
				AddTimer(-1);
				if (GetTimer() <= 0)
				{
					SetGameState(GAMESTATE_BADEND, 60);
				}
			}
		}

		// �G�̍X�V����
		UpdateEnemy();			

		// �u���b�N�̍X�V����
		UpdateBlock();

		// �̗͕\���̍X�V����
		UpdateHeart();

		// �c�@�\���̍X�V����
		UpdateStock();

		// �e�̍X�V����
		UpdateBullet();

		// �G�t�F�N�g�̍X�V����
		UpdateEffect();

		// �p�[�e�B�N���̏�����
		UpdateParticle();

		// �����̍X�V����
		UpdateExplosion();		

		// �X�R�A�̍X�V����
		UpdateScore();			

		// �Q�[�W�̍X�V����
		UpdateGauge();

		// �^�C�}�[�̍X�V����
		UpdateTimer();

		// ���R���Ńe�N�X�`���̍X�V����
		UpdatePlaceChar();

		UpdateShowDifficulty();

		// �Q�[�����g�̍X�V����
		UpdatePlayerFrame();

		UpdateNomoretime(); 

		UpdateTeleport();

		UpdateCaution();
	}

	if ((GetKeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true)
		&& GetFade() == FADE_NONE
		&& GetFadeStage() == FADESTAGE_NONE)
	{// �|�[�Y��Ԃ̐؂�ւ�
		if (GetPause() == PAUSE_NONE)
		{
			g_bPause = g_bPause ^ true;
			SetPause(g_bPause);
		}
	}

	if (GetFade() == FADE_NONE)
	{
		// �|�[�Y��ʂ̍X�V����
		UpdatePause();
	}
}

//================================================================================================================
// �Q�[���̕`�揈��
//================================================================================================================
void DrawGame(void)
{
	// �e��I�u�W�F�N�g�̕`�揈��

	// �w�i�̕`�揈��
	DrawBG();

	if (g_bUseTutorial == true)
	{
		DrawTutorial();

		DrawTutorialGate();
	}
	else
	{
		 //�X�e�[�W�̕`�揈��
		DrawStage();
	}

	DrawShowDifficulty();

	// �u���b�N�̕`�揈��
	DrawBlock();

	// Item
	DrawItem();

	// �e�̕`�揈��
	DrawBullet();

	// �G�t�F�N�g�̕`�揈��
	DrawEffect();

	// �p�[�e�B�N���̏�����
	DrawParticle();

	// �v���C���[�̕`�揈��
	DrawPlayer();

	DrawTeleport();

	// Mage
	DrawMage();

	// �����̕`�揈��
	DrawExplosion();	

	// �G�̕`�揈��
	DrawEnemy();
	
	DrawCaution();

	DrawNomoretime();

	// �Q�[�����g�̕`�揈��
	DrawPlayerFrame();

	// �X�R�A�̕`�揈��
	DrawScore();		

	// �Q�[�W�̕`�揈��
	DrawGauge();

	// �^�C�}�[�̕`�揈��
	DrawTimer();

	// �̗͕\���̕`�揈��
	DrawHeart();

	// �c�@�\���̕`�揈��
	DrawStock();

	if (g_bUseTutorial == false)
	{
		 //�X�e�[�W�t�F�[�h�̕`�揈��
		DrawFadeStage();
	}

	// ���R���Ńe�N�X�`���̕`�揈��
	DrawPlaceChar();

	// �|�[�Y��ʂ̕`�揈��
	DrawPause();
}

//================================================================================================================
// �|�[�Y��Ԃ̐ݒ菈��
//================================================================================================================
void SetGamePause(bool bUse)
{
	g_bPause = bUse;
}

//================================================================================================================
// �|�[�Y��Ԃ̎擾����
//================================================================================================================
bool GetGamePause(void)
{
	return g_bPause;
}

//================================================================================================================
// �Q�[����Ԃ̐ݒ菈��
//================================================================================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;

	g_nCounterState = nCounter;
}

//================================================================================================================
// �Q�[����Ԃ̎擾����
//================================================================================================================
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

//================================================================================================================
// �`���[�g���A���̐ݒ菈��
//================================================================================================================
void SetGameTutorial(bool bUse)
{
	g_bUseTutorial = bUse;
}

//================================================================================================================
// �`���[�g���A���̎��s�󋵂̎擾
//================================================================================================================
bool GetGameTutorial(void)
{
	return g_bUseTutorial;
}

//================================================================================================================
// �{�[�i�X���_�̒ǉ�����
//================================================================================================================
int AddBonusScore(void)
{
	int nScore = GetScore();
	int nClearedStage = GetClearStageNum();
	int nRemainingTime = GetTimer();
	int nRemainingStock = GetStock();
	int nKillcountEnemy = GetKillcountEnemy();
	bool bHaveSecretItem = GetHaveSecret();

	switch (g_gameDifficulty)
	{
	case GAMEDIFFICULTY_EASY:

		if (nClearedStage < STAGE_MAX)
		{
			nScore += (nClearedStage * 5000);					// 1�X�e�[�W�N���A�����+20000
		}
		else if (nClearedStage == STAGE_MAX)
		{
			nScore += 60000;									// �S�N�������+200000
			if (GetPlayer()->nLife == MAX_LIFE && GetPlayer()->nStock == MAX_STOCK)
			{
				nScore += 15000;								// ��x���_���[�W���H��킸�ɃN���A�����ꍇ��+15000
			}
		}

		nScore += (nRemainingTime * 500) * nRemainingStock;		// �I�����A�]���Ă������ԁ~�c��c�@���~+1000 (���񂾏ꍇ�̓m�[�J�E���g)

		break;

	case GAMEDIFFICULTY_NORMAL:

		if (nClearedStage < STAGE_MAX)
		{
			nScore += (nClearedStage * 15000);					// 1�X�e�[�W�N���A�����+20000
		}
		else if (nClearedStage == STAGE_MAX)
		{
			nScore += 150000;									// �S�N�������+200000
			if (GetPlayer()->nLife == MAX_LIFE && GetPlayer()->nStock == MAX_STOCK)
			{
				nScore += 150000;								// ��x���_���[�W���H��킸�ɃN���A�����ꍇ��+150000
			}
		}

		nScore += (nRemainingTime * 4000) * nRemainingStock;	// �I�����A�]���Ă������ԁ~�c��c�@���~+1000 (���񂾏ꍇ�̓m�[�J�E���g)

		break;

	case GAMEDIFFICULTY_HARD:

		if (nClearedStage < STAGE_MAX)
		{
			nScore += (nClearedStage * 20000);					// 1�X�e�[�W�N���A�����+20000
		}
		else if (nClearedStage == STAGE_MAX)
		{
			nScore += 200000;									// �S�N�������+200000
			if (GetPlayer()->nLife == MAX_LIFE && GetPlayer()->nStock == MAX_STOCK)
			{
				nScore += 200000;								// ��x���_���[�W���H��킸�ɃN���A�����ꍇ��+150000
			}
		}

		nScore += (nRemainingTime * 10000) * nRemainingStock;	// �I�����A�]���Ă������ԁ~�c��c�@���~+1000 (���񂾏ꍇ�̓m�[�J�E���g)

		break;
	}

	

	if (nKillcountEnemy == 0)
	{
		nScore += 100000;									// �s�E�̐��_�B����+100000 (�G����̂��|�����Ɏ��Ԑ؂ꂵ���ꍇ)
	}

	if (bHaveSecretItem == true)
	{
		nScore *= 1.75f;										// �B���A�C�e���������Ă�����A�X�R�A��1.75�{��
	}

	AddScore((nScore - GetScore()));

	return nScore;
}