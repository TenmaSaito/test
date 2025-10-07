//================================================================================================================
//
// DirectX�̓�Փx�\������ [showDifficulty.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "showDifficulty.h"
#include "input.h"
#include "player.h"

// �}�N����`
#define MAX_SHOWDIFFICULTY		(10)		// ��Փx�̍ő哯���o����

// ��Փx�\���\����
typedef struct
{
	D3DXVECTOR3 pos;				// �ʒu
	D3DXCOLOR col;					// �F
	POLY_SIZE size;					// �T�C�Y
	GAMEDIFFICULTY difficulty;		// ��Փx
	bool bUse;						// �g�p��
}SHOWDIFFICULTY;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureShowDifficulty = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShowDifficulty = NULL;	// ���_�o�b�t�@�̃|�C���^
SHOWDIFFICULTY g_aShowDifficulty[MAX_SHOWDIFFICULTY];		

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏���������
//================================================================================================================
void InitShowDifficulty(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	SHOWDIFFICULTY *pShowDifficulty = &g_aShowDifficulty[0];
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		pShowDifficulty->pos = D3DXVECTOR3_NULL;
		pShowDifficulty->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		pShowDifficulty->size = POLY_SIZE(300.0f, 125.0f);
		pShowDifficulty->difficulty = GAMEDIFFICULTY_NORMAL;
		pShowDifficulty->bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							  "data\\TEXTURE\\CHARACTER\\CHARTEX\\GAMEDIFFICULTY.png",
							  &g_pTextureShowDifficulty);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SHOWDIFFICULTY,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffShowDifficulty,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShowDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	pShowDifficulty = &g_aShowDifficulty[0];

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++,pShowDifficulty++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pShowDifficulty->pos.x - pShowDifficulty->size.hx;
		pVtx[0].pos.y = pShowDifficulty->pos.y - pShowDifficulty->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pShowDifficulty->pos.x + pShowDifficulty->size.hx;
		pVtx[1].pos.y = pShowDifficulty->pos.y - pShowDifficulty->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pShowDifficulty->pos.x - pShowDifficulty->size.hx;
		pVtx[2].pos.y = pShowDifficulty->pos.y + pShowDifficulty->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pShowDifficulty->pos.x + pShowDifficulty->size.hx;
		pVtx[3].pos.y = pShowDifficulty->pos.y + pShowDifficulty->size.hy;
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�ݒ�
		pVtx[0].col = pShowDifficulty->col;
		pVtx[1].col = pShowDifficulty->col;
		pVtx[2].col = pShowDifficulty->col;
		pVtx[3].col = pShowDifficulty->col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty));
		pVtx[1].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty));
		pVtx[2].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShowDifficulty->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̏I������
//================================================================================================================
void UninitShowDifficulty(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureShowDifficulty != NULL)
	{
		g_pTextureShowDifficulty->Release();
		g_pTextureShowDifficulty = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffShowDifficulty != NULL)
	{
		g_pVtxBuffShowDifficulty->Release();
		g_pVtxBuffShowDifficulty = NULL;
	}
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̍X�V����
//================================================================================================================
void UpdateShowDifficulty(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	SHOWDIFFICULTY* pShowDifficulty = &g_aShowDifficulty[0];
	D3DXVECTOR3 Camerapos = D3DXVECTOR3_NULL;
	PLAYER *pPlayer = GetPlayer();

	if (GetMode() == MODE_GAME)
	{
		Camerapos = pPlayer->moveposPlayer;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShowDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		if (pShowDifficulty->bUse == true)
		{
			if (pShowDifficulty->col.a < 1.0f)
			{
				pShowDifficulty->col.a += 0.01f;
			}

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[0].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[1].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[2].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[3].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�ݒ�
			pVtx[0].col = pShowDifficulty->col;
			pVtx[1].col = pShowDifficulty->col;
			pVtx[2].col = pShowDifficulty->col;
			pVtx[3].col = pShowDifficulty->col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[2].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShowDifficulty->Unlock();
}

//================================================================================================================
// �Q�[���I�[�o�[��ʂ̕`�揈��
//================================================================================================================
void DrawShowDifficulty(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffShowDifficulty, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureShowDifficulty);

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++)
	{
		if (g_aShowDifficulty[nCntShow].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntShow,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// ��Փx�\������
//================================================================================================================
void SetShowDifficulty(D3DXVECTOR3 pos, GAMEDIFFICULTY difficulty)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	SHOWDIFFICULTY* pShowDifficulty = &g_aShowDifficulty[0];
	D3DXVECTOR3 Camerapos = D3DXVECTOR3_NULL;
	PLAYER* pPlayer = GetPlayer();

	if (GetMode() == MODE_GAME)
	{
		Camerapos = pPlayer->moveposPlayer;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffShowDifficulty->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		if (pShowDifficulty->bUse != true)
		{
			pShowDifficulty->pos = pos;
			pShowDifficulty->difficulty = difficulty;
			pShowDifficulty->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[0].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[1].pos.y = (pShowDifficulty->pos.y + Camerapos.y) - pShowDifficulty->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pShowDifficulty->pos.x + Camerapos.x) - pShowDifficulty->size.hx;
			pVtx[2].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pShowDifficulty->pos.x + Camerapos.x) + pShowDifficulty->size.hx;
			pVtx[3].pos.y = (pShowDifficulty->pos.y + Camerapos.y) + pShowDifficulty->size.hy;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�ݒ�
			pVtx[0].col = pShowDifficulty->col;
			pVtx[1].col = pShowDifficulty->col;
			pVtx[2].col = pShowDifficulty->col;
			pVtx[3].col = pShowDifficulty->col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[1].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty));
			pVtx[2].tex = D3DXVECTOR2(0.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, (0.25f * pShowDifficulty->difficulty) + 0.25f);

			pShowDifficulty->bUse = true;

			break;
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffShowDifficulty->Unlock();
}

//================================================================================================================
// ��Փx�̔�\������
//================================================================================================================
void DestroyShowDifficulty(void)
{
	SHOWDIFFICULTY* pShowDifficulty = &g_aShowDifficulty[0];

	for (int nCntShow = 0; nCntShow < MAX_SHOWDIFFICULTY; nCntShow++, pShowDifficulty++)
	{
		if (pShowDifficulty->bUse == true)
		{
			pShowDifficulty->bUse = false;
		}
	}
}