//================================================================================================================
//
// DirectX�̈ړ����X�R�A���� [scoreRank.cpp]
// Author : TENMA
//
//================================================================================================================
#include "scoreRank.h"
#include "input.h"
#include "enemy.h"

// �}�N����`
#define RANK_SIZE			(70)				// �����N�̃T�C�Y
#define RANK_SS_SCORE		(1400000)			// SS�����N�̃X�R�A�̊
#define RANK_S_SCORE		(1000000)			// S�����N�̃X�R�A�
#define RANK_A_SCORE		(800000)			// A�����N�̃X�R�A�
#define RANK_B_SCORE		(500000)			// B�����N�̃X�R�A�
#define RANK_C_SCORE		(200000)			// C�����N�̃X�R�A�

// �����N�̎��
typedef enum
{
	RANKTYPE_C = 0,			// C�����N
	RANKTYPE_B,				// B�����N
	RANKTYPE_A,				// A�����N
	RANKTYPE_S,				// S�����N
	RANKTYPE_SS,			// SS�����N
	RANKTYPE_PEACEFULL,		// P�����N
	RANKTYPE_MAX
}RANKTYPE;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureScoreRank = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreRank = NULL;	// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posScoreRank;							// �X�R�A��\������ʒu
D3DXCOLOR g_colScoreRank;							// �����N�̃J���[�ݒ�
RANKTYPE g_Rank;									// �����N
bool g_bUseRank;									// �����N��\�����邩

// �X�R�A�̏���������
void InitScoreRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\RANK\\RANK_P.png",
		&g_pTextureScoreRank);

	g_posScoreRank = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu��������
	g_colScoreRank = D3DXCOLOR(1.0f,1.0f,1.0f,0.0f);
	g_Rank = RANKTYPE_C;									// �l��������
	g_bUseRank = false;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreRank,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[0].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[1].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[2].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[3].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

	g_pVtxBuffScoreRank->Unlock();
}

// �X�R�A�̏I������
void UninitScoreRank(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureScoreRank != NULL)
	{
		g_pTextureScoreRank->Release();
		g_pTextureScoreRank = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffScoreRank != NULL)
	{
		g_pVtxBuffScoreRank->Release();
		g_pVtxBuffScoreRank = NULL;
	}
}

// �X�R�A�̍X�V����
void UpdateScoreRank(void)
{
	// ��ŏ��������Ꮡ��
	VERTEX_2D* pVtx = NULL;

	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_RETURN) == true)
	{
		g_colScoreRank.a = 1.0f;
	}

	if (g_bUseRank == true && g_colScoreRank.a < 1.0f)
	{
		g_colScoreRank.a += 0.01f;
	}

	// ���_�J���[�̐ݒ�
	pVtx[0].col = g_colScoreRank;
	pVtx[1].col = g_colScoreRank;
	pVtx[2].col = g_colScoreRank;
	pVtx[3].col = g_colScoreRank;

	g_pVtxBuffScoreRank->Unlock();
}

// �X�R�A�̕`�揈��
void DrawScoreRank(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScoreRank, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureScoreRank);

	if (g_bUseRank == true)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

// �X�R�A�̐ݒ菈��
void SetScoreRank(D3DXVECTOR3 pos, int nScoreRank)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	g_posScoreRank = pos;
	g_bUseRank = true;

	if (nScoreRank >= RANK_SS_SCORE)
	{
		g_Rank = RANKTYPE_SS;
	}
	else if (nScoreRank >= RANK_S_SCORE)
	{
		g_Rank = RANKTYPE_S;
	}
	else if (nScoreRank >= RANK_A_SCORE)
	{
		g_Rank = RANKTYPE_A;
	}
	else if (nScoreRank >= RANK_B_SCORE)
	{
		g_Rank = RANKTYPE_B;
	}
	else if (nScoreRank < RANK_C_SCORE && GetKillcountEnemy() > 0)
	{
		g_Rank = RANKTYPE_C;
	}
	else if (GetKillcountEnemy() == 0)
	{
		g_Rank = RANKTYPE_PEACEFULL;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreRank->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[0].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[1].pos.y = g_posScoreRank.y - RANK_SIZE;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_posScoreRank.x - RANK_SIZE;
	pVtx[2].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_posScoreRank.x + RANK_SIZE;
	pVtx[3].pos.y = g_posScoreRank.y + RANK_SIZE;
	pVtx[3].pos.z = 0.0f;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((0.125f * g_Rank), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((0.125f * g_Rank) + 0.125f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((0.125f * g_Rank), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((0.125f * g_Rank) + 0.125f, 1.0f);

	g_pVtxBuffScoreRank->Unlock();
}