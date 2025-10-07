//================================================================================================================
//
// DirectX�̃N���W�b�g�ʐ^�\������ [creditPhoto.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "creditPhoto.h"
#include "input.h"

// �}�N����`
#define MAX_PHOTO			(3)			// �\������ʐ^�̐�

// ���S�\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	POLY_SIZE size;			// �T�C�Y
	int nTex;				// �e�N�X�`���̔ԍ�
}CREDITPHOTO;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureCreditPhoto = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCreditPhoto = NULL;	// ���_�o�b�t�@�̃|�C���^
CREDITPHOTO g_photo[MAX_PHOTO];							// ���S�\����
int g_nCounterPhoto;									// �ėp�J�E���^�[

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitCreditPhoto(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		g_photo[nCntPhoto].size = POLY_SIZE(400.0f, 230.0f);
		g_photo[nCntPhoto].nTex = 0;
	}

	g_photo[0].pos = D3DXVECTOR3(210.5f, 383.0f, 0.0f);
	g_photo[1].pos = D3DXVECTOR3(1062.5f, 213.0f, 0.0f);
	g_photo[2].pos = D3DXVECTOR3(1054.5f, 590.0f, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CREDIT\\photo.png",
							  &g_pTextureCreditPhoto);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PHOTO,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCreditPhoto,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCreditPhoto->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_photo[nCntPhoto].pos.x - g_photo[nCntPhoto].size.hx;
		pVtx[0].pos.y = g_photo[nCntPhoto].pos.y - g_photo[nCntPhoto].size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_photo[nCntPhoto].pos.x + g_photo[nCntPhoto].size.hx;
		pVtx[1].pos.y = g_photo[nCntPhoto].pos.y - g_photo[nCntPhoto].size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_photo[nCntPhoto].pos.x - g_photo[nCntPhoto].size.hx;
		pVtx[2].pos.y = g_photo[nCntPhoto].pos.y + g_photo[nCntPhoto].size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_photo[nCntPhoto].pos.x + g_photo[nCntPhoto].size.hx;
		pVtx[3].pos.y = g_photo[nCntPhoto].pos.y + g_photo[nCntPhoto].size.hy;
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
		pVtx[0].tex = D3DXVECTOR2(0.5f * nCntPhoto, nCntPhoto * 0.25f);
		pVtx[1].tex = D3DXVECTOR2(0.5f * nCntPhoto + 0.5f, nCntPhoto * 0.25f);
		pVtx[2].tex = D3DXVECTOR2(0.5f * nCntPhoto, (nCntPhoto * 0.25f) + 0.25f);
		pVtx[3].tex = D3DXVECTOR2(0.5f * nCntPhoto + 0.5f, (nCntPhoto * 0.25f) + 0.25f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCreditPhoto->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitCreditPhoto(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureCreditPhoto != NULL)
	{
		g_pTextureCreditPhoto->Release();
		g_pTextureCreditPhoto = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffCreditPhoto != NULL)
	{
		g_pVtxBuffCreditPhoto->Release();
		g_pVtxBuffCreditPhoto = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateCreditPhoto(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCreditPhoto->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		if (g_nCounterPhoto % (50 * (nCntPhoto + 1)) == 0)
		{
			g_photo[nCntPhoto].nTex = ((g_photo[nCntPhoto].nTex + 1) % 2);

			pVtx[0].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex, nCntPhoto * 0.25f);
			pVtx[1].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex + 0.5f, nCntPhoto * 0.25f);
			pVtx[2].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex, (nCntPhoto * 0.25f) + 0.25f);
			pVtx[3].tex = D3DXVECTOR2(0.5f * g_photo[nCntPhoto].nTex + 0.5f, (nCntPhoto * 0.25f) + 0.25f);
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCreditPhoto->Unlock();

	g_nCounterPhoto++;
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawCreditPhoto(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCreditPhoto, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCreditPhoto);

	for (int nCntPhoto = 0; nCntPhoto < MAX_PHOTO; nCntPhoto++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			4 * nCntPhoto,								// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}