//================================================================================================================
//
// DirectX�̔w�i���� [bg.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "bg.h"
#include "player.h"

//�}�N����`
#define BG_WIDTH			(1280.0f)			// �w�i�̉�
#define BG_HEIGHT			(720.0f)			// �w�i�̏c

// �w�i�\����
typedef struct
{
	STAGE stage;
	bool bUse;
}STAGE_BG;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureBG = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBG = NULL;					// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posBg;											// �w�i�̌��݈ʒu
STAGE_BG g_aStageBg[STAGE_MAX];									// �e�X�e�[�W
STAGE g_stageBgExac;
float g_fLengthBg;												// �w�i�̑Ίp���̒���
float g_fAngleBg;												// �w�i�̑Ίp���̊p�x

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitBG(void)
{
	AddFunctionLog("START : Background Init");

	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	HWND hWnd = NULL;

	AddFunctionLog("START : DirectXDevice GetPointer");

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	AddFunctionLog("START : Texture Create");

	for (int n = 0; n < STAGE_MAX; n++)
	{
		g_aStageBg[n].stage = (STAGE)n;
		g_aStageBg[n].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\BG\\BG.jpg",
		&g_pTextureBG);

	AddFunctionLog("END : Texture Create");

	// �Ίp���̒������擾
	g_fLengthBg = sqrtf(BG_WIDTH * BG_WIDTH + BG_HEIGHT * BG_HEIGHT) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_fAngleBg = atan2f(BG_WIDTH, BG_HEIGHT);

	g_posBg.x = BG_WIDTH * 0.5f;
	g_posBg.y = BG_HEIGHT * 0.5f;

	AddFunctionLog("START : BackgroundVertexBuffer Create");

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STAGE_MAX,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffBG,
								NULL);

	AddFunctionLog("END : BackgroundVertexBuffer Create");

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	AddFunctionLog("START : BackgroundVertex Setting");

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBG->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBg = 0; nCntBg < STAGE_MAX; nCntBg++)
	{
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		if (nCntBg < STAGE_GRASS || nCntBg >= STAGE_MAX)
		{
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{ // �X�e�[�W�̎�ނ�������͈͊O�������ꍇ�A�����𒆒f
#ifdef  _DEBUG
				MessageBox(hWnd, "���o�C", "���H", MB_ICONWARNING);
#endif
				return;
			}
		}
		else
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.125f * nCntBg, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * nCntBg + 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * nCntBg, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * nCntBg + 0.125f, 1.0f);
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBG->Unlock();

	AddFunctionLog("END : BackgroundVertex Setting");

	AddFunctionLog("END : Background Init");
}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitBG(void)
{
	AddFunctionLog("START : Background Uninit");

	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntBg = 0; nCntBg < STAGE_MAX; nCntBg++)
	{
		if (g_pTextureBG != NULL)
		{
			g_pTextureBG->Release();
			g_pTextureBG = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffBG != NULL)
	{
		g_pVtxBuffBG->Release();
		g_pVtxBuffBG = NULL;
	}

	AddFunctionLog("END : Background Uninit");
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateBG(void)
{
	//��ŏ���
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawBG(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBG, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		if (g_aStageBg[nCnt].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBG);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCnt,									// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

void SetBG(STAGE stage)
{
	AddFunctionLog("START : Background Set");

	g_aStageBg[g_stageBgExac].bUse = false;

	g_stageBgExac = stage;

	g_aStageBg[g_stageBgExac].bUse = true;

	AddFunctionLog("END : Background Set");
}