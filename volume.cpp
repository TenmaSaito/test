//================================================================================================================
//
// DirectX�̉��ʒ������� [volume.cpp]
// Author : TENMA
//
//================================================================================================================
#include "volume.h"

// �}�N����`
#define NUM_PLACE			(3)								// ���ʂ̐����̌���
#define MAX_BUFFER			(NUM_PLACE * SETSOUND_MAX)		// �`�悷�鐔
#define NUM_WIDTH			(40)							// �����̉���
#define NUM_HEIGHT			(60)							// �����̏c��
#define NUM_GAP				(146)							// 2�̉��ʂ̐����̌���(Y���W)

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureVolume = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffVolume = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posVolume;							// �X�R�A��\������ʒu
int g_aVolume[SETSOUND_MAX];						// �X�R�A�̒l
bool g_bUseVolume;									// �g���Ă��邩

// ���ʒ����̏���������
void InitVolume(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntVolume;					

	// �e���ʂ̏�����
	g_aVolume[SETSOUND_GAME] = (int)(GetVolume(SETSOUND_GAME) * 100.0f);
	g_aVolume[SETSOUND_MENU_SE] = (int)(GetVolume(SETSOUND_MENU_SE) * 100.0f);
	g_bUseVolume = false;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureVolume);

	g_posVolume = D3DXVECTOR3(818 - ((NUM_WIDTH * 0.5f) + NUM_WIDTH), 214, 0);		// �ʒu��������

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BUFFER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffVolume,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffVolume->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntVolume = 0; nCntVolume < MAX_BUFFER; nCntVolume++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) - (NUM_WIDTH * 0.5f);
		pVtx[0].pos.y = g_posVolume.y - (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) + (NUM_WIDTH * 0.5f);
		pVtx[1].pos.y = g_posVolume.y - (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) - (NUM_WIDTH * 0.5f);
		pVtx[2].pos.y = g_posVolume.y + (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posVolume.x + (((NUM_WIDTH) * (nCntVolume % NUM_PLACE))) + (NUM_WIDTH * 0.5f);
		pVtx[3].pos.y = g_posVolume.y + (NUM_HEIGHT * 0.5f) + (NUM_GAP * (nCntVolume / NUM_PLACE));
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

	g_pVtxBuffVolume->Unlock();

}

// ���ʒ����̏I������
void UninitVolume(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureVolume != NULL)
	{
		g_pTextureVolume->Release();
		g_pTextureVolume = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffVolume != NULL)
	{
		g_pVtxBuffVolume->Release();
		g_pVtxBuffVolume = NULL;
	}
}

// ���ʒ����̍X�V����
void UpdateVolume(void)
{
	// ��ŏ��������Ꮡ��
}

// ���ʒ����̕`�揈��
void DrawVolume(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntVolume;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
  	pDevice->SetStreamSource(0, g_pVtxBuffVolume, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureVolume);

	for (nCntVolume = 0; nCntVolume < MAX_BUFFER; nCntVolume++)
	{
		if (g_bUseVolume == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntVolume,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

// ���ʂ̐ݒ菈��
void SetVolume(void)
{
	VERTEX_2D* pVtx;								// ���_���ւ̃|�C���^
	int aTexU[SETSOUND_MAX][NUM_PLACE];				// �e���̐��������[
	int nDiff = 1;
	int nCntVolume;
	int nCntType;
	g_bUseVolume = true;

	for (nCntType = 0; nCntType < SETSOUND_MAX; nCntType++)
	{
		for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
		{
			aTexU[nCntType][nCntVolume] = g_aVolume[nCntType] % (int)powf(10.0f, (float)(NUM_PLACE - nCntVolume)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntVolume) - 1.0f);
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffVolume->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntType = 0; nCntType < SETSOUND_MAX; nCntType++)
	{
		for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntType][nCntVolume]) + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	g_pVtxBuffVolume->Unlock();
}

// ���ʒ������l�̍폜
void DeleteVolume(void)
{
	g_bUseVolume = false;
}

// ���ʂ̕ύX����
void AddVolume(int nValue, SETSOUND type)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	int aTexU[SETSOUND_MAX][NUM_PLACE];				//�e���̐��������[
	int nCntVolume;

	g_aVolume[type] += nValue;
	if (g_aVolume[type] <= 0)
	{
		g_aVolume[type] = 0;
	}
	else if (g_aVolume[type] >= 100)
	{
		g_aVolume[type] = 100;
	}

	for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
	{
		aTexU[type][nCntVolume] = g_aVolume[type] % (int)powf(10.0f,(float)(NUM_PLACE - nCntVolume)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntVolume) - 1.0f);
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffVolume->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (4 * (type * NUM_PLACE));

	for (nCntVolume = 0; nCntVolume < NUM_PLACE; nCntVolume++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[type][nCntVolume]) + 0.1f, 1.0f);

		pVtx += 4;
	}

	g_pVtxBuffVolume->Unlock();

	SetVolume((float)(g_aVolume[type] * 0.01f), type);

}