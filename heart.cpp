//================================================================================================================
//
// DirectX�̃n�[�g�ݒu��cpp�t�@�C�� [heart.cpp]
// Author : TENMA
//
//================================================================================================================
#include "heart.h"
#include "player.h"
#include "playerframe.h"

// �}�N����`
#define HEART_WIDTH		(30.0f)					// �n�[�g�̉���
#define HEART_HEIGHT	(30.0f)					// �n�[�g�̏c��

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureHeart = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHeart = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posHeart;							// ���C�t��\������ʒu
int g_nLife;									// ���C�t�̒l

//================================================================================================================
// �n�[�g�̏���������
//================================================================================================================
void InitHeart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\BLOCK_HEART.png",
							&g_pTextureHeart);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHeart,
		NULL);

	g_posHeart = D3DXVECTOR3(50, 50, 0);			// �ʒu��������
	g_nLife = 0;									// �l��������

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	for (int nCntHeart = 0; nCntHeart < MAX_LIFE; nCntHeart++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) - HEART_WIDTH;
		pVtx[0].pos.y = g_posHeart.y - HEART_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) + HEART_WIDTH;
		pVtx[1].pos.y = g_posHeart.y - HEART_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) - HEART_WIDTH;
		pVtx[2].pos.y = g_posHeart.y + HEART_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posHeart.x + ((HEART_WIDTH * 1.5f) * nCntHeart) + HEART_WIDTH;
		pVtx[3].pos.y = g_posHeart.y + HEART_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHeart->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// �n�[�g�̏I������
//================================================================================================================
void UninitHeart(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)

	if (g_pTextureHeart != NULL)
	{
		g_pTextureHeart->Release();
		g_pTextureHeart = NULL;
	}
	

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffHeart != NULL)
	{
		g_pVtxBuffHeart->Release();
		g_pVtxBuffHeart = NULL;
	}
}

//================================================================================================================
// �n�[�g�̍X�V����
//================================================================================================================
void UpdateHeart(void)
{
	
}

//================================================================================================================
// �n�[�g�̕`�揈��
//================================================================================================================
void DrawHeart(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffHeart, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
	pDevice->SetTexture(0, g_pTextureHeart);

	for (int nCntBlock = 0; nCntBlock < MAX_LIFE; nCntBlock++)
	{

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			nCntBlock * 4,								// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

//================================================================================================================
// �n�[�g�̐ݒu
//================================================================================================================
void SetHeart(void)
{
	VERTEX_2D* pVtx;

	AddFunctionLog("START : Block Set");

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntHeart = 0; nCntHeart < MAX_LIFE; nCntHeart++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHeart->Unlock();

	g_nLife = MAX_LIFE;
}

//================================================================================================================
// �n�[�g�̒ǉ��A����
//================================================================================================================
void AddHeart(int nValue)
{
	VERTEX_2D* pVtx;

	if (nValue >= 1 && (g_nLife - 1) < MAX_LIFE)
	{ // �񕜂����ꍇ
		g_nLife++;

		ChangeModeFrame(FRAMESTATE_HEAL);

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nLife - 1);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffHeart->Unlock();
	}
	else if (nValue <= -1 && (g_nLife - 1) > -1)
	{ // �_���[�W��H������ꍇ
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffHeart->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nLife - 1);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffHeart->Unlock();

		g_nLife--;

		if (g_nLife > 1)
		{
			ChangeModeFrame(FRAMESTATE_DAMAGE);
		}
		else if (g_nLife == 1)
		{
			ChangeModeFrame(FRAMESTATE_DANGER);
		}
	}
	else
	{ // 0�ɂ�茻�ݑ̗͂ɂĉ����𔻒肵�����ꍇ
		if (g_nLife > 1)
		{
			ChangeModeFrame(FRAMESTATE_NORMAL);
		}
		else
		{
			ChangeModeFrame(FRAMESTATE_DANGER);
		}
	}
}