//================================================================================================================
//
// DirectX�̒e�̔��ˏ��� [Explosion.cpp]
// Author : TENMA
//
//================================================================================================================
#include "explosion.h"
#include "player.h"

// �}�N����`
#define MAX_EXPLOSION		(512)									// �����̍ő吔
#define EXPLOSION_SIZE		(100)									// �����̃T�C�Y
#define ANIM_SIZE			(0.125f)								// �e�N�X�`���̋�؂�̃T�C�Y 
#define ALPHA_DECREASE		(0.125f)								// �e�N�X�`����ALPHA�l�̌�������l
#define TEX_ADDRESS			"data\\TEXTURE\\explosion000.png"		// �e�N�X�`���̃A�h���X

// �e�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				// �����̈ʒu
	D3DXCOLOR col;					// �����̐F
	int g_nCounterAnimExplosion;	// �A�j���[�V�����J�E���^�[
	int g_nPatternAnimExplosion;	// �A�j���[�V�����p�^�[��No.
	float g_fExplosionAlpha;		// �����A�j���[�V������Alpha�l�̌����W��
	bool bUse;						// �e���g�p���Ă��邩�ǂ���
}Explosion;

LPDIRECT3DTEXTURE9		g_pTextureExplosion = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL;	// ���_�o�b�t�@�̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION];						// �e�̏��
float g_fLengthExplosion;								// �Ίp���̒���
float g_fAngleExplosion;								// �Ίp���̊p�x

// �e�̏�����
void InitExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntExplosion;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		TEX_ADDRESS,
		&g_pTextureExplosion);

	// �e�̏��̏�����
	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		g_aExplosion[nCntExplosion].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aExplosion[nCntExplosion].g_nCounterAnimExplosion = 0;
		g_aExplosion[nCntExplosion].g_nPatternAnimExplosion = 0;
		g_aExplosion[nCntExplosion].g_fExplosionAlpha = 0.0f;
		g_aExplosion[nCntExplosion].bUse = false;							// ���g�p��Ԃɐݒ�
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EXPLOSION,			// MAX_EXPLOSION���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffExplosion,
		NULL);

	// �Ίp���̒������擾
	g_fLengthExplosion = sqrtf(EXPLOSION_SIZE * EXPLOSION_SIZE + EXPLOSION_SIZE * EXPLOSION_SIZE) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_fAngleExplosion = atan2f(EXPLOSION_SIZE, EXPLOSION_SIZE);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[0].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[1].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(-g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[2].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(-g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aExplosion[nCntExplosion].pos.x + sinf(g_fAngleExplosion) * g_fLengthExplosion;
		pVtx[3].pos.y = g_aExplosion[nCntExplosion].pos.y + cosf(g_fAngleExplosion) * g_fLengthExplosion;
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
		pVtx[0].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((g_aExplosion[nCntExplosion].g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 1.0f);

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	g_pVtxBuffExplosion->Unlock();
}

// �e�̏I������
void UninitExplosion(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureExplosion != NULL)
	{
		g_pTextureExplosion->Release();
		g_pTextureExplosion = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffExplosion != NULL)
	{
		g_pVtxBuffExplosion->Release();
		g_pVtxBuffExplosion = NULL;
	}
}

// �e�̍X�V����
void UpdateExplosion(void)
{
	int nCntExplosion;

	VERTEX_2D *pVtx;					// ���_���ւ̃|�C���^
	PLAYER *pPlayer = GetPlayer();		// �v���C���[�ւ̃|�C���^
	Explosion *pExplosion = &g_aExplosion[0];		// �\���̂̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++,pExplosion++)
	{
		if (pExplosion->bUse == true)
		{
			pExplosion->g_nCounterAnimExplosion++;
			if ((pExplosion->g_nCounterAnimExplosion % 4) == 0)
			{
				pExplosion->g_nPatternAnimExplosion++;

				// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
				pVtx[0].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[0].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[0].pos.z = 0.0f;																					

				pVtx[1].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[1].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[1].pos.z = 0.0f;

				pVtx[2].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(-g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[2].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(-g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[2].pos.z = 0.0f;

				pVtx[3].pos.x = (pExplosion->pos.x + pPlayer->moveposPlayer.x) + sinf(g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[3].pos.y = (pExplosion->pos.y + pPlayer->moveposPlayer.y) + cosf(g_fAngleExplosion) * g_fLengthExplosion;
				pVtx[3].pos.z = 0.0f;

				// ���_�J���[�̐ݒ�[�t�F�[�h�A�E�g���L���Ȃ珙�X�ɓ����ɂȂ�
				pVtx[0].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				pVtx[1].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				pVtx[2].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				pVtx[3].col = D3DXCOLOR(
					pExplosion->col.r,
					pExplosion->col.g,
					pExplosion->col.b,
					pExplosion->col.a
					- (pExplosion->g_nPatternAnimExplosion * pExplosion->g_fExplosionAlpha));

				// �e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 0.0f);
				pVtx[1].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 1.0f);
				pVtx[3].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 1.0f);

				if (pExplosion->g_nPatternAnimExplosion > 7)
				{// �A�j���[�V�������K��l�𒴂�����
					// �A�j���[�V�����̏I��
					pExplosion->bUse = false;			// ���g�p��Ԃ֕ύX
				}
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffExplosion->Unlock();
}

// �e�̕`�揈��
void DrawExplosion(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntExplosion;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureExplosion);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++)
	{
		if (g_aExplosion[nCntExplosion].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntExplosion,							// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

// �e�̐ݒ菈��
void SetExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, bool bFadeout)
{
	int nCntExplosion;

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->moveposPlayer;
	Explosion* pExplosion = &g_aExplosion[0];

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntExplosion = 0; nCntExplosion < MAX_EXPLOSION; nCntExplosion++,pExplosion++)
	{
		if (pExplosion->bUse == false)
		{
			pExplosion->pos = pos;
			pExplosion->col = col;
			pExplosion->g_fExplosionAlpha = 0.0f;

			pExplosion->g_nPatternAnimExplosion = 0;
			
			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[0].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(D3DX_PI + g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[1].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(D3DX_PI - g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(-g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[2].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(-g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pExplosion->pos.x + posPlayer.x) + sinf(g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[3].pos.y = (pExplosion->pos.y + posPlayer.y) + cosf(g_fAngleExplosion) * g_fLengthExplosion;
			pVtx[3].pos.z = 0.0f;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((pExplosion->g_nPatternAnimExplosion * ANIM_SIZE) + ANIM_SIZE, 1.0f);

			pExplosion->bUse = true;			// �g�p�ς݂ɕύX

			if (bFadeout == true)
			{// �����t�F�[�h�A�E�g���L���Ȃ�
				// �t�F�[�h�A�E�g�̒l��ύX
				pExplosion->g_fExplosionAlpha = ALPHA_DECREASE;
			}

			break;
		}

		pVtx += 4;
	}

	g_pVtxBuffExplosion->Unlock();
}