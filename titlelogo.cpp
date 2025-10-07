//================================================================================================================
//
// DirectX�̃^�C�g����ʂ̃��S�\��������cpp [titlelogo.cpp]
// Author : TENMA
//
//================================================================================================================
#include "titlelogo.h"
#include "title.h"
#include "input.h"

// �}�N����`
#define LOGO_WIDTH1			(350.0f)		// ���S�̉��� * 0.5f
#define LOGO_HEIGHT1		(75.0f)			// ���S�̏c�� * 0.5f
#define LOGO_WIDTH2			(500.0f)		// ���S�̉��� * 0.5f
#define LOGO_HEIGHT2		(85.0f)		// ���S�̏c�� * 0.5f

// �\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 size;		// �c��
	D3DXCOLOR col;			// �F
	bool bUse;				// �L����
}TITLELOGO, *PTITLELOGO;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTextureTitleLogo[2] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleLogo = NULL;		// ���_�o�b�t�@�̃|�C���^
TITLELOGO g_aTitleLogo[2];
bool g_bCompleteLogo;									// ���S�̏o���������

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	PTITLELOGO pTitleLogo = &g_aTitleLogo[0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_LOGO1.png",
		&g_apTextureTitleLogo[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_LOGO2.png",
		&g_apTextureTitleLogo[1]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleLogo,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	// �~��Ă���

	pTitleLogo->pos = D3DXVECTOR3(350.0f, -150.0f, 0.0f);
	pTitleLogo->size.y = LOGO_HEIGHT1;
	pTitleLogo->col = D3DXCOLOR_NULL;
	pTitleLogo->bUse = false;
	g_bCompleteLogo = false;

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
	pVtx[0].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
	pVtx[1].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
	pVtx[2].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
	pVtx[3].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


	pVtx += 4;
	pTitleLogo++;
	
	// �o��

	pTitleLogo->pos = D3DXVECTOR3(900.0f, 200.0f, 0.0f);
	pTitleLogo->size = D3DXVECTOR3_NULL;
	pTitleLogo->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pTitleLogo->bUse = false;

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = pTitleLogo->pos.x - LOGO_WIDTH2;
	pVtx[0].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT2;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = pTitleLogo->pos.x + LOGO_WIDTH2;
	pVtx[1].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT2;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = pTitleLogo->pos.x - LOGO_WIDTH2;
	pVtx[2].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT2;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = pTitleLogo->pos.x + LOGO_WIDTH2;
	pVtx[3].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT2;
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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleLogo->Unlock();
}

//================================================================================================================
// ���U���g�̏I������
//================================================================================================================
void UninitTitleLogo(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntTitleLogo = 0; nCntTitleLogo < 2; nCntTitleLogo++)
	{
		if (g_apTextureTitleLogo[nCntTitleLogo] != NULL)
		{
			g_apTextureTitleLogo[nCntTitleLogo]->Release();
			g_apTextureTitleLogo[nCntTitleLogo] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffTitleLogo != NULL)
	{
		g_pVtxBuffTitleLogo->Release();
		g_pVtxBuffTitleLogo = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateTitleLogo(void)
{
	PTITLELOGO pTitleLogo = &g_aTitleLogo[0];
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleLogo->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_RETURN) == true
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true)
	{
		pTitleLogo[0].pos.y = 150.0f;
		pTitleLogo[0].bUse = true;
		pTitleLogo[1].col.a = 1.0f;
		pTitleLogo[1].bUse = true;
		pTitleLogo[1].size.x = LOGO_WIDTH2;
		pTitleLogo[1].size.y = LOGO_HEIGHT2;
		g_bCompleteLogo = true;
	}

	for (int nCntTitleLogo = 0; nCntTitleLogo < 2; nCntTitleLogo++, pTitleLogo++)
	{
		if (pTitleLogo->bUse == true)
		{
			switch (nCntTitleLogo)
			{
			case 0:

				if (pTitleLogo->pos.y < 100.0f)
				{
					pTitleLogo->pos.y += 5.0f;
				}
				else
				{
					pTitleLogo->pos.y = 100.0f;
					pTitleLogo[1].bUse = true;
				}

				// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
				pVtx[0].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
				pVtx[0].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
				pVtx[1].pos.y = pTitleLogo->pos.y - LOGO_HEIGHT1;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = pTitleLogo->pos.x - LOGO_WIDTH1;
				pVtx[2].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = pTitleLogo->pos.x + LOGO_WIDTH1;
				pVtx[3].pos.y = pTitleLogo->pos.y + LOGO_HEIGHT1;
				pVtx[3].pos.z = 0.0f;

				break;

			case 1:

				if (pTitleLogo->col.a < 1.0f)
				{
					pTitleLogo->col.a += 0.007f;
				}
				else
				{
					pTitleLogo->col.a = 1.0f;
					SetEnableTitleLogo(true);
					g_bCompleteLogo = true;
				}

				if (pTitleLogo->size.x < LOGO_WIDTH2)
				{
					pTitleLogo->size.x += 10.0f;
				}
				else
				{
					pTitleLogo->size.x = LOGO_WIDTH2;
				}

				if (pTitleLogo->size.y < LOGO_HEIGHT2)
				{
					pTitleLogo->size.y += 5.0f;
				}
				else
				{
					pTitleLogo->size.y = LOGO_HEIGHT2;
				}

				// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
				pVtx[0].pos.x = pTitleLogo->pos.x - pTitleLogo->size.x;
				pVtx[0].pos.y = pTitleLogo->pos.y - pTitleLogo->size.y;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = pTitleLogo->pos.x + pTitleLogo->size.x;
				pVtx[1].pos.y = pTitleLogo->pos.y - pTitleLogo->size.y;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = pTitleLogo->pos.x - pTitleLogo->size.x;
				pVtx[2].pos.y = pTitleLogo->pos.y + pTitleLogo->size.y;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = pTitleLogo->pos.x + pTitleLogo->size.x;
				pVtx[3].pos.y = pTitleLogo->pos.y + pTitleLogo->size.y;
				pVtx[3].pos.z = 0.0f;

				break;
			}

			// ���_�J���[�ݒ�
			pVtx[0].col = pTitleLogo->col;
			pVtx[1].col = pTitleLogo->col;
			pVtx[2].col = pTitleLogo->col;
			pVtx[3].col = pTitleLogo->col;
		}

		pVtx += 4;
	}
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawTitleLogo(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTileLogo = 0; nCntTileLogo < 2; nCntTileLogo++)
	{
		if (g_aTitleLogo[nCntTileLogo].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureTitleLogo[nCntTileLogo]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntTileLogo,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u�̐�
		}
	}
}

// ���S�̏o������
void SetTitleLogo(void)
{
	g_aTitleLogo[0].bUse = true;
}

// ���S�̏o�����
bool GetCompleteLogo(void)
{
	return g_bCompleteLogo;
}