//================================================================================================================
//
// DirectX�̃Q�[���N���A���G�t�F�N�g�̕\������ [GameclearEffect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "GameclearEffect.h"
#include "input.h"
#include "ResultScore.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"

// �}�N����`
#define CHARA_SIZE		(100.0f)		// �L�����N�^�[�̃T�C�Y

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureGameclearEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameclearEffect = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_clearEffectPos = D3DXVECTOR3_NULL;			// �N���A���G�t�F�N�g�̈ʒu
int g_nCounterGameclearEffect;
int g_nAnimationCounterClear;
bool g_bMoveClearEffect;

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitGameclearEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\player_animation_gameclear.png",
							  &g_pTextureGameclearEffect);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffGameclearEffect,
								NULL);

	g_nCounterGameclearEffect = 0;
	g_clearEffectPos = D3DXVECTOR3(1380.0f, 500.0f, 0.0f);
	g_nAnimationCounterClear = 0;
	g_bMoveClearEffect = true;

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameclearEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[0].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[1].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[2].pos.y = g_clearEffectPos.y + CHARA_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[3].pos.y = g_clearEffectPos.y + CHARA_SIZE;
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
	g_pVtxBuffGameclearEffect->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitGameclearEffect(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureGameclearEffect != NULL)
	{
		g_pTextureGameclearEffect->Release();
		g_pTextureGameclearEffect = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffGameclearEffect != NULL)
	{
		g_pVtxBuffGameclearEffect->Release();
		g_pVtxBuffGameclearEffect = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateGameclearEffect(void)
{
	if (g_clearEffectPos.x >= -CHARA_SIZE && GetFade() == FADE_NONE)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true
			|| GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_clearEffectPos.x = -CHARA_SIZE;
			SetResultScore(D3DXVECTOR3(350.0f, 500.0f, 0.0f), GetScore());
			g_bMoveClearEffect = false;
		}

		g_clearEffectPos.x -= 5.0f;
		if (g_clearEffectPos.x >= 250.0f)
		{
			MoveResuktScore(D3DXVECTOR3(-5.0f, 0.0f, 0.0f));
		}
		else
		{
			SetResultScore(D3DXVECTOR3(350.0f, 500.0f, 0.0f), GetScore());
			g_bMoveClearEffect = false;
		}
	}

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGameclearEffect->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[0].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[1].pos.y = g_clearEffectPos.y - CHARA_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_clearEffectPos.x - CHARA_SIZE;
	pVtx[2].pos.y = g_clearEffectPos.y + CHARA_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_clearEffectPos.x + CHARA_SIZE;
	pVtx[3].pos.y = g_clearEffectPos.y + CHARA_SIZE;
	pVtx[3].pos.z = 0.0f;

	if (g_nCounterGameclearEffect % 30 == 0)
	{
		g_nAnimationCounterClear = g_nAnimationCounterClear ^ 1;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear + 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f * g_nAnimationCounterClear + 0.5f, 1.0f);
	}

	g_nCounterGameclearEffect++;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGameclearEffect->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawGameclearEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGameclearEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureGameclearEffect);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
					       0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);						// �`�悷��v���~�e�B�u�̐�
}

bool GetEnableClearEffect(void)
{
	return g_bMoveClearEffect;
}