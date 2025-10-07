//================================================================================================================
//
// DirectX�̐������Ԋԋ߁I�\������ [nomoretime.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "game.h"
#include "nomoretime.h"
#include "timer.h"

// ���S�\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXCOLOR col;			// �F
	POLY_SIZE size;			// �T�C�Y
}NOMORETIME;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureNomoretime = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNomoretime = NULL;	// ���_�o�b�t�@�̃|�C���^
NOMORETIME g_Nomoretime;										// ���S�\����
int g_nCounterNomoretime;								// �ėp�J�E���^�[

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitNomoretime(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Nomoretime.pos = D3DXVECTOR3(1480.0f, WINDOW_MID.y, 0.0f);
	g_Nomoretime.size = POLY_SIZE(325.0f, 100.0f);
	g_Nomoretime.col = D3DXCOLOR_NULL;

	g_nCounterNomoretime = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\NOMORETIME.png",
							  &g_pTextureNomoretime);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffNomoretime,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNomoretime->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[0].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[1].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[2].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[3].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�ݒ�
	pVtx[0].col = g_Nomoretime.col;
	pVtx[1].col = g_Nomoretime.col;
	pVtx[2].col = g_Nomoretime.col;
	pVtx[3].col = g_Nomoretime.col;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNomoretime->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitNomoretime(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureNomoretime != NULL)
	{
		g_pTextureNomoretime->Release();
		g_pTextureNomoretime = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffNomoretime != NULL)
	{
		g_pVtxBuffNomoretime->Release();
		g_pVtxBuffNomoretime = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateNomoretime(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNomoretime->Lock(0, 0, (void**)&pVtx, 0);

	if (GetTimer() <= NOMORE_TIME)
	{
		g_Nomoretime.pos.x -= 7.0f;
	}

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[0].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[1].pos.y = g_Nomoretime.pos.y - g_Nomoretime.size.hy;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Nomoretime.pos.x - g_Nomoretime.size.hx;
	pVtx[2].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Nomoretime.pos.x + g_Nomoretime.size.hx;
	pVtx[3].pos.y = g_Nomoretime.pos.y + g_Nomoretime.size.hy;
	pVtx[3].pos.z = 0.0f;

	g_nCounterNomoretime++;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNomoretime->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawNomoretime(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffNomoretime, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureNomoretime);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);											// �`�悷��v���~�e�B�u�̐�
	
}