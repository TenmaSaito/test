//================================================================================================================
//
// DirectX�̃^�C�}�[���� [timer.cpp]
// Author : TENMA
//
//================================================================================================================
#include "game.h"
#include "timer.h"
#include "gauge.h"
#include "sound.h"

// �}�N����`
#define NUM_PLACE			(3)				// �^�C�}�[�̌���
#define NUM_WIDTH			(25.0f)			// �^�C�}�[�̐����̉���
#define NUM_HEIGHT			(30.0f)			// �^�C�}�[�̐����̏c��
#define NUM_SPACE			(40.0f)			// �^�C�}�[�̐����̌���
#define TIMERGAUGE_COLOR	D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)		// �^�C�}�[�̃Q�[�W�̐F

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureTimer = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posTimer;							// �^�C�}�[��\������ʒu
D3DXCOLOR g_colTimer;							// �^�C�}�[�̐F
int g_nTimer;									// �^�C�}�[�̒l
int g_nGaugeTimer;								// �^�C�}�[�̃Q�[�W��No
int g_nAlphaTimer;								// ���l�ύX

// �^�C�}�[�̏���������
void InitTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntTimer;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureTimer);

	g_posTimer = D3DXVECTOR3(610.0f, 48.0f, 0.0f);		// �ʒu��������
	g_colTimer = D3DXCOLOR_NULL;
	g_nTimer = 0;								// �l��������
	g_nAlphaTimer = 1;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,			// NUM_PLACE���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < NUM_PLACE; nCntTimer++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posTimer.x + (NUM_SPACE * nCntTimer) - NUM_WIDTH;
		pVtx[0].pos.y = g_posTimer.y - NUM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posTimer.x + (NUM_SPACE * nCntTimer) + NUM_WIDTH;
		pVtx[1].pos.y = g_posTimer.y - NUM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posTimer.x + (NUM_SPACE * nCntTimer) - NUM_WIDTH;
		pVtx[2].pos.y = g_posTimer.y + NUM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posTimer.x + (NUM_SPACE * nCntTimer) + NUM_WIDTH;
		pVtx[3].pos.y = g_posTimer.y + NUM_HEIGHT;
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

	g_pVtxBuffTimer->Unlock();

}

// �^�C�}�[�̏I������
void UninitTimer(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

// �^�C�}�[�̍X�V����
void UpdateTimer(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	// ��ŏ��������Ꮡ��
	if (g_nTimer == NOMORE_TIME && GetPlaySound() != SOUND_LABEL_GAME_NOMORETIME)
	{
		StopSound(GetPlaySound());
		PlaySound(SOUND_LABEL_GAME_NOMORETIME);
	}
	else if(g_nTimer < NOMORE_TIME)
	{
		for (int nCntTimer = 0; nCntTimer < NUM_PLACE; nCntTimer++)
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = g_colTimer;
			pVtx[1].col = g_colTimer;
			pVtx[2].col = g_colTimer;
			pVtx[3].col = g_colTimer;

			pVtx += 4;
		}
	}

	g_pVtxBuffTimer->Unlock();
}

// �^�C�}�[�̕`�揈��
void DrawTimer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntTimer;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimer, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimer);

	for (nCntTimer = 0; nCntTimer < NUM_PLACE; nCntTimer++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			4 * nCntTimer,								// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

// �^�C�}�[�̐ݒ菈��
void SetTimer(int nTimer)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	int aTexU[NUM_PLACE];				//�e���̐��������[
	int nDiff = 1;
	int nCntTimer;

	g_nTimer = nTimer;

	for (nCntTimer = 0; nCntTimer < NUM_PLACE; nCntTimer++)
	{
		aTexU[nCntTimer] = g_nTimer % (int)powf(10.0f, (float)(NUM_PLACE - nCntTimer)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntTimer) - 1.0f);
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < NUM_PLACE; nCntTimer++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	// �^�C�}�[�Ɠ��������Q�[�W���쐬
	g_nGaugeTimer = SetGauge(D3DXVECTOR3(531.0f, 49.0f, 0.0f), TIMERGAUGE_COLOR, GAUGETYPE_TIMER, GAUGE_MAX, 27.0f);


	g_pVtxBuffTimer->Unlock();
}

// �^�C�}�[�̎擾
int GetTimer(void)
{
	return g_nTimer;
}

// �^�C�}�[�̒ǉ�����
void AddTimer(int nValue)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	int aTexU[NUM_PLACE];				// �e���̐��������[
	int nCntTimer;

	g_nTimer += nValue;

	if (g_nTimer <= 0)
	{
		g_nTimer = 0;
	}

	if (GetGauge()[g_nGaugeTimer].nPercentGauge > 0)
	{
		AddGauge(nValue, g_nGaugeTimer);
	}

	for (nCntTimer = 0; nCntTimer < NUM_PLACE; nCntTimer++)
	{
		aTexU[nCntTimer] = g_nTimer % (int)powf(10.0f,(float)(NUM_PLACE - nCntTimer)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntTimer) - 1.0f);
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < NUM_PLACE; nCntTimer++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntTimer]) + 0.1f, 1.0f);

		pVtx += 4;
	}


	g_pVtxBuffTimer->Unlock();

}