//================================================================================================================
//
// DirectX�̃X�e�[�W�p�t�F�[�h���� [fade_stage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "fade_stage.h"
#include "input.h"
#include "sound.h"
#include "bg.h"
#include "stage.h"
#include "player.h"

// �O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFadeStage = NULL;	// ���_�o�b�t�@�̃|�C���^
FADESTAGE g_fadeStage;								// �t�F�[�h�̎��
STAGE g_stageNext;									// ���(���[�h)�̎��
D3DXCOLOR g_colorFadeStage;							// �|���S���̐F

//================================================================================================================
// �X�e�[�W�p�t�F�[�h�̏���������
//================================================================================================================
void InitFadeStage(void) 
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	g_fadeStage = FADESTAGE_NONE;			// �ʏ��Ԃ�

	g_colorFadeStage.a = 0.0f;				// alpha�l��������

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFadeStage,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFadeStage->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFadeStage->Unlock();

	PLAYER* pPlayer = GetPlayer();

	for (int nCntPlayer = 0; nCntPlayer < PLAYERDO_MAX; nCntPlayer++)
	{
		pPlayer->aCouldDo[nCntPlayer] = true;
	}
}

//================================================================================================================
// �X�e�[�W�p�t�F�[�h�̏I������
//================================================================================================================
void UninitFadeStage(void)
{
	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffFadeStage != NULL)
	{
		g_pVtxBuffFadeStage->Release();
		g_pVtxBuffFadeStage = NULL;
	}
}

//================================================================================================================
// �X�e�[�W�p�t�F�[�h�̍X�V����
//================================================================================================================
void UpdateFadeStage(void)
{
	if (g_fadeStage != FADESTAGE_NONE)
	{
		if (g_fadeStage == FADESTAGE_IN)
		{// �t�F�[�h�C��
			g_colorFadeStage.a -= 0.01f;
			if (g_colorFadeStage.a <= 0.0f)
			{// �t�F�[�h�C���I��
				g_colorFadeStage.a = 0.0f;
				g_fadeStage = FADESTAGE_NONE;
			}
		}
		else if (g_fadeStage == FADESTAGE_OUT)
		{// �t�F�[�h�A�E�g
			g_colorFadeStage.a += 0.01f;
			if (g_colorFadeStage.a >= 1.0f)
			{// �t�F�[�h�A�E�g�I��
				g_colorFadeStage.a = 1.0f;
				g_fadeStage = FADESTAGE_IN;

				// ���[�h�ݒ�
				SetStage(g_stageNext);
				SetBG(g_stageNext);
			}
		}

		VERTEX_2D* pVtx = NULL;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFadeStage->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFadeStage.a);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffFadeStage->Unlock();
	}
}

//================================================================================================================
// �X�e�[�W�p�t�F�[�h�̕`�揈��
//================================================================================================================
void DrawFadeStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFadeStage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
	pDevice->SetTexture(0, NULL);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u�̐�
}

// �X�e�[�W�p�t�F�[�h�̐ݒ�
void SetFadeStage(STAGE stageNext, FADESTAGE start)
{
	g_fadeStage = start;

	g_stageNext = stageNext;

	g_colorFadeStage.a = 0.0f;
}

// �X�e�[�W�p�t�F�[�h�̎擾
FADESTAGE GetFadeStage(void)
{
	return g_fadeStage;
}