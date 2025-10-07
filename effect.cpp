//================================================================================================================
//
// DirectX�̃G�t�F�N�g���� [effect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "effect.h"
#include "player.h"

// �}�N����`
#define MAX_EFFECT			(12250)	// �G�t�F�N�g�̍ő吔
#define ALPHA_DECREASE		(0.03f)	// Alpha�l�̌����W��

// �G�t�F�N�g�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			// �G�t�F�N�g�̈ʒu
	D3DXCOLOR col;				// �G�t�F�N�g�̐F
	D3DXCOLOR colChange;		// �G�t�F�N�g�̕ς�镝
	D3DXVECTOR3 rot;			// �G�t�F�N�g�̕���
	D3DXVECTOR3 move;			// �G�t�F�N�g�̑��x
	float fRadius;				// �G�t�F�N�g�̔��a
	int nLife;					// �e�̎���
	bool bParticleMove;			// �p�[�e�B�N�����v���C���[�̓����ɍ��킹�Ă��炷��
	bool bColRainbow;			// ���F�Ɍ��邩
	bool bUse;					// �e���g�p���Ă��邩�ǂ���
}EFFECT;


LPDIRECT3DTEXTURE9		g_pTextureEffect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;	// ���_�o�b�t�@�̃|�C���^
EFFECT g_aEffect[MAX_EFFECT];						// �G�t�F�N�g�̏��

// �G�t�F�N�g�̏�����
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	int nCnteffect;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);

	EFFECT *pEffect = &g_aEffect[0];		// �G�t�F�N�g�̐擪�A�h���X����

	// �G�t�F�N�g�̏��̏�����
	for (int nCnteffect = 0; nCnteffect < MAX_EFFECT; nCnteffect++,pEffect++)
	{
		pEffect->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pEffect->colChange = D3DXCOLOR(0.1f, 0.2f, 0.3f, 0.1f);
		pEffect->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pEffect->fRadius = 0.0f;
		pEffect->nLife = 0;
		pEffect->bColRainbow = false;
		pEffect->bUse = false;							// ���g�p��Ԃɐݒ�
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,			// MAX_EFFECT���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	pEffect = &g_aEffect[0];		// �G�t�F�N�g�̐擪�A�h���X����(���Z�b�g)

	for (nCnteffect = 0; nCnteffect < MAX_EFFECT; nCnteffect++,pEffect++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pEffect->pos.x - pEffect->fRadius;
		pVtx[0].pos.y = pEffect->pos.y - pEffect->fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pEffect->pos.x + pEffect->fRadius;
		pVtx[1].pos.y = pEffect->pos.y - pEffect->fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pEffect->pos.x - pEffect->fRadius;
		pVtx[2].pos.y = pEffect->pos.y + pEffect->fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pEffect->pos.x + pEffect->fRadius;
		pVtx[3].pos.y = pEffect->pos.y + pEffect->fRadius;
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	g_pVtxBuffEffect->Unlock();
}

// �G�t�F�N�g�̏I������
void UninitEffect(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

// �G�t�F�N�g�̍X�V����
void UpdateEffect(void)
{
	VERTEX_2D* pVtx;

	EFFECT *pEffect = &g_aEffect[0];			// �G�t�F�N�g�̐擪�A�h���X����
	PLAYER *pPlayer = GetPlayer();				// �v���C���[�̃A�h���X����

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnteffect = 0; nCnteffect < MAX_EFFECT; nCnteffect++,pEffect++)
	{
		if (pEffect->bUse == true)
		{// �����G�t�F�N�g���g���Ă����

			pEffect->fRadius -= pEffect->fRadius * 0.02f;					// ���a���k��
			pEffect->col.a -= pEffect->col.a * 0.03f;						// �A���t�@�l������
			pEffect->pos.x += sinf(pEffect->rot.z) * pEffect->move.z;		// �ʒu���X�V
			pEffect->pos.y += cosf(pEffect->rot.z) * pEffect->move.z;		// �ʒu���X�V

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) - pEffect->fRadius;
			pVtx[0].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) - pEffect->fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) + pEffect->fRadius;
			pVtx[1].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) - pEffect->fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) - pEffect->fRadius;
			pVtx[2].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) + pEffect->fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pEffect->pos.x + pPlayer->moveposPlayer.x) + pEffect->fRadius;
			pVtx[3].pos.y = (pEffect->pos.y + pPlayer->moveposPlayer.y) + pEffect->fRadius;
			pVtx[3].pos.z = 0.0f;

			if (pEffect->bColRainbow)
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(pEffect->col.r,0.0f,0.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(0.0f, pEffect->col.g, 0.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, pEffect->col.b, 1.0f);
				pVtx[3].col = D3DXCOLOR(pEffect->col.r, pEffect->col.g, pEffect->col.b, 1.0f);
			}
			else
			{
				// ���_�J���[�̐ݒ�
				pVtx[0].col = pEffect->col;
				pVtx[1].col = pEffect->col;
				pVtx[2].col = pEffect->col;
				pVtx[3].col = pEffect->col;
			}

			pEffect->nLife -= 1;			// �G�t�F�N�g�̗̑͂�����������
			if (pEffect->nLife == 0)
			{// �G�t�F�N�g�`����莞�Ԍo�߂�����
				pEffect->bUse = false;			// ���g�p��Ԃ֕ύX
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffEffect->Unlock();
}

// �G�t�F�N�g�̕`�揈��
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);


#if 1
	// �A���t�@�u�����f�B���O�����Z�����ɐݒ�I
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

#endif

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntEffect,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}

	// �A���t�@�u�����f�B���O�����ɖ߂��I�I(�d�v�I)
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

// �G�t�F�N�g�̐ݒ菈��
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move,D3DXCOLOR col, float fRadius, int nLife, bool bUseColRainbow)
{
	VERTEX_2D *pVtx;
	EFFECT *pEffect = &g_aEffect[0];			// �G�t�F�N�g�̐擪�A�h���X����
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = pPlayer->moveposPlayer;

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++,pEffect++)
		{
			if (pEffect->bUse == false)
			{
				pEffect->pos = pos;									// �G�t�F�N�g�̈ʒu����
				pEffect->col = col;									// �G�t�F�N�g�̐F����
				pEffect->fRadius = fRadius;							// �G�t�F�N�g�̔��a����
				pEffect->nLife = nLife;								// �G�t�F�N�g�̎�������
				pEffect->move = move;								// �ȑO�̈ړ��ʂ�������
				pEffect->bColRainbow = bUseColRainbow;

				// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
				pVtx[0].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
				pVtx[0].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
				pVtx[0].pos.z = 0.0f;

				pVtx[1].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
				pVtx[1].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
				pVtx[2].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
				pVtx[3].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
				pVtx[3].pos.z = 0.0f;

				// ���_�J���[�̐ݒ�
				pVtx[0].col = pEffect->col;
				pVtx[1].col = pEffect->col;
				pVtx[2].col = pEffect->col;
				pVtx[3].col = pEffect->col;

				pEffect->bUse = true;				// �e���g�p�ς݂ɂ���

				break;
			}
			pVtx += 4;
		}
	
	g_pVtxBuffEffect->Unlock();
}

void SetParticle(D3DXVECTOR3 pos, D3DXCOLOR col,D3DXVECTOR3 move, float fRadius, int nLife, float fRandMin, float fRandMax, bool bParticleMove)
{
	VERTEX_2D* pVtx;
	EFFECT* pEffect = &g_aEffect[0];			// �G�t�F�N�g�̐擪�A�h���X����
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = pPlayer->moveposPlayer;

	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == false)
		{
			pEffect->pos = pos;				// �G�t�F�N�g�̈ʒu����
			pEffect->col = col;				// �G�t�F�N�g�̐F����
			pEffect->move = move;			// �G�t�F�N�g�̑��x����
			pEffect->fRadius = fRadius;		// �G�t�F�N�g�̔��a����
			pEffect->nLife = nLife;			// �G�t�F�N�g�̎�������
			pEffect->bParticleMove = bParticleMove;
			if (fRandMax == fRandMin)
			{
				pEffect->rot.z = fRandMax;
			}
			else 
			{
				pEffect->rot.z = (float)((rand() % (int)((fRandMax * 100.0f) - (fRandMin * 100.0f))) * 0.01f);
			}

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
			pVtx[0].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
			pVtx[1].pos.y = (pEffect->pos.y + Camerapos.y) - pEffect->fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pEffect->pos.x + Camerapos.x) - pEffect->fRadius;
			pVtx[2].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pEffect->pos.x + Camerapos.x) + pEffect->fRadius;
			pVtx[3].pos.y = (pEffect->pos.y + Camerapos.y) + pEffect->fRadius;
			pVtx[3].pos.z = 0.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			pEffect->bUse = true;				// �e���g�p�ς݂ɂ���

			break;
		}
		pVtx += 4;
	}

	g_pVtxBuffEffect->Unlock();
}

// �G�t�F�N�g�̑S����
void DestroyEffect(void)
{
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse != true)continue;

		g_aEffect[nCntEffect].bUse = false;
	}
}