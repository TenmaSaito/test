//================================================================================================================
//
// DirectX�̃e���|�[�g�G�t�F�N�g�\������ [teleport.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "game.h"
#include "player.h"
#include "teleport.h"
#include "timer.h"
#include "fade.h"
#include "fade_stage.h"

#define TEXTURE_MAX		(4)								// �e�N�X�`���̕�����
#define TEXTURE_POS		(1.0f / (float)TEXTURE_MAX)			// �e�N�X�`���̈ꖇ������̍��W

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureTeleport = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTeleport = NULL;	// ���_�o�b�t�@�̃|�C���^
POLY_SIZE g_Teleport;							// �G�t�F�N�g�̃T�C�Y
int g_nCounterTeleport;								// �ėp�J�E���^�[
int g_nTexCounterTeleport;							// �e�N�X�`���̂��炵

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitTeleport(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Teleport = POLY_SIZE(PLAYER_SIZE, PLAYER_SIZE);

	g_nCounterTeleport = 0;

	g_nTexCounterTeleport = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\EFFECT\\TELEPORT.png",
							  &g_pTextureTeleport);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTeleport,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTeleport->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = WINDOW_MID.x - g_Teleport.hx;
	pVtx[0].pos.y = WINDOW_MID.y - g_Teleport.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = WINDOW_MID.x + g_Teleport.hx;
	pVtx[1].pos.y = WINDOW_MID.y - g_Teleport.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = WINDOW_MID.x - g_Teleport.hx;
	pVtx[2].pos.y = WINDOW_MID.y + g_Teleport.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = WINDOW_MID.x + g_Teleport.hx;
	pVtx[3].pos.y = WINDOW_MID.y + g_Teleport.hy;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(TEXTURE_POS, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(TEXTURE_POS, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTeleport->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitTeleport(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureTeleport != NULL)
	{
		g_pTextureTeleport->Release();
		g_pTextureTeleport = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffTeleport != NULL)
	{
		g_pVtxBuffTeleport->Release();
		g_pVtxBuffTeleport = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateTeleport(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTeleport->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterTeleport % 12 == 0)
	{
		g_nTexCounterTeleport++;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX) + TEXTURE_POS, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEXTURE_POS * (g_nTexCounterTeleport % TEXTURE_MAX) + TEXTURE_POS, 1.0f);
	}

	g_nCounterTeleport++;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTeleport->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawTeleport(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTeleport, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTeleport);

	if (GetFade() == FADE_IN
		|| (GetFadeStage() == FADESTAGE_IN || GetFadeStage() == FADESTAGE_OUT))
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}