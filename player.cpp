//================================================================================================================
//
// DirectX�̃v���C���[���� [player.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "bullet.h"
#include "enemy.h"
#include "score.h"
#include "sound.h"
#include "gauge.h"
#include "bg.h"
#include "stage.h"
#include "effect.h"
#include "particle.h"
#include "block.h"
#include "heart.h"
#include "stock.h"
#include "placeChar.h"
#include "mage.h"
#include "playerframe.h"

// �}�N����`
#define MAX_PLAYER_SIZE	(300)						// �v���C���[�̑傫���̍ő�l
#define MIN_PLAYER_SIZE	(50)						// �v���C���[�̑傫���̍ŏ��l
#define PLAYER_SPOWN	D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f)	// �v���C���[�̏����ʒu
#define MAX_PLAYERTEX	(4)							// �v���C���[�̃e�N�X�`���̍ő吔
#define MOVE_ACCELE		(1.55f)						// �v���C���[�̉�����
#define ROT_ACCELE		(0.01f)						// �v���C���[�̉�]�̉�����
#define SIZE_ACCELE		(1.0f)						// �v���C���[�̃T�C�Y�̑���������
#define MOVE_RESIST		(0.15f)						// �v���C���[�̌����W��(��R�l)
#define SHOT_SPD		(5)							// �v���C���[�̘A�ˊ��o�̕b��(�~���b)
#define ENEMY_X			(140.0f)					// �G��X���W�̊Ԋu
#define ENEMY_Y			(90.0f)						// �G��Y���W�̊Ԋu
#define ALL_ENEMY		(32)						// �G�̔z�u��
#define ANIM_COUNT		(6)							// �v���C���[�̃A�j���[�V�����̍X�V�b��
#define MAX_ANIM		(10)						// �A�j���[�V�����̐�

#define APPEAR_STATE	(120)						// �o�����̏�Ԏ�������
#define DAMAGE_STATE	(60)						// �q�b�g���̏�Ԏ�������
#define WAIT_STATE		(60)						// �o���ҋ@���̏�Ԏ�������
#define BARRIER_STATE	(600)						// �o���A�̎�������
#define BARRIAR_COOLDOWN	(200)					// �o���A�g�p��̃N�[���^�C��
#define UNMOVE_STATE	(LASER_LIFE - 75)			// ���[�U�[�������Ă���Ԃ̐Î~����
#define LASER_RECOIL	(-3.0f)						// ���[�U�[�̔���

#define GAUGE_COLOR		D3DXCOLOR(0.5f,0.1f,0.0f,0.5f)		// �Q�[�W�̐F
#define BARRIER_COLOR	D3DXCOLOR(0.0f,0.2f,0.8f,0.3f)		// �o���A�̐F

#define HEART_POS		D3DXVECTOR3((float)(HEART_WIDTH * 1.75f) * (nCntPlayer + 1), (float)HEART_HEIGHT, 0.0f)		// �n�[�g�̈ʒu
#define STOCK_POS		D3DXVECTOR3((float)BLOCK_WIDTH, (float)(BLOCK_HEIGHT * 3), 0.0f)							// �X�g�b�N�̈ʒu
#define STOCK_NUMBER_POS	D3DXVECTOR3(STOCK_POS.x + 100.0f,STOCK_POS.y,0.0f)

#define PARTICLE_COUNTER	(30)					// �`���[�W�V���b�g��̃p�[�e�B�N���̔����������鎞��
#define EFFECT_COUNTER		(70)					// �O�ՃG�t�F�N�g�̎�������
#define EFFECT_SIZE			(30)					// �O�ՃG�t�F�N�g�̃T�C�Y

#define ANIMATION_SPD		(15)					// �A�j���[�V�����̑��x
#define BRINK_SPD			(ANIMATION_SPD * 8)		// �u���̑��x
#define ANIMATION_START		(30)					// �Î~�A�j���[�V�����̊J�n�t���[��

#define BULLET_TEX			(30)					// �ˌ���̃e�N�X�`���̎�������

// �e�N�X�`���A�j���[�V�����̎��
typedef enum
{
	TEXTURE_ANIMATION_0 = 0,			// ��ԏ�̃e�N�X�`��
	TEXTURE_ANIMATION_1,				// ��Ԗڂ̃e�N�X�`��
	TEXTURE_ANIMATION_2,				// �O�Ԗڂ̃e�N�X�`��
	TEXTURE_ANIMATION_BRINK,			// �u��
	TEXTURE_ANIMATION_STOP0,			// �Î~�Ńe�N�X�`��1
	TEXTURE_ANIMATION_STOP1,			// �Î~�Ńe�N�X�`��2
	TEXTURE_ANIMATION_STOP2,			// �Î~�Ńe�N�X�`��3
	TEXTURE_ANIMATION_MAX
}TEXTURE_ANIMATION;

// �v���g�^�C�v�錾
void KeyboardPress(void);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTexturePlayer = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;				// ���_�o�b�t�@�̃|�C���^
PLAYER g_player;
D3DXVECTOR3 g_posPast;											// �v���C���[�̏����ʒu		
D3DXVECTOR3 g_damageMovePlayer;									// �_���[�W�𕉂����Ƃ��ɍ��E�ɗh���傫��
XINPUT_VIBRATION* g_pJoyVibration;
XINPUT_STATE* g_pState = GetJoypadState();
PLAYERSTATE g_subState;
int g_nCounterSubState;
TEXTURE_ANIMATION g_nCounterTexAnimationPlayer;					// �e�N�X�`���A�j���[�V��������y���W�����߂�l
int g_nCntTexYpos;												// �e�N�X�`�����W�̑��₵��(1 or -1)
int g_nCounterPlayer;											// �ėp�J�E���^�[
int g_nCounterPlayerStop;										// �v���C���[���Î~���Ă���Ƃ��̃A�j���[�V�����p�^�[��
int g_nCounterAnimationPlayer;									// �v���C���[�����͂��~�߂Ă��牽�t���[����������
int g_nCounterBulletTime;										// �e�������Ă���̑ҋ@����
int g_nCounterBulletChargeTime;									// �`���[�W�Z�𗭂߂Ă��鎞��
bool g_bUseFullburst;											// �t���o�[�X�g���Q�[�����J�n���Ă����x�ł��g������
bool g_bShotBullet;												// �e����������

//================================================================================================================
// �v���C���[�̏���������
//================================================================================================================
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^ + �f�o�C�X�̎擾

	/*�\���̂̏�����*/
	g_player.posPlayer = PLAYER_SPOWN;			// �v���C���[�ʒu�̏�����
	g_player.movePlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �v���C���[�ړ��ʂ̏�����
	g_player.moveposPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �v���C���[�̑��Έړ��ʂ̏�����
	g_player.type = SHOTTYPE_LASER;			// �`���[�W�V���b�g�̏�����
	g_player.fBulletSpeed = LASER_SPD;				// �`���[�W�V���b�g�̒e���̏�����
	g_player.nBulletLife = LASER_LIFE;					// �`���[�W�Z�̗̑͂̏�����
	g_player.nCounterAnimPlayer = 0;			// �J�E���^�[�̏�����
	g_player.nPatternAnimPlayer = 0;			// �A�j���[�V����No�̏�����
	g_player.nLife = MAX_LIFE;					// �v���C���[�̗̑͂̏�����
	g_player.nStock = MAX_STOCK;				// �v���C���[�̎c�@�̏�����
	g_player.rotPlayer = PLAYERDIRECTION_RIGHT;	// �v���C���[�̌����̏�����(�E����)
	g_player.tex = PLAYERTEX_DOWN;				
	g_player.state = PLAYERSTATE_APPEAR;		// �v���C���[�̏�Ԃ̏�����
	g_player.nCounterState = APPEAR_STATE;		// ��Ԏ������Ԃ�������(�o�����ɐݒ�)
	g_player.bDisp = true;						// �v���C���[�̕\����Ԃ�������(�\��)
	g_player.nGaugeNo = NOMORE_GAUGE;			// �Q�[�W�̎g�p��Ԃ�������
	g_player.nCounterBullPlayer = 0;			// �e�̑��x�̏�����
	g_player.nCounterBulletCharge = 0;			// �Q�[�W�̏�����
	g_player.nCounterBarrier = 0;
	g_player.nCounterBarrierTime = 0;
	g_player.nBarrierNo = NOMORE_GAUGE;
	g_player.nCounterCoolTime = 0;
	g_player.nBlockStockID = 0;
	g_player.nCounterParticle = 0;				
	g_pJoyVibration = GetJoyVibration();
	g_player.g_nIDDirection = -1;
	g_player.bCharge = false;
	g_player.bHaveKey = false;

	for (int nCntPlayer = 0; nCntPlayer < PLAYERDO_MAX; nCntPlayer++)
	{
		g_player.aCouldDo[nCntPlayer] = false;
	}

	// �Ίp���̒������擾
	g_player.fLengthPlayer = sqrtf(powf(PLAYER_SIZE,2.0f) + powf(PLAYER_SIZE, 2.0f)) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_player.fAnglePlayer = atan2f(PLAYER_SIZE, PLAYER_SIZE);
	/**/

	g_posPast = g_player.posPlayer;
	g_subState = PLAYERSTATE_NORMAL;
	g_nCounterSubState = 0;
	g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_1;
	g_nCntTexYpos = 1;
	g_nCounterPlayer = 0;
	g_nCounterPlayerStop = 0;
	g_nCounterAnimationPlayer = 0;
	g_nCounterBulletTime = 0;
	g_nCounterBulletChargeTime = 0;
	g_bShotBullet = false;
	g_damageMovePlayer = D3DXVECTOR3_NULL;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\PLAYER_3.png",
							  &g_pTexturePlayer);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffPlayer,
								NULL);

	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * -0.75f) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * -0.75f) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * 0.75f) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * 0.75f) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * -0.25f) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * -0.25f) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.posPlayer.x + sinf(D3DX_PI * 0.25f) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.posPlayer.y + cosf(D3DX_PI * 0.25f) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	if (g_nCounterTexAnimationPlayer >= TEXTURE_ANIMATION_MAX || g_nCounterTexAnimationPlayer < TEXTURE_ANIMATION_0)
	{
		g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_1;
		g_nCntTexYpos *= -1;
	}

	pVtx[0].tex = D3DXVECTOR2((0.125f * g_player.tex), (0.125f * g_nCounterTexAnimationPlayer));
	pVtx[1].tex = D3DXVECTOR2((0.125f * g_player.tex) + 0.125f, (0.125f * g_nCounterTexAnimationPlayer));
	pVtx[2].tex = D3DXVECTOR2((0.125f * g_player.tex), (0.125f * g_nCounterTexAnimationPlayer) + 0.125f);
	pVtx[3].tex = D3DXVECTOR2((0.125f * g_player.tex) + 0.125f, (0.125f * g_nCounterTexAnimationPlayer) + 0.125f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//================================================================================================================
// �v���C���[�̏I������
//================================================================================================================
void UninitPlayer(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//================================================================================================================
// �v���C���[�̍X�V����
//================================================================================================================
void UpdatePlayer(void)
{
	bool bMove = false;							// ���ݑ��삵�Ă��邩
	VERTEX_2D* pVtx;							// ���_���ւ̃|�C���^
	Gauge *pGauge = GetGauge();						

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_player.state)
	{
	case PLAYERSTATE_APPEAR:
		// �o����
		if (g_player.nCounterState == APPEAR_STATE)
		{
			ChangeModeFrame(FRAMESTATE_APPEAR);
		}
		g_player.nCounterState--;

		KeyboardPress();

		// �_�ŏ���
		if ((g_player.nCounterState % 5) == 0)
		{
			// �\��
			g_player.bDisp = true;
		}
		else
		{
			// ��\��
			g_player.bDisp = false;
		}

		if (g_player.nCounterState <= 0)
		{// ��莞�Ԍo�ߌ�ʏ��Ԃ֑J��
			g_player.state = PLAYERSTATE_NORMAL;
			
			// HP�A�C�R����ݒu
			for (int nCntPlayer = 0; nCntPlayer < MAX_LIFE; nCntPlayer++)
			{
				// �̗͂̔z�u
				SetHeart();
				// Mage�̔z�u
				SetMage();
				if (g_player.nStock == MAX_STOCK)
				{// �c�@���}�b�N�X�̎�
					// �c�@�̔z�u
					SetStock();
				}
			}

			for (int nCntPlayer = 0; nCntPlayer < PLAYERDO_MAX; nCntPlayer++)
			{
				g_player.aCouldDo[nCntPlayer] = true;
			}
		}

		break;

	case PLAYERSTATE_NORMAL:
		// �ʏ펞
		if (g_subState != PLAYERSTATE_UNMOVE)
		{
			KeyboardPress();
		}

		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		g_player.bDisp = true;

		break;

	case PLAYERSTATE_DAMAGE:
		// ��e��
		g_nCounterAnimationPlayer = 0;

		if (g_subState != PLAYERSTATE_UNMOVE)
		{
			KeyboardPress();
		}

		// �o����
		g_player.nCounterState--;
		// �_�ŏ���
		if ((g_player.nCounterState % 5) == 0)
		{
			// �\��
			g_player.bDisp = true;
			if (g_player.nCounterState % 10 == 0)
			{
				g_damageMovePlayer.x = -5.0f;
			}
			else if (g_player.nCounterState % 5 == 0)
			{
				g_damageMovePlayer.x = 5.0f;
			}
		}
		else
		{
			// ��\��
			g_player.bDisp = false;
		}

		if (g_player.nCounterState <= 0)
		{// ��莞�Ԍo�ߌ�ʏ��Ԃ֑J��
			g_damageMovePlayer.x = 0.0f;
			g_player.state = PLAYERSTATE_NORMAL;
		}

		break;

	case PLAYERSTATE_WAIT:
		// ���S��
		 
		if (g_player.nCounterState == WAIT_STATE)
		{
			ChangeModeFrame(FRAMESTATE_DEATH);
		}

		// ��\����
		g_player.bDisp = false;
		EnableMageDraw(false);

		StopSound(SOUND_LABEL_SE_FULLCHARGE);
		AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
		g_player.nCounterBulletCharge = 0;
		g_player.nCounterBarrier = 0;
		g_pJoyVibration->wLeftMotorSpeed = 0;

		g_pJoyVibration->wRightMotorSpeed = 0;

		XInputSetState(0, g_pJoyVibration);

		if (g_player.nStock > 0)
		{// �c�@������ꍇ
			g_player.nCounterState--;
			if (g_player.nCounterState <= 0)
			{
				g_player.state = PLAYERSTATE_APPEAR;
				g_player.nLife = MAX_LIFE;
				g_player.nCounterState = APPEAR_STATE;
				g_player.aCouldDo[PLAYERDO_CHARGE] = false;
				g_player.aCouldDo[PLAYERDO_BARRIAR] = false;
			}
		}
		else
		{// �c�@���Ȃ��Ȃ����ꍇ
			g_player.state = PLAYERSTATE_DEATH;

			SetGameState(GAMESTATE_BADEND, 60);
		}

		break;

	case PLAYERSTATE_DEATH:
		// �Q�[���I�[�o�[��
		// ��\����
		g_player.bDisp = false;

		break;

	case PLAYERSTATE_BARRIER:


		ChangeModeFrame(FRAMESTATE_BARRIAR);

		if (g_subState != PLAYERSTATE_UNMOVE)
		{
			KeyboardPress();
		}

		// �\��
		g_player.bDisp = true;

		break;

	case PLAYERSTATE_UNMOVE:

		break;

	case PLAYERSTATE_TELEPORT_COOLDOWN:
		// �N�[���^�C�����̃��[�U�[�̔����y���͍�҂̈Ӑ}�ɂ��o�O�ł͂Ȃ��d�l�Ƃ��Ĉ����܂�
		KeyboardPress();

		g_player.nCounterState--;
		if (g_player.nCounterState <= 0)
		{
			g_player.state = PLAYERSTATE_NORMAL;
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
		}

		break;

	default:

		break;
	}

	if (g_subState == PLAYERSTATE_UNMOVE)
	{
		g_nCounterAnimationPlayer = 0;

		if (g_nCounterBulletTime >= 0)
		{

			g_nCounterBulletChargeTime++;
			if (g_nCounterBulletChargeTime % 12 == 0)
			{
				g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
			}
		}

		g_nCounterSubState--;
		if (g_nCounterSubState <= 0)
		{
			g_subState = PLAYERSTATE_NORMAL;
		}
	}

	if (g_player.nCounterCoolTime > 0 && g_player.state != PLAYERSTATE_BARRIER)
	{
		g_player.nCounterCoolTime--;
		if (g_player.nCounterCoolTime < 0)
		{
			g_player.nCounterCoolTime = 0;
		}
	}

	GetPlayerRot(&g_player);

	if (g_player.nCounterParticle > 0)
	{
		SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 10.0f),
			PLAYER_BULLET_SIZE * (0.125f * (float)(rand() % 9)),
			300,
			D3DX_PI,
			-D3DX_PI,
			false);

		SetParticle(g_player.posPlayer,
			D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
			D3DXVECTOR3(0.0f, 0.0f, 20.0f),
			PLAYER_BULLET_SIZE* (0.15f * (float)(rand() % 9)),
			200,
			D3DX_PI,
			-D3DX_PI,
			false);

		g_player.nCounterParticle--;
	}
	
	// �ʒu���X�V
	g_player.posPlayer.x += g_player.movePlayer.x;
	g_player.posPlayer.y += g_player.movePlayer.y;

	if ((g_player.posPlayer.x) <= STAGE_LEFT + (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.x = STAGE_LEFT + (PLAYER_SIZE * 0.5f);
		g_player.movePlayer.x = 0.0f;
	}

	if (g_player.posPlayer.x >= STAGE_RIGHT - (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.x = (float)(STAGE_RIGHT - (PLAYER_SIZE * 0.5f));
		g_player.movePlayer.x = 0.0f;
	}

	if (g_player.posPlayer.y <= STAGE_CEILING + (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.y = STAGE_CEILING + (PLAYER_SIZE * 0.5f);
		g_player.movePlayer.y = 0.0f;
	}

	if (g_player.posPlayer.y > STAGE_FLOOR - (PLAYER_SIZE * 0.5f))
	{
		g_player.posPlayer.y = (float)(STAGE_FLOOR - (PLAYER_SIZE * 0.5f));
		g_player.movePlayer.y = 0.0f;
	}

	// �ړ��ʂ��X�V(��������)
	g_player.movePlayer.x += (0.0f - g_player.movePlayer.x) * MOVE_RESIST;
	g_player.movePlayer.y += (0.0f - g_player.movePlayer.y) * MOVE_RESIST;

	int n = 0;

	if (g_posPast != g_player.posPlayer 
		&& g_player.state != PLAYERSTATE_WAIT 
		&& g_player.state != PLAYERSTATE_DEATH
		&& (GetKeyboardWASD() == true
		|| GetJoypadWASD() == true
		|| GetJoyThumbWASD() == true))
	{
		float fAngle[2] = {};
		D3DXVECTOR3 particlePlayer = D3DXVECTOR3(g_player.posPlayer.x, g_player.posPlayer.y + (PLAYER_SIZE * 0.45f), g_player.posPlayer.z);
		fAngle[0] = GetPlayerRot(&g_player) - (D3DX_PI * 0.25f);
		fAngle[1] = GetPlayerRot(&g_player) + (D3DX_PI * 0.25f);
		// �v���C���[�̋O�Ղ���o
		SetParticle(particlePlayer, D3DXCOLOR(0.85f, 0.81f, 0.51f, 1.0f), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 7.0f, 30, D3DX_PI, -D3DX_PI, true);
		SetParticle(particlePlayer, D3DXCOLOR(0.85f, 0.81f, 0.51f, 1.0f), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 7.0f, 30, D3DX_PI, -D3DX_PI, true);
		SetParticle(particlePlayer, D3DXCOLOR(0.85f, 0.81f, 0.51f, 1.0f), D3DXVECTOR3(2.0f, 2.0f, 2.0f), 7.0f, 30, D3DX_PI, -D3DX_PI, true);
	}

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
#if 0
	pVtx[0].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) - g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_player.posPlayer.x + sinf(GetPlayerRot(&g_player) + (g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.y = g_player.posPlayer.y + cosf(GetPlayerRot(&g_player) + (g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;


	g_player.nCounterAnimPlayer++;				// �J�E���^�\�����Z
	if ((g_player.nCounterAnimPlayer % ANIM_COUNT) == 0)
	{//��莞�Ԍo�ߌ�
		g_player.nPatternAnimPlayer = (g_player.nPatternAnimPlayer + 1) % MAX_ANIM;			// �A�j���[�V����No�����Z

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(((g_player.nPatternAnimPlayer % 5) * 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f));
		pVtx[1].tex = D3DXVECTOR2((((g_player.nPatternAnimPlayer % 5) * 0.2f) + 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f));
		pVtx[2].tex = D3DXVECTOR2(((g_player.nPatternAnimPlayer % 5) * 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f) + 0.5f);
		pVtx[3].tex = D3DXVECTOR2((((g_player.nPatternAnimPlayer % 5) * 0.2f) + 0.2f), ((g_player.nPatternAnimPlayer / 5) * 0.5f) + 0.5f);
	}

#endif

	// �e�N�X�`�����W�̐ݒ�
	if ((g_nCounterTexAnimationPlayer >= TEXTURE_ANIMATION_BRINK || g_nCounterTexAnimationPlayer < TEXTURE_ANIMATION_0)
		&& g_nCounterAnimationPlayer < ANIMATION_START)
	{
		g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_1;
		g_nCntTexYpos *= -1;
	}
	
	if (g_bShotBullet == true && g_nCounterBulletTime <= 0)
	{
		g_bShotBullet = false;
		g_player.tex = (PLAYERTEX)(g_player.tex - 4);
	}
	else if (g_nCounterBulletTime > 0)
	{
		g_nCounterBulletTime--;
	}

	if (g_nCounterAnimationPlayer >= ANIMATION_START)
	{ // �v���C���[�̓��͂��~�܂�A��莞�Ԃ�������e�N�X�`���A�j���[�V�����𒆎~
		if (g_nCounterPlayer != 0)
		{
			if (g_player.tex == PLAYERTEX_LEFT 
				|| g_player.tex == PLAYERTEX_RIGHT 
				|| g_player.tex == PLAYERTEX_LEFT_ATK 
				|| g_player.tex == PLAYERTEX_RIGHT_ATK)
			{
				g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_0;
				g_nCounterPlayer = 0;
			}
			else if (g_player.tex == PLAYERTEX_UP 
				|| g_player.tex == PLAYERTEX_DOWN
				|| g_player.tex == PLAYERTEX_UP_ATK
				|| g_player.tex == PLAYERTEX_DOWN_ATK)
			{
				g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_1;
				g_nCounterPlayer = 0;
			}
		}

		if (g_nCounterAnimationPlayer >= (ANIMATION_START * 2.0f))
		{
			// �v���C���[�̌����ɉ����āA�Î~�A�j���[�V�������Đ�
			if (g_nCounterAnimationPlayer % (ANIMATION_SPD * 4) == 0)
			{
				g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
			}

			if ((g_nCounterTexAnimationPlayer >= TEXTURE_ANIMATION_MAX || g_nCounterTexAnimationPlayer < TEXTURE_ANIMATION_STOP0))
			{
				g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_STOP1;
				g_nCntTexYpos *= -1;
			}
		}
	}

	g_nCounterAnimationPlayer++;

	pVtx[0].pos.x = (WINDOW_MID.x + g_damageMovePlayer.x) + sinf((D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.y = WINDOW_MID.y + cosf((D3DX_PI + g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (WINDOW_MID.x + g_damageMovePlayer.x) + sinf((D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.y = WINDOW_MID.y + cosf((D3DX_PI - g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (WINDOW_MID.x + g_damageMovePlayer.x) + sinf(-g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.y = WINDOW_MID.y + cosf(-g_player.fAnglePlayer) * g_player.fLengthPlayer;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (WINDOW_MID.x + g_damageMovePlayer.x) + sinf((g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.y = WINDOW_MID.y + cosf((g_player.fAnglePlayer)) * g_player.fLengthPlayer;
	pVtx[3].pos.z = 0.0f;

	pVtx[0].tex = D3DXVECTOR2((0.125f * g_player.tex), (0.125f * g_nCounterTexAnimationPlayer));
	pVtx[1].tex = D3DXVECTOR2((0.125f * g_player.tex) + 0.125f, (0.125f * g_nCounterTexAnimationPlayer));
	pVtx[2].tex = D3DXVECTOR2((0.125f * g_player.tex), (0.125f * g_nCounterTexAnimationPlayer) + 0.125f);
	pVtx[3].tex = D3DXVECTOR2((0.125f * g_player.tex) + 0.125f, (0.125f * g_nCounterTexAnimationPlayer) + 0.125f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();

	g_player.moveposPlayer.x = PLAYER_SPOWN.x - g_player.posPlayer.x;
	g_player.moveposPlayer.y = PLAYER_SPOWN.y - g_player.posPlayer.y;

#if 1
	g_posPast = g_player.posPlayer;
#endif
}

//================================================================================================================
// �v���C���[�̕`�揈��
//================================================================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayer);

	if (g_player.bDisp == true)
	{// �\������Ȃ�
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

// �v���C���[���擾
PLAYER *GetPlayer(void)
{
	return &g_player;
}

// �v���C���[�̌�����float�ɕϊ�
float GetPlayerRot(PLAYER* pPlayer)
{
	BLOCK* pBlock = GetBlock(g_player.g_nIDDirection);

	switch (pPlayer->rotPlayer)
	{
	case PLAYERDIRECTION_UP:
		
		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_UP;
		}
		else
		{
			g_player.tex = PLAYERTEX_UP_ATK;
		}

		if (g_nCounterSubState == UNMOVE_STATE)
		{
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x = 0.0f;
			g_player.movePlayer.y = -LASER_RECOIL;
		}

		// �����
		return 0.0f;

		break;

	case PLAYERDIRECTION_DOWN:

		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_DOWN;
		}
		else
		{
			g_player.tex = PLAYERTEX_DOWN_ATK;
		}
		
		if (g_nCounterSubState == UNMOVE_STATE)
		{
			g_player.movePlayer.x = 0.0f;
			g_player.movePlayer.y = LASER_RECOIL;
		}

		// ������
		return D3DX_PI;
	
		break;
		
	case PLAYERDIRECTION_LEFT:

		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_LEFT;
		}
		else
		{
			g_player.tex = PLAYERTEX_LEFT_ATK;
		}

		if (g_nCounterSubState == UNMOVE_STATE)
		{
			g_player.movePlayer.x = -LASER_RECOIL;
			g_player.movePlayer.y = 0.0f;
		}

		// ������
		return (D3DX_PI * 0.5f);

		break;

	case PLAYERDIRECTION_RIGHT:
		// �E����

		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_RIGHT;
		}
		else
		{
			g_player.tex = PLAYERTEX_RIGHT_ATK;
		}

		if (g_nCounterSubState == UNMOVE_STATE)
		{
			g_player.movePlayer.x = LASER_RECOIL;
			g_player.movePlayer.y = 0.0f;
		}

		return (D3DX_PI * -0.5f);

		break;

	case PLAYERDIRECTION_UPLEFT:
		// �E����

		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_UP;
		}
		else
		{
			g_player.tex = PLAYERTEX_UP_ATK;
		}

		if (g_nCounterSubState == UNMOVE_STATE)
		{
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x = sinf(D3DX_PI * -0.75f) * LASER_RECOIL;
			g_player.movePlayer.y = cosf(D3DX_PI * -0.75f) * LASER_RECOIL;
		}

		return (D3DX_PI * 0.25f);

		break;

	case PLAYERDIRECTION_UPRIGHT:
		// �E����
		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_UP;
		}
		else
		{
			g_player.tex = PLAYERTEX_UP_ATK;
		}

		if (g_nCounterSubState == UNMOVE_STATE)
		{
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x = sinf(D3DX_PI * 0.75f) * LASER_RECOIL;
			g_player.movePlayer.y = cosf(D3DX_PI * 0.75f) * LASER_RECOIL;
		}

		return (D3DX_PI * -0.25f);

		break;

	case PLAYERDIRECTION_DOWNLEFT:
		// �E����
		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_DOWN;
		}
		else
		{
			g_player.tex = PLAYERTEX_DOWN_ATK;
		}

		if (g_nCounterSubState == UNMOVE_STATE)
		{
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x = sinf(D3DX_PI * -0.25f) * LASER_RECOIL;
			g_player.movePlayer.y = cosf(D3DX_PI * -0.25f) * LASER_RECOIL;
		}

		return (D3DX_PI * 0.75f);

		break;

	case PLAYERDIRECTION_DOWNRIGHT:
		// �E����
		if (g_bShotBullet == false)
		{
			g_player.tex = PLAYERTEX_DOWN;
		}
		else
		{
			g_player.tex = PLAYERTEX_DOWN_ATK;
		}

		if (g_nCounterSubState == UNMOVE_STATE)
		{
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x = sinf(D3DX_PI * 0.25f) * LASER_RECOIL;
			g_player.movePlayer.y = cosf(D3DX_PI * 0.25f) * LASER_RECOIL;
		}

		return (D3DX_PI * -0.75f);

		break;

	default:

		g_player.tex = PLAYERTEX_DOWN;

		return D3DX_PI;

		break;
	}
}

// �v���C���[�̔�e����
void HitPlayer(int nCntDamage)
{
	if (g_player.state == PLAYERSTATE_NORMAL)
	{
		g_player.nLife -= nCntDamage;

		AddHeart(-1);

		if (g_player.nLife <= 0)
		{// �v���C���[��HP��0�ɂȂ�����
			g_player.nStock--;
			AddStock(-1);
			g_player.state = PLAYERSTATE_WAIT;
			g_player.nCounterState = WAIT_STATE;
			PlaySound(SOUND_LABEL_SE_EXPLOSION);

			AddScore(-1000);
		}
		else if (g_player.nLife > 0 && g_player.nLife < MAX_LIFE)
		{// �v���C���[��HP��MAX�ȉ��ɂȂ�����
			PlaySound(SOUND_LABEL_SE_HIT);
			g_player.state = PLAYERSTATE_DAMAGE;

			g_player.nCounterState = DAMAGE_STATE;

			AddScore(-10);

			VERTEX_2D* pVtx;
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

			g_pVtxBuffPlayer->Unlock();
		}
	}
}

// �L�[�{�[�h�̓��͏���
void KeyboardPress(void)
{
	Gauge* pGauge = GetGauge();
	g_pState = GetJoypadState();

	if (g_player.aCouldDo[PLAYERDO_MOVE] == true)
	{
		// �L�[�{�[�h���͂̊e����
		if (GetKeyboardPress(DIK_W) == true
			|| GetJoypadPress(JOYKEY_UP) == true
			|| (GetJoyThumbLYState() == true && g_pState->Gamepad.sThumbLY >= 12500.0f))
		{// W���������Ƃ�
			// �ǉ�����
			if (GetKeyboardPress(DIK_A) == true
				|| GetJoypadPress(JOYKEY_LEFT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -12500.0f))
			{// ����ړ�
				// �v���C���[�̈ړ��ʂ��X�V
				g_player.movePlayer.x += sinf(D3DX_PI * -0.75f) * MOVE_ACCELE;
				g_player.movePlayer.y += cosf(D3DX_PI * -0.75f) * MOVE_ACCELE;

				// �v���C���[�̌������X�V
				g_player.rotPlayer = PLAYERDIRECTION_UPLEFT;
			}
			else if (GetKeyboardPress(DIK_D) == true
				|| GetJoypadPress(JOYKEY_RIGHT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 12500.0f))
			{// �E��ړ�
				// �v���C���[�̈ړ��ʂ��X�V
				g_player.movePlayer.x += sinf(D3DX_PI * 0.75f) * MOVE_ACCELE;
				g_player.movePlayer.y += cosf(D3DX_PI * 0.75f) * MOVE_ACCELE;

				// �v���C���[�̌������X�V
				g_player.rotPlayer = PLAYERDIRECTION_UPRIGHT;
			}
			else
			{// ��ֈړ�
				// �v���C���[�̈ړ��ʂ��X�V
				g_player.movePlayer.y -= MOVE_ACCELE;

				// �v���C���[�̌������X�V
				g_player.rotPlayer = PLAYERDIRECTION_UP;
			}

			if (g_nCounterPlayer == 0)
			{
				g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_1;
			}

			g_nCounterPlayer++;
			if (g_nCounterPlayer % ANIMATION_SPD == 0)
			{
				g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
			}

			g_nCounterAnimationPlayer = 0;
		}
		else if (GetKeyboardPress(DIK_S) == true
			|| GetJoypadPress(JOYKEY_DOWN) == true
			|| (GetJoyThumbLYState() == true && g_pState->Gamepad.sThumbLY <= -12500.0f))
		{// S���������Ƃ�
			// �ǉ�����
			if (GetKeyboardPress(DIK_A) == true
				|| GetJoypadPress(JOYKEY_LEFT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -12500.0f))
			{// �����ړ�
				// �v���C���[�̈ړ��ʂ��X�V
				g_player.movePlayer.x += sinf(D3DX_PI * -0.25f) * MOVE_ACCELE;
				g_player.movePlayer.y += cosf(D3DX_PI * -0.25f) * MOVE_ACCELE;

				// �v���C���[�̌������X�V
				g_player.rotPlayer = PLAYERDIRECTION_DOWNLEFT;
			}
			else if (GetKeyboardPress(DIK_D) == true
				|| GetJoypadPress(JOYKEY_RIGHT) == true
				|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 12500.0f))
			{// �E���ړ�
				// �v���C���[�̈ړ��ʂ��X�V
				g_player.movePlayer.x += sinf(D3DX_PI * 0.25f) * MOVE_ACCELE;
				g_player.movePlayer.y += cosf(D3DX_PI * 0.25f) * MOVE_ACCELE;

				// �v���C���[�̌������X�V
				g_player.rotPlayer = PLAYERDIRECTION_DOWNRIGHT;
			}
			else
			{// ���ֈړ�
				// �v���C���[�̈ړ��ʂ��X�V
				g_player.movePlayer.y += MOVE_ACCELE;

				// �v���C���[�̌������X�V
				g_player.rotPlayer = PLAYERDIRECTION_DOWN;
			}

			if (g_nCounterPlayer == 0)
			{
				g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_1;
			}

			g_nCounterPlayer++;
			if (g_nCounterPlayer % ANIMATION_SPD == 0)
			{
				g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
			}

			g_nCounterAnimationPlayer = 0;
		}
		else if (GetKeyboardPress(DIK_A) == true
			|| GetJoypadPress(JOYKEY_LEFT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX <= -12500.0f))
		{// A���������Ƃ�

			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x -= MOVE_ACCELE;

			// �v���C���[�̌������X�V
			g_player.rotPlayer = PLAYERDIRECTION_LEFT;

			if (g_nCounterPlayer == 0)
			{
				g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_0;
			}

			g_nCounterPlayer++;
			if (g_nCounterPlayer % ANIMATION_SPD == 0)
			{
				g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
			}

			g_nCounterAnimationPlayer = 0;
		}
		else if (GetKeyboardPress(DIK_D) == true
			|| GetJoypadPress(JOYKEY_RIGHT) == true
			|| (GetJoyThumbLXState() == true && g_pState->Gamepad.sThumbLX >= 12500.0f))
		{// D���������Ƃ�
			// �v���C���[�̈ړ��ʂ��X�V
			g_player.movePlayer.x += MOVE_ACCELE;

			// �v���C���[�̌������X�V
			g_player.rotPlayer = PLAYERDIRECTION_RIGHT;

			if (g_nCounterPlayer == 0)
			{
				g_nCounterTexAnimationPlayer = TEXTURE_ANIMATION_0;
			}

			g_nCounterPlayer++;
			if (g_nCounterPlayer % ANIMATION_SPD == 0)
			{
				g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
			}

			g_nCounterAnimationPlayer = 0;
		}
	}

	if (g_player.g_nIDDirection != -1)GetPlayerRot(&g_player);

	if (g_player.aCouldDo[PLAYERDO_SHOT] == true)
	{
		if ((GetKeyboardRepeat(DIK_SPACE) == true || GetJoypadRepeat(JOYKEY_A) == true)
			&& g_player.nCounterBarrier == 0)
		{// SPACE���������Ƃ�

			if (g_player.nCounterBulletCharge == 0)
			{
				// �ˌ�
				SetBullet(g_player.posPlayer, -15.0f, GetPlayerRot(&g_player), 300, BULLETTYPE_PLAYER, SHOTTYPE_NORMAL, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), false);
				PlaySound(SOUND_LABEL_SE_SHOT_1);
				g_player.nCounterBulletCharge++;
				g_bShotBullet = true;
			}
			else if (g_player.nCounterBulletCharge < 6)
			{
				if (g_player.aCouldDo[PLAYERDO_CHARGE] == true)
				{
					if (g_player.nGaugeNo == NOMORE_GAUGE)
					{// �Q�[�W���Z�b�g(�Ԃ�l�͊m�ۂ����Q�[�W��No)
						g_player.nGaugeNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f), GAUGE_COLOR, GAUGETYPE_HOMING, 0, 50.0f);
					}
					g_player.nCounterBulletCharge++;
				}
			}
			else if (g_player.nCounterBulletCharge >= 6 && pGauge[g_player.nGaugeNo].nPercentGauge <= GAUGE_MAX)
			{ // �Q�[�W�𑝂₷(�v���C���[���g���Ă���Q�[�W��No�̃Q�[�W)
				if (g_player.aCouldDo[PLAYERDO_CHARGE] == true)
				{
					if (g_player.nCounterBulletCharge == 6)
					{
						PlaySound(SOUND_LABEL_SE_CHARGE);
					}

					AddGauge(1, g_player.nGaugeNo);
					g_player.nCounterBulletCharge++;

					float fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);
					SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						D3DXVECTOR3(0.0f, 0.0f, -1.0f),
						PLAYER_BULLET_SIZE * (0.1f * (float)(rand() % 9)),
						50,
						atan2f(sinf(fLength), cosf(fLength)),
						atan2f(sinf(fLength), cosf(fLength)),
						false);

					fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);

					SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						D3DXVECTOR3(0.0f, 0.0f, -1.0f),
						PLAYER_BULLET_SIZE * (0.1f * (float)(rand() % 9)),
						50,
						atan2f(sinf(fLength), cosf(fLength)),
						atan2f(sinf(fLength), cosf(fLength)),
						false);

					if (g_pJoyVibration->wLeftMotorSpeed < MAX_VIBRATION)
					{
						g_pJoyVibration->wLeftMotorSpeed += 500;

						g_pJoyVibration->wRightMotorSpeed += 500;

						if (g_pJoyVibration->wLeftMotorSpeed >= MAX_VIBRATION)
						{
							g_pJoyVibration->wLeftMotorSpeed = MAX_VIBRATION;
						}

						if (g_pJoyVibration->wRightMotorSpeed >= MAX_VIBRATION)
						{
							g_pJoyVibration->wRightMotorSpeed = MAX_VIBRATION;
						}

						XInputSetState(0, g_pJoyVibration);
					}
				}
			}
			else if (g_player.bCharge == false)
			{
				if (g_player.aCouldDo[PLAYERDO_CHARGE] == true)
				{
					SetPlaceChar(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y - 50.0f, 0.0f), POLY_SIZE(400.0f, 50.0f), CHARTEX_FULLCHARGE, 120);
					g_player.bCharge = true;
					PlaySound(SOUND_LABEL_SE_FULLCHARGE);
				}
			}

			g_nCounterBulletTime = BULLET_TEX;

			g_nCounterBulletChargeTime++;
			if (g_nCounterBulletChargeTime % 12 == 0)
			{
				g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
			}

			g_nCounterAnimationPlayer = 0;
		}
		else if (GetKeyboardRelease(DIK_SPACE) == true || GetJoypadRelease(JOYKEY_A) == true)
		{// SPACE�𗣂����Ƃ�
			if (pGauge[g_player.nGaugeNo].nPercentGauge > GAUGE_MAX && g_player.aCouldDo[PLAYERDO_SP] == true)
			{
				SetBullet(g_player.posPlayer, g_player.fBulletSpeed, GetPlayerRot(&g_player), g_player.nBulletLife, BULLETTYPE_PLAYER, g_player.type, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), true);

				StopSound(SOUND_LABEL_SE_FULLCHARGE);
				PlaySound(SOUND_LABEL_SE_CHARGE_SHOT);

				if (g_player.type == SHOTTYPE_LASER || g_player.type == SHOTTYPE_FULLBURST)
				{
					g_nCounterSubState = UNMOVE_STATE;
					g_subState = PLAYERSTATE_UNMOVE;
					g_nCounterBulletTime = UNMOVE_STATE;

					GetPlayerRot(&g_player);
				}

				AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
				g_player.nCounterBulletCharge = 0;
				g_player.nCounterParticle = PARTICLE_COUNTER;

				g_pJoyVibration->wLeftMotorSpeed = 0;

				g_pJoyVibration->wRightMotorSpeed = 0;

				XInputSetState(0, g_pJoyVibration);
			}

			StopSound(SOUND_LABEL_SE_FULLCHARGE);

			g_nCounterAnimationPlayer = 0;
		}
		else
		{
			if (g_player.nCounterBulletCharge > 0)
			{
				if (g_player.nCounterBulletCharge >= 4)
				{
					StopSound(SOUND_LABEL_SE_CHARGE);

					if (pGauge[g_player.nGaugeNo].nPercentGauge > 0)
					{// �Q�[�W������
						AddGauge(-1, g_player.nGaugeNo);
					}
				}

				g_nCounterAnimationPlayer = 0;

				g_nCounterAnimationPlayer = 0;

				g_player.bCharge = false;

				g_player.nCounterBulletCharge--;

				if (g_pJoyVibration->wLeftMotorSpeed > 0)
				{
					g_pJoyVibration->wLeftMotorSpeed -= 500;

					g_pJoyVibration->wRightMotorSpeed -= 500;

					if (g_pJoyVibration->wLeftMotorSpeed <= 0)
					{
						g_pJoyVibration->wLeftMotorSpeed = 0;
					}

					if (g_pJoyVibration->wRightMotorSpeed <= 0)
					{
						g_pJoyVibration->wRightMotorSpeed = 0;
					}

					XInputSetState(0, g_pJoyVibration);
				}
			}
		}
	}

	if (g_player.aCouldDo[PLAYERDO_BARRIAR] == true && g_player.state != PLAYERSTATE_APPEAR)
	{
		if ((GetKeyboardRepeat(DIK_B) == true || GetJoypadRepeat(JOYKEY_B) == true)
			&& g_player.nCounterBulletCharge == 0
			&& g_player.state != PLAYERSTATE_BARRIER
			&& g_player.nCounterCoolTime <= 0)
		{
			if (g_player.nCounterBarrier < 6)
			{
				if (g_player.nGaugeNo == NOMORE_GAUGE)
				{// �Q�[�W���Z�b�g(�Ԃ�l�͊m�ۂ����Q�[�W��No)
					g_player.nGaugeNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f), GAUGE_COLOR, GAUGETYPE_HOMING, 0, 50.0f);
				}

				g_player.nCounterBarrier++;
			}
			else if (g_player.nCounterBarrier >= 6 && pGauge[g_player.nGaugeNo].nPercentGauge <= GAUGE_MAX)
			{
				if (g_player.nCounterBarrier == 6)
				{
					PlaySound(SOUND_LABEL_SE_CHARGE);
				}

				// �Q�[�W�𑝂₷(�v���C���[���g���Ă���Q�[�W��No�̃Q�[�W)
				AddGauge(1, g_player.nGaugeNo);
				g_player.nCounterBarrier++;

				float fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);
				SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
					D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, -1.0f),
					PLAYER_BULLET_SIZE * (0.1f * (float)(rand() % 9)),
					50,
					atan2f(sinf(fLength), cosf(fLength)),
					atan2f(sinf(fLength), cosf(fLength)),
					false);

				fLength = (float)((rand() % (int)((D3DX_PI * 10.0f) - (-D3DX_PI * 10.0f))) * 10.0f);

				SetParticle(D3DXVECTOR3(g_player.posPlayer.x + sinf(fLength) * g_player.fLengthPlayer, g_player.posPlayer.y + cosf(fLength) * g_player.fLengthPlayer, 0.0f),
					D3DXCOLOR(0.0f, 0.0f, 0.5f, 1.0f),
					D3DXVECTOR3(0.0f, 0.0f, -1.0f),
					PLAYER_BULLET_SIZE * (0.1f * (float)(rand() % 9)),
					50,
					atan2f(sinf(fLength), cosf(fLength)),
					atan2f(sinf(fLength), cosf(fLength)),
					false);

				if (g_pJoyVibration->wLeftMotorSpeed < MAX_VIBRATION)
				{
					g_pJoyVibration->wLeftMotorSpeed += 500;

					g_pJoyVibration->wRightMotorSpeed += 500;

					if (g_pJoyVibration->wLeftMotorSpeed >= MAX_VIBRATION)
					{
						g_pJoyVibration->wLeftMotorSpeed = MAX_VIBRATION;
					}

					if (g_pJoyVibration->wRightMotorSpeed >= MAX_VIBRATION)
					{
						g_pJoyVibration->wRightMotorSpeed = MAX_VIBRATION;
					}

					XInputSetState(0, g_pJoyVibration);
				}

				g_nCounterBulletTime = BULLET_TEX;

				g_nCounterBulletChargeTime++;
				if (g_nCounterBulletChargeTime % 12 == 0)
				{
					g_nCounterTexAnimationPlayer = (TEXTURE_ANIMATION)(g_nCounterTexAnimationPlayer + (1 * g_nCntTexYpos));
				}
			}
			else if (pGauge[g_player.nGaugeNo].nPercentGauge > GAUGE_MAX)
			{
				AddGauge(-pGauge[g_player.nGaugeNo].nPercentGauge, g_player.nGaugeNo);
				g_player.nBarrierNo = SetGauge(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f), BARRIER_COLOR, GAUGETYPE_BARRIER, GAUGE_MAX, 50.0f);
				g_player.state = PLAYERSTATE_BARRIER;
				g_player.nCounterCoolTime = BARRIAR_COOLDOWN;
				SetPlaceChar(D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y - 50.0f, 0.0f), POLY_SIZE(400.0f, 50.0f), CHARTEX_BARRIAR, 120);

				PlaySound(SOUND_LABEL_SE_BARRIAR);

				g_player.nCounterBarrier = 0;
				g_player.nCounterBarrierTime = BARRIER_STATE;
				g_nCounterBulletTime = 0;

				g_pJoyVibration->wLeftMotorSpeed = 0;

				g_pJoyVibration->wRightMotorSpeed = 0;

				XInputSetState(0, g_pJoyVibration);
			}

			g_nCounterAnimationPlayer = 0;
		}
		else
		{
			if (g_player.state != PLAYERSTATE_BARRIER)
			{
				if (g_player.nCounterBarrier > 0)
				{
					if (g_player.nCounterBarrier >= 4)
					{
						StopSound(SOUND_LABEL_SE_CHARGE);

						if (pGauge[g_player.nGaugeNo].nPercentGauge > 0)
						{// �Q�[�W������
							AddGauge(-1, g_player.nGaugeNo);
						}
					}

					g_player.nCounterBarrier--;

					if (g_pJoyVibration->wLeftMotorSpeed > 0)
					{
						g_pJoyVibration->wLeftMotorSpeed -= 500;

						g_pJoyVibration->wRightMotorSpeed -= 500;

						if (g_pJoyVibration->wLeftMotorSpeed <= 0)
						{
							g_pJoyVibration->wLeftMotorSpeed = 0;
						}

						if (g_pJoyVibration->wRightMotorSpeed <= 0)
						{
							g_pJoyVibration->wRightMotorSpeed = 0;
						}

						XInputSetState(0, g_pJoyVibration);
					}
				}
			}
			else
			{
				if (g_player.nCounterBarrierTime <= 0)
				{// �o���A��ԏI��
					g_player.nCounterBarrierTime = 0;

					AddGauge(-pGauge[g_player.nBarrierNo].nPercentGauge, g_player.nBarrierNo);

					g_player.state = PLAYERSTATE_NORMAL;

					AddHeart(0);
				}
				else
				{
					g_player.nCounterBarrierTime--;
					if (g_player.nCounterBarrierTime <= 100)
					{
						if ((g_player.nCounterBarrierTime % 2) == 0)
						{
							pGauge[g_player.nBarrierNo].col.a = 0.0f;
						}
						else
						{
							pGauge[g_player.nBarrierNo].col.a = 0.3f;
						}
					}
				}
			}

		}
	}

	if (((GetKeyboardPress(DIK_F) == true
		&& GetKeyboardPress(DIK_LSHIFT) == true
		&& GetKeyboardTrigger(DIK_SPACE) == true)
		|| (GetJoypadPress(JOYKEY_LEFT_SHOULDER) == true
			&& GetJoypadPress(JOYKEY_RIGHT_SHOULDER) == true
			&& GetJoypadPress(JOYKEY_X) == true
			&& GetJoypadPress(JOYKEY_LEFT_THUMB) == true))
		&& g_bUseFullburst == false)
	{
		g_player.type = SHOTTYPE_FULLBURST;
		g_player.fBulletSpeed = FULL_BURST_SPD;
		g_player.nBulletLife = FULL_BURST_LIFE;
		g_bUseFullburst = true;
	}

#ifdef _DEBUG

	if (GetKeyboardTrigger(DIK_N) == true)
	{
		DestroyEnemy();
	}

#endif
}

// ����\��Ԃ̕ύX
void SetPlayerDo(PLAYERDO playerDo, bool bCould)
{
	g_player.aCouldDo[playerDo] = bCould;
}

// �v���C���[�̌���(DIRECTION)���擾
PLAYERDIRECTION GetPlayerDirection(void)
{
	// PLAYERDIRECTION�̍X�V
	GetPlayerRot(&g_player);

	return g_player.rotPlayer;
}

// �`���[�W�Z�̃^�C�v���w��
void SetPlayerShotType(SHOTTYPE type)
{
	g_player.type = type;
	switch (type)
	{
	case SHOTTYPE_HOMING:

		g_player.fBulletSpeed = HOMING_SPD;
		g_player.nBulletLife = HOMING_LIFE;

		break;

	case SHOTTYPE_BOMB:

		g_player.fBulletSpeed = BOMB_SPD;
		g_player.nBulletLife = BOMB_LIFE;

		break;

	case SHOTTYPE_LASER:

		g_player.fBulletSpeed = LASER_SPD;
		g_player.nBulletLife = LASER_LIFE;

		break;

	case SHOTTYPE_FULLBURST:

		g_player.fBulletSpeed = 0.0f;
		g_player.nBulletLife = 1;

		break;
	}
}

void SetEnablePlayerFullburst(bool bUse)
{
	g_bUseFullburst = bUse;
}