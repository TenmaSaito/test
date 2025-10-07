//================================================================================================================
//
// DirectX�̌x���\������ [caution.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "caution.h"
#include "input.h"

// �}�N����`
#define CAUTION_COUNT		(120)

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureCaution = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCaution = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_Cautionpos;							// �ʒu
POLY_SIZE g_Cautionsize = POLY_SIZE(1280.0f, 120.0f);
float g_fAlphaCaution;									
int g_nCounterCaution;								// �ėp�J�E���^�[
int g_nAlphaCaution;								// ���l
bool g_bUseCaution;

//================================================================================================================
// �x���̏���������
//================================================================================================================
void InitCaution(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();
	g_Cautionpos = D3DXVECTOR3(WINDOW_MID.x, 610.0f, 0.0f);
	g_fAlphaCaution = 0.1f;
	g_nCounterCaution = 0;
	g_nAlphaCaution = 1;
	g_bUseCaution = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CHARTEX\\BARRIER_CAUTION.png",
							  &g_pTextureCaution);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCaution,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCaution->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_Cautionpos.x - g_Cautionsize.hx;
	pVtx[0].pos.y = g_Cautionpos.y - g_Cautionsize.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Cautionpos.x + g_Cautionsize.hx;
	pVtx[1].pos.y = g_Cautionpos.y - g_Cautionsize.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Cautionpos.x - g_Cautionsize.hx;
	pVtx[2].pos.y = g_Cautionpos.y + g_Cautionsize.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Cautionpos.x + g_Cautionsize.hx;
	pVtx[3].pos.y = g_Cautionpos.y + g_Cautionsize.hy;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);
	pVtx[1].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);
	pVtx[2].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);
	pVtx[3].col = D3DXCOLOR(1.0f,1.0f,1.0f,g_fAlphaCaution);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCaution->Unlock();
}

//================================================================================================================
// �x���̏I������
//================================================================================================================
void UninitCaution(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureCaution != NULL)
	{
		g_pTextureCaution->Release();
		g_pTextureCaution = NULL;
	}
	

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffCaution != NULL)
	{
		g_pVtxBuffCaution->Release();
		g_pVtxBuffCaution = NULL;
	}
}

//================================================================================================================
// �x���̍X�V����
//================================================================================================================
void UpdateCaution(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCaution->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterCaution > 0 && g_nCounterCaution < (CAUTION_COUNT - 10))
	{
		g_fAlphaCaution -= 0.1f;
		if (g_fAlphaCaution <= 0.0f || g_nCounterCaution <= 0)
		{
			g_fAlphaCaution = 0.0f;
			g_nCounterCaution = 0;
			g_bUseCaution = false;
		}
	}

	// ���_�J���[�ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_fAlphaCaution);

	g_nCounterCaution--;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCaution->Unlock();
}

//================================================================================================================
// �x���̕`�揈��
//================================================================================================================
void DrawCaution(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCaution, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCaution);

	if (g_bUseCaution == true)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

//================================================================================================================
// �x���\������
//================================================================================================================
void SetCaution(void)
{
	g_nCounterCaution = CAUTION_COUNT;
	g_fAlphaCaution = 1.0f;
	g_bUseCaution = true;
}