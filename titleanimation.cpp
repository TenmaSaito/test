//================================================================================================================
//
// DirectX�̃��U���g��ʂ̔w�i�\��������cpp [TitleAnimation.cpp]
// Author : TENMA
//
//================================================================================================================
#include "titleanimation.h"

// �}�N����`
#define ANIMATIONPNG_SIZE		(2000)				// �A�j���[�V�����̃T�C�Y

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureTitleAnimation = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleAnimation = NULL;	// ���_�o�b�t�@�̃|�C���^
float g_fLengthTitleAnimation = 0.0f;						// �Ίp���̒���
float g_fAngleTitleAnimation = 0.0f;						// �Ίp���̊p�x
float g_fAngleMove = 0.0f;									// ��]�����p�x

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitTitleAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_ANIMATION.png",
		&g_pTextureTitleAnimation);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleAnimation,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	g_fLengthTitleAnimation = 0.0f;
	g_fAngleTitleAnimation = 0.0f;
	g_fAngleMove = 0.0f;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleAnimation->Lock(0, 0, (void**)&pVtx, 0);

	// �Ίp���̒������擾
	g_fLengthTitleAnimation = sqrtf(powf(ANIMATIONPNG_SIZE, 2.0f) + powf(ANIMATIONPNG_SIZE, 2.0f)) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_fAngleTitleAnimation = atan2f(ANIMATIONPNG_SIZE, ANIMATIONPNG_SIZE);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.y = SCREEN_HEIGHT + cosf(D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.y = SCREEN_HEIGHT + cosf(D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(-g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.y = SCREEN_HEIGHT + cosf(-g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[3].pos.y = SCREEN_HEIGHT + cosf(g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
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
	g_pVtxBuffTitleAnimation->Unlock();
}

//================================================================================================================
// ���U���g�̏I������
//================================================================================================================
void UninitTitleAnimation(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureTitleAnimation != NULL)
	{
		g_pTextureTitleAnimation->Release();
		g_pTextureTitleAnimation = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffTitleAnimation != NULL)
	{
		g_pVtxBuffTitleAnimation->Release();
		g_pVtxBuffTitleAnimation = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateTitleAnimation(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	g_fAngleMove += 0.002f;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleAnimation->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove + D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove + D3DX_PI + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove + D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove + D3DX_PI - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove - g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = (SCREEN_WIDTH * 0.5f) + sinf(g_fAngleMove + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[3].pos.y = SCREEN_HEIGHT + cosf(g_fAngleMove + g_fAngleTitleAnimation) * g_fLengthTitleAnimation;
	pVtx[3].pos.z = 0.0f;

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleAnimation->Unlock();
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawTitleAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleAnimation, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTitleAnimation);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);						// �`�悷��v���~�e�B�u�̐�
}