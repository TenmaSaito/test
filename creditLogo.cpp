//================================================================================================================
//
// DirectX�̃N���W�b�g���S�\������ [creditLogo.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "creditLogo.h"
#include "input.h"

// ���S�\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	POLY_SIZE size;			// �T�C�Y
}CREDITLOGO;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTextureCreditLogo[2] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCreditLogo = NULL;	// ���_�o�b�t�@�̃|�C���^
CREDITLOGO g_logo[2];									// ���S�\����
int g_nCounterCreditLogo;								// �ėp�J�E���^�[

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitCreditLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	g_logo[0].pos = D3DXVECTOR3(175.0f, 100.0f, 0.0f);
	g_logo[0].size = POLY_SIZE(300.0f, 100.0f);
	g_logo[0].col = D3DXCOLOR_NULL;

	g_logo[1].pos = D3DXVECTOR3(910.0f, 530.0f, 0.0f);
	g_logo[1].size = POLY_SIZE(300.0f, 150.0f);
	g_logo[1].col = D3DXCOLOR_NULL;

	g_nCounterCreditLogo = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CREDIT\\Credit.png",
							  &g_apTextureCreditLogo[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\CREDIT\\YOU!.png",
		&g_apTextureCreditLogo[1]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCreditLogo,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCreditLogo->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		pVtx[0].pos.x = g_logo[nCntLogo].pos.x - g_logo[nCntLogo].size.hx;
		pVtx[0].pos.y = g_logo[nCntLogo].pos.y - g_logo[nCntLogo].size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_logo[nCntLogo].pos.x + g_logo[nCntLogo].size.hx;
		pVtx[1].pos.y = g_logo[nCntLogo].pos.y - g_logo[nCntLogo].size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_logo[nCntLogo].pos.x - g_logo[nCntLogo].size.hx;
		pVtx[2].pos.y = g_logo[nCntLogo].pos.y + g_logo[nCntLogo].size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_logo[nCntLogo].pos.x + g_logo[nCntLogo].size.hx;
		pVtx[3].pos.y = g_logo[nCntLogo].pos.y + g_logo[nCntLogo].size.hy;
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�ݒ�
		pVtx[0].col = g_logo[0].col;
		pVtx[1].col = g_logo[0].col;
		pVtx[2].col = g_logo[0].col;
		pVtx[3].col = g_logo[0].col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCreditLogo->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitCreditLogo(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		if (g_apTextureCreditLogo[nCntLogo] != NULL)
		{
			g_apTextureCreditLogo[nCntLogo]->Release();
			g_apTextureCreditLogo[nCntLogo] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffCreditLogo != NULL)
	{
		g_pVtxBuffCreditLogo->Release();
		g_pVtxBuffCreditLogo = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateCreditLogo(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCreditLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterCreditLogo % 120 >= 0 && g_nCounterCreditLogo % 120 < 60)
	{
		g_logo[1].col.a += (1.0f / 60.0f);
		if (g_logo[1].col.a > 1.0f)
		{
			g_logo[1].col.a = 1.0f;
		}
	}
	else if (g_nCounterCreditLogo % 120 >= 60 && g_nCounterCreditLogo % 120 < 120)
	{
		g_logo[1].col.a -= (1.0f / 60.0f);
		if (g_logo[1].col.a < 0.0f)
		{
			g_logo[1].col.a = 0.0f;
		}
	}

	pVtx += 4;

	// ���_�J���[�ݒ�
	pVtx[0].col = g_logo[1].col;
	pVtx[1].col = g_logo[1].col;
	pVtx[2].col = g_logo[1].col;
	pVtx[3].col = g_logo[1].col;

	g_nCounterCreditLogo++;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCreditLogo->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawCreditLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCreditLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	for (int nCntLogo = 0; nCntLogo < 2; nCntLogo++)
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureCreditLogo[nCntLogo]);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			4 * nCntLogo,								// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}