//================================================================================================================
//
// DirectX�̖��@�w�ݒu��cpp�t�@�C�� [mage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "mage.h"
#include "player.h"

// �}�N����`
#define MAGE_WIDTH		(80.0f)					// ����
#define MAGE_HEIGHT		(80.0f)					// �c��

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureMage = NULL;		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMage = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posMage;							// ���C�t��\������ʒu
PLAYERDIRECTION g_direction = PLAYERDIRECTION_DOWN;
bool g_bUseMage = false;

//================================================================================================================
// �n�[�g�̏���������
//================================================================================================================
void InitMage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\DIRECTION\\DIRECTION.png",
		&g_pTextureMage);
	

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMage,
		NULL);

	g_posMage = D3DXVECTOR3(PLAYER_SPOWN_X, PLAYER_SPOWN_Y, 0.0f);			// �ʒu��������
	g_bUseMage = false;
	
	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMage->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	
	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_posMage.x - MAGE_WIDTH;
	pVtx[0].pos.y = g_posMage.y - MAGE_HEIGHT;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posMage.x + MAGE_WIDTH;
	pVtx[1].pos.y = g_posMage.y - MAGE_HEIGHT;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posMage.x - MAGE_WIDTH;
	pVtx[2].pos.y = g_posMage.y + MAGE_HEIGHT;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posMage.x + MAGE_WIDTH;
	pVtx[3].pos.y = g_posMage.y + MAGE_HEIGHT;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.125f * g_direction, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f * g_direction + 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.125f * g_direction, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f * g_direction + 0.125f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMage->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// �n�[�g�̏I������
//================================================================================================================
void UninitMage(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureMage != NULL)
	{
		g_pTextureMage->Release();
		g_pTextureMage = NULL;
	}
	
	
	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffMage != NULL)
	{
		g_pVtxBuffMage->Release();
		g_pVtxBuffMage = NULL;
	}
}

//================================================================================================================
// �n�[�g�̍X�V����
//================================================================================================================
void UpdateMage(void)
{
	VERTEX_2D* pVtx = nullptr;

	g_pVtxBuffMage->Lock(0, 0, (void**)&pVtx, 0);

	if (g_bUseMage == true)
	{
		g_direction = GetPlayerDirection();

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.125f * g_direction, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f * g_direction + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f * g_direction, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f * g_direction + 0.125f, 1.0f);
	}

	g_pVtxBuffMage->Unlock();
}

//================================================================================================================
// �n�[�g�̕`�揈��
//================================================================================================================
void DrawMage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
	pDevice->SetTexture(0, g_pTextureMage);

	if (g_bUseMage == true)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

//================================================================================================================
// ���@�w�̐ݒu
//================================================================================================================
void SetMage(void)
{
	g_bUseMage = true;

	g_direction = PLAYERDIRECTION_DOWN;
}

//================================================================================================================
// ���@�w�̗L����
//================================================================================================================
void EnableMageDraw(bool bUse)
{
	g_bUseMage = bUse;
}