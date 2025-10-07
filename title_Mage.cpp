//================================================================================================================
//
// DirectX�̃^�C�g����ʂł̖��@�w�\�������̂�cpp [title_Mage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "title_Mage.h"
#include "title.h"
#include "titleSelect.h"
#include "titlelogo.h"
#include "fade.h"

// �}�N����`
#define MAGE_SELECT_SIZE		(SELECTSIZE_HEIGHT)				// �I��p���@�w�̔��a
#define MAGE_TEXT_SIZE			(150.0f)						
#define MAGE_COLORS				(0.65f)							// ���@�w�̏����J���[�̒l(����)

// �^�C�g�����@�w�\����
typedef struct
{
	D3DXVECTOR3 pos;			// �ʒu
	D3DXCOLOR col;				// �F
	TITLE_MAGETYPE type;		// ���
	POLY_SIZE size;				// �傫��
	bool bUseLightUp;			// col���㏸�����邩�ǂ���
}TITLE_MAGE, *PTITLE_MAGE;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureTitleMage = NULL;			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleMage = NULL;			// ���_�o�b�t�@�̃|�C���^
TITLE_MAGE g_aTitleMage[TITLE_MAGETYPE_MAX];				// ���@�w�̍\���̂̐錾
int g_nColorAlpha;											// ���l�̕���( + / - )

//================================================================================================================
// ���@�w�̏���������
//================================================================================================================
void InitTitleMage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;			// �f�o�C�X�̃|�C���^
	PTITLE_MAGE pTitle_Mage = &g_aTitleMage[0];

	for (int nCntTitleMage = 0; nCntTitleMage < TITLE_MAGETYPE_MAX; nCntTitleMage++, pTitle_Mage++)
	{
		pTitle_Mage->pos = D3DXVECTOR3_NULL;
		pTitle_Mage->col = D3DXCOLOR(MAGE_COLORS, MAGE_COLORS, MAGE_COLORS, 0.0f);
		pTitle_Mage->type = (TITLE_MAGETYPE)nCntTitleMage;
		pTitle_Mage->bUseLightUp = false;
	}

	g_aTitleMage[TITLE_MAGETYPE_SELECT].size = POLY_SIZE(75.0f, 70.0f);
	g_aTitleMage[TITLE_MAGETYPE_TEXT].size = POLY_SIZE(150.0f, 75.0f);
	g_nColorAlpha = 1;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_MAGE.png",
		&g_pTextureTitleMage);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_MAGETYPE_MAX,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleMage,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	pTitle_Mage = &g_aTitleMage[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleMage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < TITLE_MAGETYPE_MAX; nCntVtx++, pTitle_Mage++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[0].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[1].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[2].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[3].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�ݒ�
		pVtx[0].col = pTitle_Mage->col;
		pVtx[1].col = pTitle_Mage->col;
		pVtx[2].col = pTitle_Mage->col;
		pVtx[3].col = pTitle_Mage->col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f * nCntVtx);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f * nCntVtx);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f * nCntVtx + 0.5f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.5f * nCntVtx + 0.5f);
		
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleMage->Unlock();
}

//================================================================================================================
// ���@�w�̏I������
//================================================================================================================
void UninitTitleMage(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureTitleMage != NULL)
	{
		g_pTextureTitleMage->Release();
		g_pTextureTitleMage = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffTitleMage != NULL)
	{
		g_pVtxBuffTitleMage->Release();
		g_pVtxBuffTitleMage = NULL;
	}
}

//================================================================================================================
// ���@�w�̍X�V����
//================================================================================================================
void UpdateTitleMage(void)
{
	PTITLE_MAGE pTitle_Mage = &g_aTitleMage[0];
	VERTEX_2D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleMage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < TITLE_MAGETYPE_MAX; nCntVtx++, pTitle_Mage++)
	{
		if(GetCompleteLogo() == true && pTitle_Mage->col.a == 0.0f)
		{
			pTitle_Mage->col.a = MAGE_COLORS;
		}

		if (pTitle_Mage->bUseLightUp == true 
			&& pTitle_Mage->col.a < 1.0f
			&& pTitle_Mage->type == TITLE_MAGETYPE_TEXT)
		{
			pTitle_Mage->col += D3DXCOLOR(0.005f, 0.005f, 0.005f, 0.005f);
			if (pTitle_Mage->col.a > 1.0f)
			{
				pTitle_Mage->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}

		if (pTitle_Mage->type == TITLE_MAGETYPE_SELECT)
		{
			if (GetCompleteLogo() == true)
			{
				pTitle_Mage->col += D3DXCOLOR(0.01f * g_nColorAlpha, 0.01f * g_nColorAlpha, 0.01f * g_nColorAlpha, 0.01f * g_nColorAlpha);
				if (pTitle_Mage->col.a > 1.0f)
				{
					pTitle_Mage->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					g_nColorAlpha *= -1;
				}
				else if (pTitle_Mage->col.a < 0.2f)
				{
					pTitle_Mage->col = D3DXCOLOR(0.2f, 0.2f, 0.2f, 0.2f);
					g_nColorAlpha *= -1;
				}
			}
			else
			{
				pTitle_Mage->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
			}
		}

		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[0].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[1].pos.y = pTitle_Mage->pos.y - pTitle_Mage->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitle_Mage->pos.x - pTitle_Mage->size.hx;
		pVtx[2].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitle_Mage->pos.x + pTitle_Mage->size.hx;
		pVtx[3].pos.y = pTitle_Mage->pos.y + pTitle_Mage->size.hy;
		pVtx[3].pos.z = 0.0f;

		// ���_�J���[�ݒ�
		pVtx[0].col = pTitle_Mage->col;
		pVtx[1].col = pTitle_Mage->col;
		pVtx[2].col = pTitle_Mage->col;
		pVtx[3].col = pTitle_Mage->col;

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleMage->Unlock();
}

//================================================================================================================
// ���@�w�̕`�揈��
//================================================================================================================
void DrawTitleMage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleMage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleMage);

	for (int nCntPoly = 0; nCntPoly < TITLE_MAGETYPE_MAX; nCntPoly++)
	{
		if (GetEnableTitleSelect() == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntPoly,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// ���@�w�̐ݒu����
//================================================================================================================
void SetTitleMage(D3DXVECTOR3 pos, TITLE_MAGETYPE type)
{
	g_aTitleMage[type].pos = pos;
}

//================================================================================================================
// ���@�w�̃��C�g�A�b�v����
//================================================================================================================
void LightUpTitleMage(TITLE_MAGETYPE type)
{
	g_aTitleMage[type].bUseLightUp = true;
}