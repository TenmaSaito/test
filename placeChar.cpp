//================================================================================================================
//
// DirectX�̎��R���Ńe�N�X�`���̔z�u���� [PlaceChar.cpp]
// Author : TENMA
//
//================================================================================================================
#include "placeChar.h"

// �}�N����`
#define MAX_FADETEX				(32)			// �e�N�X�`���̓����o���\�ő吔
#define TEX_SPACE				(0.05f)			// �e�N�X�`���̌���

// ���R���Ńe�N�X�`���\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;							// �e�N�X�`�����o��������N�_
	D3DXCOLOR col;								// �e�N�X�`����alpha�l
	POLY_SIZE size;								// �|���S���̕��ƍ���
	CHARTEX tex;								// �`�悷��e�N�X�`���̎��
	int nLife;									// �`�悷�鎞��
	bool bUse;									// �g���Ă��邩
}FADETEX;

LPDIRECT3DTEXTURE9		g_pTexturePlaceChar = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlaceChar = NULL;				// ���_�o�b�t�@�̃|�C���^
FADETEX g_aFadeTex[MAX_FADETEX];								// �\���̂̐錾
int g_nCounterPlaceChar;										// �ėp�J�E���^�[

//================================================================================================================
// ���R���Ńe�N�X�`���̏�����
//================================================================================================================
void InitPlaceChar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	int nCntPlaceChar;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\CHARTEX\\CHARTEX.png",
		&g_pTexturePlaceChar);
	
	FADETEX *pFadeTex = &g_aFadeTex[0];		// �\���̂̐擪�A�h���X����

	// ���R���Ńe�N�X�`���̏��̏�����
	for (int nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++, pFadeTex++)
	{
		pFadeTex->pos = D3DXVECTOR3_NULL;
		pFadeTex->col = D3DXCOLOR_NULL;
		pFadeTex->size.x = 0.0f;
		pFadeTex->size.y = 0.0f;
		pFadeTex->tex = CHARTEX_FULLCHARGE;
		pFadeTex->nLife = 0;
		pFadeTex->bUse = false;
	}

	g_nCounterPlaceChar = NULL;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_FADETEX,			// MAX_PlaceChar���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlaceChar,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlaceChar->Lock(0, 0, (void**)&pVtx, 0);

	pFadeTex = &g_aFadeTex[0];

	for (nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++, pFadeTex++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[0].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[0].pos.z = 0.0f;			  

		pVtx[1].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[1].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[1].pos.z = 0.0f;			  

		pVtx[2].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[2].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
		pVtx[2].pos.z = 0.0f;			  

		pVtx[3].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[3].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[1].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	g_pVtxBuffPlaceChar->Unlock();
}

//================================================================================================================
// ���R���Ńe�N�X�`���̏I������
//================================================================================================================
void UninitPlaceChar(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTexturePlaceChar != NULL)
	{
		g_pTexturePlaceChar->Release();
		g_pTexturePlaceChar = NULL;
	}
	

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffPlaceChar != NULL)
	{
		g_pVtxBuffPlaceChar->Release();
		g_pVtxBuffPlaceChar = NULL;
	}
}

//================================================================================================================
// ���R���Ńe�N�X�`���̍X�V����
//================================================================================================================
void UpdatePlaceChar(void)
{
	VERTEX_2D* pVtx;

	FADETEX* pFadeTex = &g_aFadeTex[0];

	g_pVtxBuffPlaceChar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++, pFadeTex++)
	{
		if (pFadeTex->bUse == true)
		{
			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
			pVtx[0].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
			pVtx[1].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
			pVtx[2].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
			pVtx[3].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
			pVtx[3].pos.z = 0.0f;

			if (pFadeTex->nLife <= 0)
			{
				pFadeTex->col.a -= 0.01f;
				if (pFadeTex->col.a <= 0.0f)
				{
					pFadeTex->col.a = 0.0f;
					pFadeTex->bUse = false;
				}
			}

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pFadeTex->col;
			pVtx[1].col = pFadeTex->col;
			pVtx[2].col = pFadeTex->col;
			pVtx[3].col = pFadeTex->col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
			pVtx[1].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
			pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);

			pFadeTex->nLife--;
			if (pFadeTex->nLife <= 0)
			{
				pFadeTex->nLife = 0;
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffPlaceChar->Unlock();

	g_nCounterPlaceChar++;
}

//================================================================================================================
// ���R���Ńe�N�X�`���̕`�揈��
//================================================================================================================
void DrawPlaceChar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	int nCntPlaceChar;
	FADETEX* pFadeTex = &g_aFadeTex[0];

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPlaceChar, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++,pFadeTex++)
	{
		if (pFadeTex->bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTexturePlaceChar);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntPlaceChar,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// ���R���Ńe�N�X�`���̐ݒ菈��
//================================================================================================================
void SetPlaceChar(D3DXVECTOR3 pos, POLY_SIZE size, CHARTEX tex, int nCounter)
{
	VERTEX_2D* pVtx;
	FADETEX* pFadeTex = &g_aFadeTex[0];

	for (int nCntPlaceChar = 0; nCntPlaceChar < MAX_FADETEX; nCntPlaceChar++,pFadeTex++)
	{
		if (pFadeTex->bUse == true) continue;

		g_pVtxBuffPlaceChar->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += 4 * nCntPlaceChar;
		
		pFadeTex->pos = pos;
		pFadeTex->size = size;
		pFadeTex->col = D3DXCOLOR_NULL;
		pFadeTex->tex = tex;
		pFadeTex->nLife = nCounter;

		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[0].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[1].pos.y = pFadeTex->pos.y - (pFadeTex->size.y * 0.5f);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pFadeTex->pos.x - (pFadeTex->size.x * 0.5f);
		pVtx[2].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pFadeTex->pos.x + (pFadeTex->size.x * 0.5f);
		pVtx[3].pos.y = pFadeTex->pos.y + (pFadeTex->size.y * 0.5f);
		pVtx[3].pos.z = 0.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pFadeTex->col;
		pVtx[1].col = pFadeTex->col;
		pVtx[2].col = pFadeTex->col;
		pVtx[3].col = pFadeTex->col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[1].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex);
		pVtx[2].tex = D3DXVECTOR2(0.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, TEX_SPACE * g_aFadeTex[nCntPlaceChar].tex + 0.05f);

		pFadeTex->bUse = true;

		break;
	}

	g_pVtxBuffPlaceChar->Unlock();
}

//================================================================================================================
// ���R���Ńe�N�X�`���̋����폜����
//================================================================================================================
void DeleteChar(CHARTEX tex)
{
	FADETEX* pFadeTex = &g_aFadeTex[0];

	for (int nCntChar = 0; nCntChar < MAX_FADETEX; nCntChar++,pFadeTex++)
	{
		if (pFadeTex->tex == tex)
		{
			pFadeTex->bUse = false;
		}
	}
}