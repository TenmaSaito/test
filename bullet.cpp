//================================================================================================================
//
// DirectX�̒e�̔��ˏ��� [bullet.cpp]
// Author : TENMA
//
//================================================================================================================
#include "bullet.h"
#include "sound.h"
#include "explosion.h"
#include "player.h"
#include "enemy.h"
#include "effect.h"
#include "stage.h"
#include "particle.h"
#include "input.h"

// �}�N����`
#define EFFECT_COLOR1	D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)				// �G�t�F�N�g�̐F1
#define EFFECT_COLOR2	D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f)				// �G�t�F�N�g�̐F2
#define EFFECT_COLOR3	D3DXCOLOR(0.05f, 0.0f, 0.0f, 1.0f)				// �G�t�F�N�g�̐F3
#define EFFECT_COLOR4	D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f)				// �G�t�F�N�g�̐F4
#define EFFECT_COLOR_ENEMY1		D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)		// �G�t�F�N�g�̐F1
#define EFFECT_COLOR_ENEMY2		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)		// �G�t�F�N�g�̐F2
#define EFFECT_COLOR_ENEMY3		D3DXCOLOR(0.0f, 0.0f, 0.05f, 1.0f)		// �G�t�F�N�g�̐F3
#define EFFECT_COLOR_ENEMY4		D3DXCOLOR(0.0f, 0.1f, 0.0f, 1.0f)		// �G�t�F�N�g�̐F4
#define PARTICLE_SIZE(x)		(x * (0.1f * (float)(rand() % 9)))		// �p�[�e�B�N���̃T�C�Y
#define PARTICLE_LIFE	(100)											// �p�[�e�B�N���̎���
#define PARTICLE_COLOR	D3DXCOLOR(0.75f, 0.5f, 0.25f, 1.0f)				// �p�[�e�B�N���̐F
#define PARTICLE_MOVE	D3DXVECTOR3(0.0f, 0.0f, 5.0f)					// �p�[�e�B�N���̈ړ���(z)
#define HOMING_NONE		(-1)											// �z�[�~���O�e�œG��ǔ����Ȃ�
#define HOMING_COUNT	(0)												// �z�[�~���O�e�̏�������̃J�E���g�b��
#define HOMING_MOVE		(3.0f)											// �z�[�~���O�c�̏��������̉�����
#define HOMING_TIME		(150)											// �z�[�~���O�e�̒ǔ���������

#define NORMAL_DAMAGE	(1)												// �ʏ�e�̈ꔭ�̃_���[�W
#define HOMING_DAMAGE	(NORMAL_DAMAGE * 8)								// �z�[�~���O�e�̈ꔭ�̃_���[�W
#define LASER_DAMAGE	(NORMAL_DAMAGE)									// ���[�U�[�̈ꔭ�̃_���[�W		
#define BOMB_DAMAGE		(NORMAL_DAMAGE * 10)							// �{���̒����_���[�W
#define BOMBBULLET_DAMAGE		(NORMAL_DAMAGE * 5)						// �{���̊e�U�e�_���[�W

#define MAX_BOMB_BULLET	(8)												// �{���̔������A��������e�̐�
#define BOMB_BULLET_SPD	(8.0f)											// ��������e�̑��x
#define BOMB_BULLET_LIFE	(300)										// ��������e�̎���	

#define LASER_BULLET_SPD	(-16.0f)									// ��������e�̑��x
#define LASER_BULLET_LIFE	(300)										// ��������e�̎���	

// �v���g�^�C�v�錾
void CollisionEnemy(BULLET *pBullet);
void CollisionPlayer(BULLET *pBullet);

LPDIRECT3DTEXTURE9		g_pTextureBullet = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;	// ���_�o�b�t�@�̃|�C���^
BULLET g_aBullet[MAX_BULLET];						// �e�̏��
int g_nCounterBullet;								// �ėp�J�E���^�[
float g_nBulletSPD[MAX_BULLET];						// �e��

//================================================================================================================
// �e�̏�����
//================================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	int nCntBullet;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Bullet.png",
		&g_pTextureBullet);

	BULLET*pBullet = &g_aBullet[0];		// �e�̐擪�A�h���X����

	// �e�̏��̏�����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		pBullet->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBullet->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBullet->nLife = 0;
		pBullet->type = BULLETTYPE_PLAYER;
		pBullet->shot = SHOTTYPE_NORMAL;
		pBullet->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pBullet->fLength = sqrtf(PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE + PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE) * 0.5f;
		pBullet->fAngle = atan2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE);
 		pBullet->nEnemy = HOMING_NONE;
		pBullet->nCounterHoming = 0;
		pBullet->nCounterHomingtime = 0;
		pBullet->bEffect = false;
		pBullet->bUse = false;							// ���g�p��Ԃɐݒ�
		g_nBulletSPD[nCntBullet] = 0;
	}

	g_nCounterBullet = NULL;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,			// MAX_BULLET���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	pBullet = &g_aBullet[0];		// �e�̐擪�A�h���X����(���Z�b�g)

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pBullet->pos.x + sinf(D3DX_PI + pBullet->fAngle) * pBullet->fLength;
		pVtx[0].pos.y = pBullet->pos.y + cosf(D3DX_PI + pBullet->fAngle) * pBullet->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pBullet->pos.x + sinf(D3DX_PI - pBullet->fAngle) * pBullet->fLength;
		pVtx[1].pos.y = pBullet->pos.y + cosf(D3DX_PI - pBullet->fAngle) * pBullet->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pBullet->pos.x + sinf(-pBullet->fAngle) * pBullet->fLength;
		pVtx[2].pos.y = pBullet->pos.y + cosf(-pBullet->fAngle) * pBullet->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pBullet->pos.x + sinf(pBullet->fAngle) * pBullet->fLength;
		pVtx[3].pos.y = pBullet->pos.y + cosf(pBullet->fAngle) * pBullet->fLength;
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
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	g_pVtxBuffBullet->Unlock();
}

//================================================================================================================
// �e�̏I������
//================================================================================================================
void UninitBullet(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//================================================================================================================
// �e�̍X�V����
//================================================================================================================
void UpdateBullet(void)
{
	VERTEX_2D* pVtx;

	BULLET*pBullet = &g_aBullet[0];			// �e�̐擪�A�h���X����

	PLAYER *pPlayer = GetPlayer();				// �v���C���[�̃A�h���X����

	D3DXVECTOR3 pos = pPlayer->moveposPlayer;	// �v���C���[�̈ړ��ʂ���

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		if (pBullet->bUse == true)
		{
			ENEMY *pEnemy = GetEnemy();

			switch (pBullet->type)
			{
			case BULLETTYPE_PLAYER:

				if (pBullet->shot == SHOTTYPE_HOMING
					&& pEnemy[pBullet->nEnemy].bUse == true)
				{
					pBullet->nCounterHoming++;
					if (pBullet->nCounterHoming < HOMING_COUNT)
					{
						switch (pPlayer->rotPlayer)
						{
						case PLAYERDIRECTION_UP:
							// �����
							pBullet->pos.y -= HOMING_MOVE;

							break;

						case PLAYERDIRECTION_DOWN:
							// ������
							pBullet->pos.y += HOMING_MOVE;

							break;

						case PLAYERDIRECTION_LEFT:
							// ������
							pBullet->pos.x += HOMING_MOVE;

							break;

						case PLAYERDIRECTION_RIGHT:
							// �E����
							pBullet->pos.x -= HOMING_MOVE;

							break;
						}
					}
					else
					{
						float fLength = atan2f((pBullet->pos.x - pEnemy[pBullet->nEnemy].pos.x), (pBullet->pos.y - pEnemy[pBullet->nEnemy].pos.y));
						pBullet->pos.x += sinf(fLength * 1.f) * (pBullet->move.y * 1.f);
						pBullet->pos.y += cosf(fLength * 1.f) * (pBullet->move.y * 1.0f);
					}
				}
				else
				{
					pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
					pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;
				}

				if (pBullet->bEffect == true)
				{
					if (pBullet->shot != SHOTTYPE_LASERBULLET)
					{
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, pBullet->col, PLAYER_BULLET_SIZE * 1.25f, 40, false);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, EFFECT_COLOR2, PLAYER_BULLET_SIZE, 40, false);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, EFFECT_COLOR3, PLAYER_BULLET_SIZE * 2.25f, 50, false);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, pBullet->col, PLAYER_BULLET_SIZE * 2.5f, 50, false);

						SetParticle(pBullet->pos,									// �p�[�e�B�N���̐ݒu
							PARTICLE_COLOR,
							PARTICLE_MOVE,
							PARTICLE_SIZE(PLAYER_BULLET_SIZE),
							PARTICLE_LIFE,
							D3DX_PI,
							-D3DX_PI,
							true);
					}
					else
					{
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), PLAYER_BULLET_SIZE * 1.25f, 40, true);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), PLAYER_BULLET_SIZE * 1.75f, 40, true);
						SetEffect(pBullet->pos, D3DXVECTOR3_NULL, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), PLAYER_BULLET_SIZE * 1.5f, 40, true);
					}
				}

				break;

			case BULLETTYPE_ENEMY_1:

				pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
				pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;

				if (pBullet->shot == SHOTTYPE_HOMING)
				{
					if (pBullet->nLife % 75 >= 60 && pBullet->nLife % 75 <= 74)
					{
						pBullet->move.y = -6.5f;
					}
					else
					{
						pBullet->move.y = g_nBulletSPD[nCntBullet];
					}

					if (pBullet->nCounterHomingtime > 0)
					{
						float fRotMove,fRotDest,fRotDiff;
						fRotMove = pBullet->move.z;
						fRotDest = atan2f(pBullet->pos.x - pPlayer->posPlayer.x, pBullet->pos.y - pPlayer->posPlayer.y);
						fRotDiff = fRotDest - fRotMove;

						if (fRotDiff >= 3.14)
						{
							fRotDiff -= 6.28;
						}
						else if (fRotDiff <= -3.14)
						{
							fRotDiff = 6.28;
						}

						fRotMove += fRotDiff * 0.5f;

						if (fRotMove >= 3.14)
						{
							fRotMove -= 6.28;
						}
						else if (fRotMove <= -3.14)
						{
							fRotMove = 6.28;
						}

						pBullet->pos.x += sinf(fRotMove) * pBullet->move.y;
						pBullet->pos.y += cosf(fRotMove) * pBullet->move.y;
						pBullet->nCounterHomingtime--;

						pBullet->move.z = fRotMove;
					}
				}

				if (pBullet->bEffect == true)
				{
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,EFFECT_COLOR_ENEMY1, ENEMY_BULLET_SIZE * 1.25f, 40, false);
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,EFFECT_COLOR_ENEMY2, ENEMY_BULLET_SIZE, 40, false);
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,EFFECT_COLOR_ENEMY3, ENEMY_BULLET_SIZE * 2.5f, 50, false);
					SetEffect(pBullet->pos, D3DXVECTOR3_NULL,pBullet->col, ENEMY_BULLET_SIZE * 2.5f, 50, false);
				}

				break;

			default:

				break;
			}
		
			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pBullet->pos.x + pos.x) + sinf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.y = (pBullet->pos.y + pos.y) + cosf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.z = 0.0f;

			if (pBullet->shot == SHOTTYPE_FULLBURST)
			{
				SetBullet(pBullet->pos,
					0.0f,
					pBullet->move.z,
					LASER_LIFE,
					pBullet->type,
					SHOTTYPE_LASER,
					pBullet->col,
					true);

				SetBullet(pBullet->pos,
					BOMB_SPD,
					pBullet->move.z,
					1,
					pBullet->type,
					SHOTTYPE_BOMB,
					pBullet->col,
					true);

				SetBullet(pBullet->pos,
					HOMING_SPD,
					pBullet->move.z,
					HOMING_LIFE,
					pBullet->type,
					SHOTTYPE_HOMING,
					pBullet->col,
					true);

				SetBullet(pBullet->pos,
					HOMING_SPD,
					-pBullet->move.z,
					HOMING_LIFE,
					pBullet->type,
					SHOTTYPE_HOMING,
					pBullet->col,
					true);
			}

			switch (pBullet->type)
			{
			case BULLETTYPE_PLAYER:
				// �v���C���[�̒e�̏ꍇ
				CollisionEnemy(pBullet);
				break;

			case BULLETTYPE_ENEMY_1:
				// �G�̒e�̏ꍇ
				PLAYER *pPlayer = GetPlayer();

				if (pPlayer->state == PLAYERSTATE_NORMAL
					|| pPlayer->state == PLAYERSTATE_DAMAGE
					|| pPlayer->state == PLAYERSTATE_UNMOVE
					|| pPlayer->state == PLAYERSTATE_BARRIER)
				{// �v���C���[�������Ă����
					CollisionPlayer(pBullet);
				}

				break;
			}

			if (pBullet->pos.x < STAGE_LEFT || pBullet->pos.x > STAGE_RIGHT ||
				pBullet->pos.y < STAGE_CEILING || pBullet->pos.y > STAGE_FLOOR)
			{// ��ʊO�֏o���ꍇ
				pBullet->bUse = false;			// ���g�p��Ԃ֕ύX

				if (pBullet->shot == SHOTTYPE_BOMB)
				{
					for (int nCntBomb = 0; nCntBomb < MAX_BOMB_BULLET; nCntBomb++)
					{
						SetBullet(pBullet->pos,
							BOMB_BULLET_SPD, D3DX_PI - (((D3DX_PI * 2.0f) / MAX_BOMB_BULLET) * nCntBomb),
							BOMB_BULLET_LIFE,
							pBullet->type,
							SHOTTYPE_BOMBBULLET,
							D3DXCOLOR_NULL,
							true);
					}
				}
			}

			pBullet->nLife -= 1;
			if (pBullet->nLife <= 0)
			{// �e���ˌ��莞�Ԍo�߂�����
				pBullet->bUse = false;			// ���g�p��Ԃ֕ύX
				
				if (pBullet->shot == SHOTTYPE_BOMB)
				{
					for (int nCntBomb = 0; nCntBomb < MAX_BOMB_BULLET; nCntBomb++)
					{
						SetBullet(pBullet->pos,
							BOMB_BULLET_SPD, D3DX_PI - (((D3DX_PI * 2.0f) / MAX_BOMB_BULLET) * nCntBomb),
							BOMB_BULLET_LIFE,
							pBullet->type,
							SHOTTYPE_BOMBBULLET,
							pBullet->col,
							true);
					}
				}
			}
			else
			{
				if(pBullet->shot == SHOTTYPE_LASER && g_nCounterBullet % 4 == 0)
				SetBullet(pBullet->pos,
					LASER_BULLET_SPD,
					pBullet->move.z,
					LASER_BULLET_LIFE,
					pBullet->type,
					SHOTTYPE_LASERBULLET,
					pBullet->col,
					true);
			}
		}

		pVtx += 4;
	}

	g_pVtxBuffBullet->Unlock();

	g_nCounterBullet++;
}

//================================================================================================================
// �e�̕`�揈��
//================================================================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^,�f�o�C�X�̎擾
	int nCntBullet;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntBullet,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// �e�̐ݒ菈��
//================================================================================================================
void SetBullet(D3DXVECTOR3 pos, float speed, float rot, int nLife, BULLETTYPE type, SHOTTYPE shot, D3DXCOLOR col, bool bEffect)
{
	int nCntBullet;
	int nCntMax = 5;
	VERTEX_2D *pVtx;
	BULLET *pBullet = &g_aBullet[0];			// �e�̐擪�A�h���X����
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = pPlayer->moveposPlayer;

	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++,pBullet++)
	{
		if (pBullet->bUse == false)
		{
			pBullet->pos = pos;				// �e�̈ʒu����
			pBullet->move.y = speed;		// �e�̑��x����
			g_nBulletSPD[nCntBullet] = speed;
			pBullet->move.z = rot;			// �e�̌�������
			pBullet->nLife = nLife;			// �e�̎�������
			pBullet->type = type;			// �e�̎�ނ���
			if (type == BULLETTYPE_PLAYER)
			{
				pBullet->fLength = sqrtf(PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE + PLAYER_BULLET_SIZE * PLAYER_BULLET_SIZE) * 0.5f;
				pBullet->fAngle = atan2f(PLAYER_BULLET_SIZE, PLAYER_BULLET_SIZE);
			}
			else if (type == BULLETTYPE_ENEMY_1)
			{
				pBullet->fLength = sqrtf(ENEMY_BULLET_SIZE * ENEMY_BULLET_SIZE + ENEMY_BULLET_SIZE * ENEMY_BULLET_SIZE) * 0.5f;
				pBullet->fAngle = atan2f(ENEMY_BULLET_SIZE, ENEMY_BULLET_SIZE);
				pBullet->move.y = speed * 0.4f;		// �e�̑��x����
				g_nBulletSPD[nCntBullet] = speed * 0.4f;
			}

			pBullet->col = col;				// �e�̐F����
			pBullet->nEnemy = HOMING_NONE;	// �z�[�~���O�Ώۂ����Z�b�g
			pBullet->nCounterHoming = NULL;	// �z�[�~���O�e�̓���J�E���g�����Z�b�g
			pBullet->bEffect = bEffect;		// �G�t�F�N�g�̎g�p���邩��ۑ�
			pBullet->shot = shot;			// �e��̎�ނ���
			if (pBullet->shot == SHOTTYPE_HOMING)
			{
				if (pBullet->type == BULLETTYPE_PLAYER)
				{
					ENEMY* pEnemy = GetEnemy();			// �G�̏����擾
					float fLength;						// �e�ƓG�Ƃ̋���
					float fLengthLast = 100000.0f;		// �ߋ��̓G�ƒe�Ƃ̋���(������)

					for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
					{
						if (pEnemy->bUse == true)
						{
							fLength = sqrtf(powf((pEnemy->pos.x - pBullet->pos.x), 2.0f) + powf((pEnemy->pos.y - pBullet->pos.y), 2.0f));
							if (fLength <= fLengthLast)					// ���Ɖߋ��̋������r
							{// �������̓G�Ƃ̋������ߋ��ɔ��肵���G�Ƃ̋����������������
								pBullet->nEnemy = nCntEnemy;			// ���̓G���z�[�~���O�ΏۂƂ��ĕۑ�����B
								fLengthLast = fLength;					// ���̓G�Ƃ̋�����ۑ�
							}
						}
					}
				}
				else if (pBullet->type == BULLETTYPE_ENEMY_1)
				{
					pBullet->nCounterHomingtime = HOMING_TIME;
				}
			}

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z + (D3DX_PI + pBullet->fAngle)) * pBullet->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z + (D3DX_PI - pBullet->fAngle)) * pBullet->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pBullet->pos.x + Camerapos.x) + sinf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.y = (pBullet->pos.y + Camerapos.y) + cosf(pBullet->move.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.z = 0.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pBullet->col;
			pVtx[1].col = pBullet->col;
			pVtx[2].col = pBullet->col;
			pVtx[3].col = pBullet->col;

			pBullet->bUse = true;				// �e���g�p�ς݂ɂ���

			break;
		}

		pVtx += 4;
	}
	
	g_pVtxBuffBullet->Unlock();
}

//================================================================================================================
// �e�̃A�h���X�̎擾
//================================================================================================================
BULLET* GetBullet(void)
{
	return &g_aBullet[0];
}

//================================================================================================================
// �G�Ƃ̓����蔻��
//================================================================================================================
void CollisionEnemy(BULLET *pBullet)
{
	ENEMY *pEnemy = GetEnemy();			// �G�̏��̃|�C���^

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// �G���g���Ă����

			if (pBullet->pos.x > pEnemy->pos.x - (pEnemy->size.x * 0.5f) - (PLAYER_BULLET_SIZE * 0.5f)
				&& pBullet->pos.x < pEnemy->pos.x + (pEnemy->size.x * 0.5f) + (PLAYER_BULLET_SIZE * 0.5f)
				&& pBullet->pos.y > pEnemy->pos.y - (pEnemy->size.y * 0.5f) - (PLAYER_BULLET_SIZE * 0.5f)
				&& pBullet->pos.y < pEnemy->pos.y + (pEnemy->size.y * 0.5f) + (PLAYER_BULLET_SIZE * 0.5f)
				&& (pBullet->shot != SHOTTYPE_LASER && pBullet->shot != SHOTTYPE_BOMB))
			{
				switch (pBullet->shot)
				{
				case SHOTTYPE_HOMING:

					HitEnemy(nCntEnemy, HOMING_DAMAGE);

					break;

				case SHOTTYPE_NORMAL:

					HitEnemy(nCntEnemy, NORMAL_DAMAGE);

					break;

				case SHOTTYPE_LASERBULLET:

					HitEnemy(nCntEnemy, LASER_DAMAGE);

					break;

				case SHOTTYPE_BOMBBULLET:

					HitEnemy(nCntEnemy, BOMBBULLET_DAMAGE);

					break;

				}

				if (pBullet->shot != SHOTTYPE_LASERBULLET)
				pBullet->bUse = false;
			}
			
		}
	}
}

//================================================================================================================
// �v���C���[�Ƃ̓����蔻��
//================================================================================================================
void CollisionPlayer(BULLET* pBullet)
{
	PLAYER *pPlayer = GetPlayer();			// �v���C���[�̏��̃|�C���^
	float fSpeed;							// �e�̑��x
	float fAngle = GetPlayerRot(pPlayer);

	if (pBullet->pos.x > pPlayer->posPlayer.x - (PLAYER_SIZE * 0.225f) - (ENEMY_BULLET_SIZE * 0.5f)
		&& pBullet->pos.x < pPlayer->posPlayer.x + (PLAYER_SIZE * 0.225f) + (ENEMY_BULLET_SIZE * 0.5f)
		&& pBullet->pos.y > pPlayer->posPlayer.y - (PLAYER_SIZE * 0.35f) - (ENEMY_BULLET_SIZE * 0.5f)
		&& pBullet->pos.y < pPlayer->posPlayer.y + (PLAYER_SIZE * 0.35f) + (ENEMY_BULLET_SIZE * 0.5f)
		&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_BARRIER))
	{
		if (pPlayer->state == PLAYERSTATE_NORMAL)
		{
			HitPlayer(1);

			pBullet->bUse = false;
		}
		else
		{
			if (GetKeyboardWASD()
				|| GetJoypadWASD()
				|| GetJoyThumbWASD())
			{
				pBullet->move.z = fAngle;
				pBullet->move.y = -7.0f;
			}
			else
			{
				pBullet->move.z += D3DX_PI;
				pBullet->move.y = 7.0f;
			}
			
			pBullet->pos.x += sinf(pBullet->move.z) * pBullet->move.y;
			pBullet->pos.y += cosf(pBullet->move.z) * pBullet->move.y;
			pBullet->type = BULLETTYPE_PLAYER;
			pBullet->col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			PlaySound(SOUND_LABEL_SE_MIRROR);
		}
	}
}

//================================================================================================================
// �G�̒e��S�������{���̏���(bullet�̃��Z�b�g)
//================================================================================================================
void BombBullet(void)
{
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].bUse = false;
	}
}

int GetBulletAll(void)
{
	int n = 0;

	for (int nCntbullet = 0; nCntbullet < MAX_BULLET; nCntbullet++)
	{
		if (g_aBullet[nCntbullet].bUse) n++;
	}

	return n;
}