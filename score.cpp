//================================================================================================================
//
// DirectX�̃X�R�A���� [score.cpp]
// Author : TENMA
//
//================================================================================================================
#include "score.h"

// �}�N����`
#define NUM_PLACE			(8)				// �X�R�A�̌���

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureScore = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posScore;							// �X�R�A��\������ʒu
int g_nScore;									// �X�R�A�̒l

// �X�R�A�̏���������
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureScore);

	g_posScore = D3DXVECTOR3(980.0f, 62.5f, 0.0f);		// �ʒu��������
	g_nScore = 0;										// �l��������

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[0].pos.y = g_posScore.y - 28.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[1].pos.y = g_posScore.y - 28.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[2].pos.y = g_posScore.y + 28.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[3].pos.y = g_posScore.y + 28.0f;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();

}

// �X�R�A�̏I������
void UninitScore(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

// �X�R�A�̍X�V����
void UpdateScore(void)
{
	// ��ŏ��������Ꮡ��
	VERTEX_2D* pVtx = NULL;

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[0].pos.y = g_posScore.y - 28.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[1].pos.y = g_posScore.y - 28.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posScore.x + (35.0f * nCntScore) - 15.0f;
		pVtx[2].pos.y = g_posScore.y + 28.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posScore.x + (35.0f * nCntScore) + 15.0f;
		pVtx[3].pos.y = g_posScore.y + 28.0f;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();
}

// �X�R�A�̕`�揈��
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			4 * nCntScore,								// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

// �X�R�A�̐ݒ菈��
void SetScore(int nScore)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	int aTexU[NUM_PLACE];				//�e���̐��������[
	int nDiff = 1;
	int nCntScore;

	g_nScore = nScore;

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScore) - 1.0f);
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();
}

// �X�R�A�̎擾
int GetScore(void)
{
	return g_nScore;
}

// �X�R�A�̒ǉ�����
void AddScore(int nValue)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	int aTexU[NUM_PLACE];				//�e���̐��������[
	int nCntScore;

	g_nScore += nValue;
	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		aTexU[nCntScore] = g_nScore % (int)powf(10.0f,(float)(NUM_PLACE - nCntScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScore) - 1.0f);
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffScore->Unlock();

}