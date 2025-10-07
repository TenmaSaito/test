//================================================================================================================
//
// DirectX�̌��̕\������ [moon.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "moon.h"
#include "input.h"

// �}�N����`
#define MOON_SIZE		(100.0f)				// ���z�̑傫��

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureMoon = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMoon = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_MoonPos = D3DXVECTOR3_NULL;		// ���z�̈ʒu
int g_nAnimatonCounterMoon;
int g_nCounterMoon;

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitMoon(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\MOB\\MOON_ANIMATION.png",
							  &g_pTextureMoon);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffMoon,
								NULL);

	g_MoonPos = D3DXVECTOR3(180.0f, 100.0f, 0.0f);
	g_nAnimatonCounterMoon = 0; 
	g_nCounterMoon = 0;

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMoon->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_MoonPos.x - MOON_SIZE;
	pVtx[0].pos.y = g_MoonPos.y - MOON_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_MoonPos.x + MOON_SIZE;
	pVtx[1].pos.y = g_MoonPos.y - MOON_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_MoonPos.x - MOON_SIZE;
	pVtx[2].pos.y = g_MoonPos.y + MOON_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_MoonPos.x + MOON_SIZE;
	pVtx[3].pos.y = g_MoonPos.y + MOON_SIZE;
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
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMoon->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitMoon(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureMoon != NULL)
	{
		g_pTextureMoon->Release();
		g_pTextureMoon = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffMoon != NULL)
	{
		g_pVtxBuffMoon->Release();
		g_pVtxBuffMoon = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateMoon(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMoon->Lock(0, 0, (void**)&pVtx, 0);

	if (g_nCounterMoon % 45 == 0)
	{
		g_nAnimatonCounterMoon = g_nAnimatonCounterMoon ^ 1;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterMoon, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterMoon + 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterMoon, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f * g_nAnimatonCounterMoon + 0.5f, 1.0f);
	}

	g_nCounterMoon++;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMoon->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawMoon(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMoon, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMoon);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
					       0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);						// �`�悷��v���~�e�B�u�̐�
}