//================================================================================================================
//
// DirectX�̈ړ����X�R�A���� [ResultScore.cpp]
// Author : TENMA
//
//================================================================================================================
#include "ResultScore.h"
#include "input.h" 
#include "score.h"
#include "scoreRank.h"

// �}�N����`
#define NUM_PLACE			(8)				// �X�R�A�̌���
#define NUM_WIDTH			(45)			// �ꌅ�̉���
#define NUM_HEIGHT			(55)			// �ꌅ�̏c��
#define GOAL_SCOREPOS		D3DXVECTOR3(350.0f, 500.0f, 0.0f)		// �X�R�A�̍ŏI�ʒu
#define RANK_POS			D3DXVECTOR3(WINDOW_MID.x, WINDOW_MID.y - 30.0f, 0.0f)	// �����N�̕\���ʒu

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureResultScore = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posResultScore;							// �X�R�A��\������ʒu
int g_nResultScore;										// �X�R�A�̒l

// �X�R�A�̏���������
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntResultScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureResultScore);

	g_posResultScore = D3DXVECTOR3(1300.0f, 62.5f, 0.0f);		// �ʒu��������
	g_nResultScore = 0;											// �l��������

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[0].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[1].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[2].pos.y = g_posResultScore.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[3].pos.y = g_posResultScore.y + NUM_HEIGHT;
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

	g_pVtxBuffResultScore->Unlock();

}

// �X�R�A�̏I������
void UninitResultScore(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

// �X�R�A�̍X�V����
void UpdateResultScore(void)
{
	if (g_posResultScore.x <= GOAL_SCOREPOS.x)
	{
		g_posResultScore.x = GOAL_SCOREPOS.x;
		SetScoreRank(RANK_POS, GetScore());
	}

	// ��ŏ��������Ꮡ��
	VERTEX_2D* pVtx = NULL;

	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[0].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[1].pos.y = g_posResultScore.y - NUM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) - NUM_WIDTH;
		pVtx[2].pos.y = g_posResultScore.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posResultScore.x + ((NUM_WIDTH * 2) * nCntResultScore) + NUM_WIDTH;
		pVtx[3].pos.y = g_posResultScore.y + NUM_HEIGHT;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	g_pVtxBuffResultScore->Unlock();
}

// �X�R�A�̕`�揈��
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntResultScore;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultScore);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			4 * nCntResultScore,								// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

// �X�R�A�̐ݒ菈��
void SetResultScore(D3DXVECTOR3 pos, int nResultScore)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	int aTexU[NUM_PLACE];				//�e���̐��������[
	int nDiff = 1;
	int nCntResultScore;

	g_posResultScore = pos;
	g_nResultScore = nResultScore;

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		aTexU[nCntResultScore] = g_nResultScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore) - 1.0f);
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffResultScore->Unlock();
}

// �X�R�A�̎擾
int GetResultScore(void)
{
	return g_nResultScore;
}

// �X�R�A�̒ǉ�����
void AddResultScore(int nValue)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	int aTexU[NUM_PLACE];				//�e���̐��������[
	int nCntResultScore;

	g_nResultScore += nValue;
	if (g_nResultScore <= 0)
	{
		g_nResultScore = 0;
	}

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		aTexU[nCntResultScore] = g_nResultScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntResultScore) - 1.0f);
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResultScore = 0; nCntResultScore < NUM_PLACE; nCntResultScore++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntResultScore]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffResultScore->Unlock();

}

void MoveResuktScore(D3DXVECTOR3 move)
{
	g_posResultScore += move;
}