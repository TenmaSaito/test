//================================================================================================================
//
// DirectX�̎c�@�ݒu��cpp�t�@�C�� [stock.cpp]
// Author : TENMA
//
//================================================================================================================
#include "stock.h"
#include "player.h"

// �}�N����`
#define STOCK_WIDTH		(30.0f)					// �c�@�̉���
#define STOCK_HEIGHT	(30.0f)					// �c�@�̏c��

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureStock = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posStock;							// �c�@��\������ʒu
int g_nStock;									// �c�@�̒l

//================================================================================================================
// �c�@�̏���������
//================================================================================================================
void InitStock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\PLAYER.png",
							&g_pTextureStock);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_STOCK,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStock,
		NULL);

	g_posStock = D3DXVECTOR3(50, 100, 0);			// �ʒu��������
	g_nStock = 0;									// �l��������

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) - STOCK_WIDTH;
		pVtx[0].pos.y = g_posStock.y - STOCK_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) + STOCK_WIDTH;
		pVtx[1].pos.y = g_posStock.y - STOCK_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) - STOCK_WIDTH;
		pVtx[2].pos.y = g_posStock.y + STOCK_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStock.x + ((STOCK_WIDTH * 1.5f) * nCntStock) + STOCK_WIDTH;
		pVtx[3].pos.y = g_posStock.y + STOCK_HEIGHT;
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
		pVtx[0].tex = D3DXVECTOR2(0.75f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.75f, 0.25f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.25f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStock->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// �c�@�̏I������
//================================================================================================================
void UninitStock(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)

	if (g_pTextureStock != NULL)
	{
		g_pTextureStock->Release();
		g_pTextureStock = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffStock != NULL)
	{
		g_pVtxBuffStock->Release();
		g_pVtxBuffStock = NULL;
	}
}

//================================================================================================================
// �c�@�̍X�V����
//================================================================================================================
void UpdateStock(void)
{
	
}

//================================================================================================================
// �c�@�̕`�揈��
//================================================================================================================
void DrawStock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffStock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
	pDevice->SetTexture(0, g_pTextureStock);

	for (int nCntBlock = 0; nCntBlock < MAX_LIFE; nCntBlock++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			nCntBlock * 4,								// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

//================================================================================================================
// �c�@�̐ݒu
//================================================================================================================
void SetStock(void)
{
	VERTEX_2D* pVtx;

	AddFunctionLog("START : Block Set");

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStock->Unlock();

	g_nStock = MAX_STOCK;
}

//================================================================================================================
// �c�@�̒ǉ��A��������
//================================================================================================================
void AddStock(int nValue)
{
	VERTEX_2D* pVtx;

	if (nValue >= 1 && (g_nStock - 1) < MAX_LIFE)
	{
		g_nStock++;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nStock - 1);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffStock->Unlock();
	}
	else if (nValue <= -1 && (g_nStock - 1) > -1)
	{
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * (g_nStock - 1);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffStock->Unlock();

		g_nStock--;
	}
}

//================================================================================================================
// �c��c�@�̎擾
//================================================================================================================
int GetStock(void)
{
	return g_nStock;
}