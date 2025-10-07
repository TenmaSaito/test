//================================================================================================================
//
// DirectX�̃Q�[���I�[�o�[���G�t�F�N�g�̕\������ [GameoverEffect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "GameoverEffect.h"
#include "input.h"
#include "ResultScore.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "scoreRank.h"

// �}�N����`
#define CHARA_SIZE		(100.0f)		// �L�����N�^�[�̃T�C�Y

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureGameoverEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameoverEffect = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_overEffectPos = D3DXVECTOR3_NULL;				// �N���A���G�t�F�N�g�̈ʒu
int g_nCounterGameoverEffect;
int g_nAnimationCounter;	
bool g_bMoveOverEffect;										// �G�t�F�N�g�������Ă��邩

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitGameoverEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\player_animation_gameover.png",
							  &g_pTextureGameoverEffect);

	g_nCounterGameoverEffect = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffGameoverEffect,
								NULL);

	g_overEffectPos = D3DXVECTOR3(1380.0f,500.0f,0.0f);
	g_nAnimationCounter = 0;
	g_bMoveOverEffect = true;

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameoverEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_overEffectPos.x - CHARA_SIZE;
	pVtx[0].pos.y = g_overEffectPos.y - CHARA_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_overEffectPos.x + CHARA_SIZE;
	pVtx[1].pos.y = g_overEffectPos.y - CHARA_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_overEffectPos.x - CHARA_SIZE;
	pVtx[2].pos.y = g_overEffectPos.y + CHARA_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_overEffectPos.x + CHARA_SIZE;
	pVtx[3].pos.y = g_overEffectPos.y + CHARA_SIZE;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGameoverEffect->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitGameoverEffect(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureGameoverEffect != NULL)
	{
		g_pTextureGameoverEffect->Release();
		g_pTextureGameoverEffect = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffGameoverEffect != NULL)
	{
		g_pVtxBuffGameoverEffect->Release();
		g_pVtxBuffGameoverEffect = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateGameoverEffect(void)
{
	if (g_overEffectPos.x >= -CHARA_SIZE && GetFade() == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true
			|| GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_overEffectPos.x = -CHARA_SIZE;
			SetResultScore(D3DXVECTOR3(350.0f, 500.0f, 0.0f), GetScore());
			g_bMoveOverEffect = false;
		}

		g_overEffectPos.x -= 5.0f;
		if (g_overEffectPos.x >= 250.0f)
		{
			MoveResuktScore(D3DXVECTOR3(-5.0f, 0.0f, 0.0f));
		}
		else
		{
			SetResultScore(D3DXVECTOR3(350.0f, 500.0f, 0.0f), GetScore());
			g_bMoveOverEffect = false;
		}
	}

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameoverEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_overEffectPos.x - CHARA_SIZE;
	pVtx[0].pos.y = g_overEffectPos.y - CHARA_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_overEffectPos.x + CHARA_SIZE;
	pVtx[1].pos.y = g_overEffectPos.y - CHARA_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_overEffectPos.x - CHARA_SIZE;
	pVtx[2].pos.y = g_overEffectPos.y + CHARA_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_overEffectPos.x + CHARA_SIZE;
	pVtx[3].pos.y = g_overEffectPos.y + CHARA_SIZE;
	pVtx[3].pos.z = 0.0f;

	if (g_nCounterGameoverEffect % 30 == 0)
	{
		g_nAnimationCounter = g_nAnimationCounter ^ 1;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * g_nAnimationCounter, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f * g_nAnimationCounter + 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f * g_nAnimationCounter, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f * g_nAnimationCounter + 0.5f, 1.0f);
	}

	g_nCounterGameoverEffect++;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGameoverEffect->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawGameoverEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameoverEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGameoverEffect);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
					       0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);						// �`�悷��v���~�e�B�u�̐�
}

bool GetEnableGameoverEffect(void)
{
	return g_bMoveOverEffect;
}