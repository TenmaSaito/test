//================================================================================================================
//
// DirectX�̃Q�[�����̘g�\��������cpp [playerframe.cpp]
// Author : TENMA
//
//================================================================================================================
#include "playerframe.h"
#include "heart.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTexturePlayerFrame = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayerFrame = NULL;	// ���_�o�b�t�@�̃|�C���^
FRAMESTATE g_frameState;
D3DXCOLOR g_frameCol;
int g_nCounterFrame;
int g_nCounterStateFrame;

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitPlayerFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\PLAYERFRAME.png",
		&g_pTexturePlayerFrame);

	g_frameState = FRAMESTATE_NORMAL;
	g_frameCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	g_nCounterFrame = 0;
	g_nCounterStateFrame = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayerFrame,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayerFrame->Lock(0, 0, (void**)&pVtx, 0);

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

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayerFrame->Unlock();
}

//================================================================================================================
// ���U���g�̏I������
//================================================================================================================
void UninitPlayerFrame(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTexturePlayerFrame != NULL)
	{
		g_pTexturePlayerFrame->Release();
		g_pTexturePlayerFrame = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffPlayerFrame != NULL)
	{
		g_pVtxBuffPlayerFrame->Release();
		g_pVtxBuffPlayerFrame = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdatePlayerFrame(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayerFrame->Lock(0, 0, (void**)&pVtx, 0);

	switch (g_frameState)
	{
	case FRAMESTATE_NORMAL:

		g_frameCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

		break;

	case FRAMESTATE_DAMAGE:

		g_frameCol = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_nCounterStateFrame--;

		if (g_nCounterStateFrame <= 0)
		{
			g_nCounterStateFrame = 0;
			g_frameState = FRAMESTATE_NORMAL;
		}

		break;

	case FRAMESTATE_DANGER:

		if (g_nCounterFrame % 60 >= 0 && g_nCounterFrame % 60 < 30)
		{
			g_frameCol.r += (1.0f / 30.0f);
			if (g_frameCol.r > 1.0f)
			{
				g_frameCol.r = 1.0f;
			}
		}
		else if (g_nCounterFrame % 60 >= 30 && g_nCounterFrame % 60 < 60)
		{
			g_frameCol.r -= (1.0f / 30.0f);
			if (g_frameCol.r < 0.0f)
			{
				g_frameCol.r = 0.0f;
			}
		}

		break;

	case FRAMESTATE_DEATH:

		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		break;

	case FRAMESTATE_APPEAR:

		g_frameCol.r -= 0.005f;
		g_frameCol.b -= 0.005f;
		if (g_frameCol.r <= 0.0f)
		{
			g_frameCol = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
			g_frameState = FRAMESTATE_NORMAL;
		}

		break;

	case FRAMESTATE_BARRIAR:

		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 0.25f, 1.0f);

		break;

	case FRAMESTATE_HEAL:

		g_frameCol = D3DXCOLOR(1.0f, 0.0f, 0.5f, 1.0f);

		g_nCounterStateFrame--;

		if (g_nCounterStateFrame <= 0)
		{
			g_nCounterStateFrame = 0;
			g_frameState = FRAMESTATE_NORMAL;
		}

		break;

	case FRAMESTATE_POWER_ITEM:

		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

		g_nCounterStateFrame--;

		if (g_nCounterStateFrame <= 0)
		{
			g_nCounterStateFrame = 0;
			AddHeart(0);		// �̗͂̊m�F
		}

		break;
	}

	pVtx[0].col = g_frameCol;
	pVtx[1].col = g_frameCol;
	pVtx[2].col = g_frameCol;
	pVtx[3].col = g_frameCol;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayerFrame->Unlock();

	g_nCounterFrame++;
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawPlayerFrame(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlayerFrame, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturePlayerFrame);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u�̐�
}

void ChangeModeFrame(FRAMESTATE state)
{
	g_frameState = state;
	g_nCounterStateFrame = 60;

	if (state == FRAMESTATE_APPEAR)
	{
		g_frameCol = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}
	else if (state == FRAMESTATE_DANGER)
	{
		g_frameCol = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	}
}