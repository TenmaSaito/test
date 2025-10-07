//================================================================================================================
//
// DirectX�̃u���b�N�t�@�C�� [block.cpp]
// Author : TENMA
//
//================================================================================================================
#include "block.h"
#include "sound.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "fade_stage.h"
#include "score.h"
#include "effect.h"
#include "input.h"
#include "particle.h"
#include "heart.h"
#include "timer.h"
#include "caution.h"

// �}�N����`
#define MAX_BLOCK		(128)			// �u���b�N�̍ő吔

// �v���g�^�C�v�錾
void CollisionPlayer(BLOCK *pBlock);
void CollisionEnemy(BLOCK *Block);
void CollisionBullet(BLOCK *pBlock);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureBlock = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;	// ���_�o�b�t�@�̃|�C���^
BLOCK g_aBlock[MAX_BLOCK];						// �u���b�N�\����
int g_nAlpha;									// ���n�ϓ��l�ϐ�
float g_fAngle;

//================================================================================================================
// �u���b�N�̏���������
//================================================================================================================
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	BLOCK *pBlock = &g_aBlock[0];
	int nCntBlock;

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		pBlock->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pBlock->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pBlock->type = BLOCKTYPE_WALL;
		pBlock->fLength  = sqrtf(powf((BLOCK_WIDTH * 2), 2) + powf((BLOCK_HEIGHT * 2), 2)) * 0.5f;
		pBlock->fAngle = atan2f((BLOCK_WIDTH * 2), (BLOCK_HEIGHT * 2));
		pBlock->fLaunchAngle = 0.0f;
		pBlock->fWidth = (float)BLOCK_WIDTH;
		pBlock->fHeight = (float)BLOCK_HEIGHT;
		pBlock->nCounter = 0;
		pBlock->bUse = false;
	}

	g_nAlpha = 1;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\BLOCK.png",
							&g_pTextureBlock);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx = NULL;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	pBlock = &g_aBlock[0];

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pBlock->pos.x + sinf(D3DX_PI + pBlock->fAngle) * pBlock->fLength;
		pVtx[0].pos.y = pBlock->pos.y + cosf(D3DX_PI + pBlock->fAngle) * pBlock->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pBlock->pos.x + sinf(D3DX_PI - pBlock->fAngle) * pBlock->fLength;
		pVtx[1].pos.y = pBlock->pos.y + cosf(D3DX_PI - pBlock->fAngle) * pBlock->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pBlock->pos.x + sinf(-pBlock->fAngle) * pBlock->fLength;
		pVtx[2].pos.y = pBlock->pos.y + cosf(-pBlock->fAngle) * pBlock->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pBlock->pos.x + sinf(pBlock->fAngle) * pBlock->fLength;
		pVtx[3].pos.y = pBlock->pos.y + cosf(pBlock->fAngle) * pBlock->fLength;
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
		pVtx[0].tex = D3DXVECTOR2(0.0909f * pBlock->type, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.0909f * pBlock->type + 0.0909f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0909f * pBlock->type, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.0909f * pBlock->type + 0.0909f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// �u���b�N�̏I������
//================================================================================================================
void UninitBlock(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureBlock != NULL)
	{
		g_pTextureBlock->Release();
		g_pTextureBlock = NULL;
	}
	

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//================================================================================================================
// �u���b�N�̍X�V����
//================================================================================================================
void UpdateBlock(void)
{
	VERTEX_2D *pVtx = NULL;
	BLOCK *pBlock = &g_aBlock[0];
	PLAYER *pPlayer = GetPlayer();
	int nTurnExac = GetTurn();
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			if (pBlock->type == BLOCKTYPE_BATTERY)
			{
				pBlock->nCounter++;
				if ((pBlock->nCounter % 36) == 0)
				{
					pBlock->fLaunchAngle += D3DX_PI * 0.15f;

					SetBullet(pBlock->pos,
						2.0f,
						pBlock->fLaunchAngle,
						1000,
						BULLETTYPE_ENEMY_1,
						SHOTTYPE_NORMAL,
						D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f),
						false);
				}
			}
			else if (pBlock->type >= BLOCKTYPE_EXIT_A && pBlock->type <= BLOCKTYPE_EXIT_G)
			{
				if (nTurnExac != ALREADY_CLEARED)
				{
					pBlock->col.a = 0.0f;
				}
				else
				{
					if (pBlock->col.a == 0.0f)
					{
						pBlock->col.a = 0.2f;
					}

					pBlock->col.a += 0.01f * g_nAlpha;
					if (pBlock->col.a >= 1.0f || pBlock->col.a <= 0.1f)
					{
						g_nAlpha *= -1;
					}
				}
			}

			// �����蔻��
			CollisionPlayer(pBlock);
			CollisionEnemy(pBlock);
			CollisionBullet(pBlock);

			if (nCntBlock != pPlayer->g_nIDDirection)
			{
				pos = pPlayer->moveposPlayer;
			}

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pBlock->pos.x + pos.x) + sinf(D3DX_PI + pBlock->fAngle) * pBlock->fLength;
			pVtx[0].pos.y = (pBlock->pos.y + pos.y) + cosf(D3DX_PI + pBlock->fAngle) * pBlock->fLength;
			pVtx[0].pos.z = 0.0f;		  
										  
			pVtx[1].pos.x = (pBlock->pos.x + pos.x) + sinf(D3DX_PI - pBlock->fAngle) * pBlock->fLength;
			pVtx[1].pos.y = (pBlock->pos.y + pos.y) + cosf(D3DX_PI - pBlock->fAngle) * pBlock->fLength;
			pVtx[1].pos.z = 0.0f;		  
										  
			pVtx[2].pos.x = (pBlock->pos.x + pos.x) + sinf(-pBlock->fAngle) * pBlock->fLength;
			pVtx[2].pos.y = (pBlock->pos.y + pos.y) + cosf(-pBlock->fAngle) * pBlock->fLength;
			pVtx[2].pos.z = 0.0f;		  
										  
			pVtx[3].pos.x = (pBlock->pos.x + pos.x) + sinf(pBlock->fAngle) * pBlock->fLength;
			pVtx[3].pos.y = (pBlock->pos.y + pos.y) + cosf(pBlock->fAngle) * pBlock->fLength;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pBlock->col;
			pVtx[1].col = pBlock->col;
			pVtx[2].col = pBlock->col;
			pVtx[3].col = pBlock->col;

			pVtx[0].tex = D3DXVECTOR2(0.1f * pBlock->type, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f * pBlock->type + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.1f * pBlock->type, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f * pBlock->type + 0.1f, 1.0f);
		}

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
	
}

//================================================================================================================
// �u���b�N�̕`�揈��
//================================================================================================================
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	BLOCK *pBlock = &g_aBlock[0];
	HWND hWnd = NULL;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == true)
		{
			// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
			pDevice->SetTexture(0, g_pTextureBlock);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				nCntBlock * 4,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// �u���b�N�̓ǂݍ��ݏ���
//================================================================================================================
void LoadBlock(char* pFileName)
{
	char aStr[256];
	char aTrash[3];
	FILE *pFile;
	D3DXVECTOR3 pos;
	BLOCKTYPE type = BLOCKTYPE_WALL;
	HWND hWnd = GetActiveWindow();

	pFile = fopen(pFileName, "r");
	if (pFile != NULL)
	{
		(void)fscanf(pFile, "%s", &aStr[0]);
		if (strcmp(&aStr[0], "START_SCRIPT") == 0)
		{
			while (1)
			{
				(void)fscanf(pFile, "%s", &aStr[0]);
				if (strcmp(&aStr[0], "START_SETBLOCK") == 0)
				{
					while (1)
					{
						(void)fscanf(pFile, "%s", &aStr[0]);
						if (strcmp(&aStr[0], "POS") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
						}
						else if (strcmp(&aStr[0], "TYPE") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%d", &type);
						}

						if (strcmp(&aStr[0], "END_SETBLOCK") == 0)
						{
							SetBlock(type, pos,BLOCK_WIDTH,BLOCK_HEIGHT);
							break;
						}
					}
				}
				
				if (strcmp(&aStr[0], "END_SCRIPT") == 0)
				{
					break;
				}
			}
		}

		fclose(pFile);
		
	}
	else
	{
		MessageBox(hWnd, "�t�@�C���f�[�^�̓ǂݍ��݂Ɏ��s�I(BLOCK)", "�x���I", MB_ICONWARNING);
	}
}

//================================================================================================================
// �u���b�N�̐ݒu
//================================================================================================================
int SetBlock(BLOCKTYPE type, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	BLOCK* pBlock = &g_aBlock[0];
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos;
	VERTEX_2D* pVtx;
	HWND hWnd = NULL;
	int nCntBlock;

	if (type < BLOCKTYPE_WALL || type >= BLOCKTYPE_MAX)
	{
		if (SUCCEEDED(GetHandleWindow(&hWnd)))
		{ // �u���b�N�̎�ނ�������K��O�̒l�������ꍇ�A�����𒆒f
#ifdef  _DEBUG
			MessageBox(hWnd, "���o�C", "���H", MB_ICONWARNING);
#endif //  _DEBUG
			return 0;
		}
	}

	AddFunctionLog("START : Block Set");

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++,pBlock++)
	{
		if (pBlock->bUse == false)
		{
			Camerapos = pPlayer->moveposPlayer;

			pBlock->pos = pos;
			pBlock->type = type;
			pBlock->col = D3DXCOLOR_NULL;
			pBlock->nCounter = 0;

			pBlock->fWidth = fWidth;
			pBlock->fHeight = fHeight;

			// �Ίp���̒������擾
			pBlock->fLength = sqrtf(powf((fWidth * 2), 2) + powf((fHeight * 2), 2)) * 0.5f;

			// �Ίp���̊p�x���Z�o
			pBlock->fAngle = atan2f((BLOCK_WIDTH * 2), (BLOCK_HEIGHT * 2));
			
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntBlock;

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pBlock->pos.x + Camerapos.x) + sinf(D3DX_PI + pBlock->fAngle) * pBlock->fLength;
			pVtx[0].pos.y = (pBlock->pos.y + Camerapos.y) + cosf(D3DX_PI + pBlock->fAngle) * pBlock->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pBlock->pos.x + Camerapos.x) + sinf(D3DX_PI - pBlock->fAngle) * pBlock->fLength;
			pVtx[1].pos.y = (pBlock->pos.y + Camerapos.y) + cosf(D3DX_PI - pBlock->fAngle) * pBlock->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pBlock->pos.x + Camerapos.x) + sinf(-pBlock->fAngle) * pBlock->fLength;
			pVtx[2].pos.y = (pBlock->pos.y + Camerapos.y) + cosf(-pBlock->fAngle) * pBlock->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pBlock->pos.x + Camerapos.x) + sinf(pBlock->fAngle) * pBlock->fLength;
			pVtx[3].pos.y = (pBlock->pos.y + Camerapos.y) + cosf(pBlock->fAngle) * pBlock->fLength;
			pVtx[3].pos.z = 0.0f;

			pVtx[0].tex = D3DXVECTOR2(0.0909f * pBlock->type, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0909f * pBlock->type + 0.0909f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0909f * pBlock->type, 0.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0909f * pBlock->type + 0.0909f, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pBlock->col;
			pVtx[1].col = pBlock->col;
			pVtx[2].col = pBlock->col;
			pVtx[3].col = pBlock->col;

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffBlock->Unlock();

			pBlock->bUse = true;

			break;
		}
	}

	return nCntBlock;
}

//================================================================================================================
// �u���b�N�̎擾
//================================================================================================================
BLOCK *GetBlock(int nID)
{
	return &g_aBlock[nID];
}

//================================================================================================================
// �u���b�N�̏���(�w��)
//================================================================================================================
void DeleteBlock(int nID)
{
	BLOCK* pBlock = &g_aBlock[nID];

	if (pBlock->bUse == true)
	{
		pBlock->bUse = false;
	}
}

//================================================================================================================
// �u���b�N�̏���(�S��)
//================================================================================================================
void ResetBlock(void)
{
	BLOCK *pBlock = &g_aBlock[0];

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++, pBlock++)
	{
		if (pBlock->bUse == true)
		{
			pBlock->bUse = false;
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Player)
//================================================================================================================
void CollisionPlayer(BLOCK* pBlock)
{
	PLAYER* pPlayer = GetPlayer();
	STAGE stage = GetStage();
	int nCntEnemy = GetTotalEnemy();
	int nTurnExac = GetTurn();

	if (pPlayer->posPlayer.x >= pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.x <= pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.y >= pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f)
		&& pPlayer->posPlayer.y <= pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f))
	{
		switch (pBlock->type)
		{
		case BLOCKTYPE_WALL:
			// �v���C���[�ƃu���b�N�̊p�x�Ŕ���
			g_fAngle = atan2f(pBlock->pos.x - pPlayer->posPlayer.x, pBlock->pos.y - pPlayer->posPlayer.y);

			if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}

			break;

		case BLOCKTYPE_BATTERY:
			// �v���C���[�ƃu���b�N�̊p�x�Ŕ���
			g_fAngle = atan2f(pBlock->pos.x - pPlayer->posPlayer.x, pBlock->pos.y - pPlayer->posPlayer.y);

			if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}

			break;

		case BLOCKTYPE_EXIT_A:

			if (nCntEnemy <= 0
				&& nTurnExac == ALREADY_CLEARED
				&& GetFadeStage() == FADESTAGE_NONE
				&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE || pPlayer->state == PLAYERSTATE_BARRIER)
				&& GetTimer() > 0)
			{
				if (pPlayer->state == PLAYERSTATE_BARRIER)
				{
					SetCaution();
				}
				else
				{
					pPlayer->movePlayer.x = 0.0f;
					pPlayer->movePlayer.y = 0.0f;

					pPlayer->state = PLAYERSTATE_UNMOVE;

					if (stage == STAGE_GRASS)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_VOLCANO, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_VOLCANO);
					}
					else if (stage == STAGE_VOLCANO)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_GRASS, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_NORMAL);
					}
				}
			}

			break;

		case BLOCKTYPE_EXIT_B:

			if (nCntEnemy <= 0 
				&& nTurnExac == ALREADY_CLEARED 
				&& GetFadeStage() == FADESTAGE_NONE 
			    &&(pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE || pPlayer->state == PLAYERSTATE_BARRIER)
				&& GetTimer() > 0)
			{
				if (pPlayer->state == PLAYERSTATE_BARRIER)
				{
					SetCaution();
				}
				else
				{
					pPlayer->movePlayer.x = 0.0f;
					pPlayer->movePlayer.y = 0.0f;

					pPlayer->state = PLAYERSTATE_UNMOVE;

					if (stage == STAGE_GRASS)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_FOREST, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_FOREST);
					}
					else if (stage == STAGE_FOREST)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_GRASS, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_NORMAL);
					}
				}
			}

			break;

		case BLOCKTYPE_EXIT_C:

			if (nCntEnemy <= 0 
				&& nTurnExac == ALREADY_CLEARED 
				&& GetFadeStage() == FADESTAGE_NONE 
				&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE || pPlayer->state == PLAYERSTATE_BARRIER)
				&& GetTimer() > 0)
			{
				if (pPlayer->state == PLAYERSTATE_BARRIER)
				{
					SetCaution();
				}
				else
				{
					pPlayer->movePlayer.x = 0.0f;
					pPlayer->movePlayer.y = 0.0f;

					pPlayer->state = PLAYERSTATE_UNMOVE;

					if (stage == STAGE_GRASS)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_ICE, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_ICE);
					}
					else if (stage == STAGE_ICE)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_GRASS, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_NORMAL);
					}
				}
			}

			break;

		case BLOCKTYPE_EXIT_D:

			if (nCntEnemy <= 0 
				&& nTurnExac == ALREADY_CLEARED 
				&& GetFadeStage() == FADESTAGE_NONE 
				&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE || pPlayer->state == PLAYERSTATE_BARRIER)
				&& GetTimer() > 0)
			{

				if (pPlayer->state == PLAYERSTATE_BARRIER)
				{
					SetCaution();
				}
				else
				{
					pPlayer->movePlayer.x = 0.0f;
					pPlayer->movePlayer.y = 0.0f;

					pPlayer->state = PLAYERSTATE_UNMOVE;

					if (stage == STAGE_VOLCANO)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_DESERT, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_DESERT);
					}
					else if (stage == STAGE_DESERT)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_VOLCANO, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_VOLCANO);
					}
				}
			}

			break;

		case BLOCKTYPE_EXIT_E:

			if (nCntEnemy <= 0 
				&& nTurnExac == ALREADY_CLEARED 
				&& GetFadeStage() == FADESTAGE_NONE 
				&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE || pPlayer->state == PLAYERSTATE_BARRIER) 
				&& GetTimer() > 0)
			{
				if (pPlayer->state == PLAYERSTATE_BARRIER)
				{
					SetCaution();
				}
				else
				{

					pPlayer->movePlayer.x = 0.0f;
					pPlayer->movePlayer.y = 0.0f;

					pPlayer->state = PLAYERSTATE_UNMOVE;

					if (stage == STAGE_DESERT)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_FOREST, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_FOREST);
					}
					else if (stage == STAGE_FOREST)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_DESERT, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_DESERT);
					}
				}
			}

			break;

		case BLOCKTYPE_EXIT_F:

			if (nCntEnemy <= 0 
				&& nTurnExac == ALREADY_CLEARED 
				&& GetFadeStage() == FADESTAGE_NONE 
				&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE || pPlayer->state == PLAYERSTATE_BARRIER)
				&& GetTimer() > 0)
			{
				if (pPlayer->state == PLAYERSTATE_BARRIER)
				{
					SetCaution();
				}
				else
				{
					pPlayer->movePlayer.x = 0.0f;
					pPlayer->movePlayer.y = 0.0f;

					pPlayer->state = PLAYERSTATE_UNMOVE;

					if (stage == STAGE_FOREST)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_SEA, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_SEA);
					}
					else if (stage == STAGE_SEA)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_FOREST, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_FOREST);
					}
				}
			}

			break;

		case BLOCKTYPE_EXIT_G:

			if (nCntEnemy <= 0 
				&& nTurnExac == ALREADY_CLEARED 
				&& GetFadeStage() == FADESTAGE_NONE 
				&& (pPlayer->state == PLAYERSTATE_NORMAL || pPlayer->state == PLAYERSTATE_DAMAGE || pPlayer->state == PLAYERSTATE_BARRIER)
				&& GetTimer() > 0)
			{
				if (pPlayer->state == PLAYERSTATE_BARRIER)
				{
					SetCaution();
				}
				else
				{
					pPlayer->movePlayer.x = 0.0f;
					pPlayer->movePlayer.y = 0.0f;

					pPlayer->state = PLAYERSTATE_UNMOVE;

					if (stage == STAGE_SEA)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_ICE, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_ICE);
					}
					else if (stage == STAGE_ICE)
					{
						PlaySound(SOUND_LABEL_SE_TELEPORT);
						SetFadeStage(STAGE_SEA, FADESTAGE_OUT);
						if (GetTimer() <= NOMORE_TIME) break;
						FadeSound(SOUND_LABEL_GAME_SEA);
					}
				}
			}

			break;

		case BLOCKTYPE_EX_EXIT_1:

			// �v���C���[�ƃu���b�N�̊p�x�Ŕ���
			g_fAngle = atan2f(pBlock->pos.x - pPlayer->posPlayer.x, pBlock->pos.y - pPlayer->posPlayer.y);

			if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x + BLOCK_WIDTH + (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
			{
				pPlayer->posPlayer.y = pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_SIZE * 0.5f);
				pPlayer->movePlayer.y = 0.0f;
			}
			else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
			{
				pPlayer->posPlayer.x = pBlock->pos.x - BLOCK_WIDTH - (PLAYER_SIZE * 0.25f);
				pPlayer->movePlayer.x = 0.0f;
			}

			if (pPlayer->bHaveKey == true)
			{
				pBlock->bUse = false;
				// TODO : ������������̉���炷�I�I�I�I���I�I�I

				PlaySound(SOUND_LABEL_SE_OPEN);
				pPlayer->bHaveKey = false;
			}

			break;
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Enemy)
//================================================================================================================
void CollisionEnemy(BLOCK* pBlock)
{
	ENEMY *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pBlock->type == BLOCKTYPE_WALL || pBlock->type == BLOCKTYPE_BATTERY)
		{
			if (pEnemy->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f)
				&& pEnemy->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f))
			{
				// �v���C���[�ƃu���b�N�̊p�x�Ŕ���
				g_fAngle = atan2f(pBlock->pos.x - pEnemy->pos.x, pBlock->pos.y - pEnemy->pos.y);

				if (g_fAngle > (D3DX_PI * -0.25f) && g_fAngle <= (D3DX_PI * 0.25f))
				{
					pEnemy->pos.y = pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * -0.75f) && g_fAngle <= (D3DX_PI * -0.25f))
				{
					pEnemy->pos.x = pBlock->pos.x + BLOCK_WIDTH + (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * 0.75f) || g_fAngle <= (D3DX_PI * -0.75f))
				{
					pEnemy->pos.y = pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_SIZE * 0.5f);
				}
				else if (g_fAngle > (D3DX_PI * 0.25f) && g_fAngle <= (D3DX_PI * 0.75f))
				{
					pEnemy->pos.x = pBlock->pos.x - BLOCK_WIDTH - (ENEMY_SIZE * 0.5f);
				}
			}
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Bullet)
//================================================================================================================
void CollisionBullet(BLOCK* pBlock)
{
	BULLET *pBullet = GetBullet();

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true && pBlock->type == BLOCKTYPE_WALL)
		{
			if (pBullet->type == BULLETTYPE_PLAYER)
			{
				if (pBullet->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (PLAYER_BULLET_SIZE * 0.5f)
					&& pBullet->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (PLAYER_BULLET_SIZE * 0.5f)
					&& pBullet->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (PLAYER_BULLET_SIZE * 0.5f)
					&& pBullet->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (PLAYER_BULLET_SIZE * 0.5f))
				{
					if (pBullet->type == BULLETTYPE_PLAYER)
					{
						SetParticle(pBullet->pos, pBullet->col, 10, D3DX_PI, -D3DX_PI, 5);
					}

					pBullet->bUse = false;
				}
			}
			else if (pBullet->type == BULLETTYPE_ENEMY_1)
			{
				if (pBullet->pos.x >= pBlock->pos.x - BLOCK_WIDTH - (ENEMY_BULLET_SIZE * 0.5f)
					&& pBullet->pos.x <= pBlock->pos.x + BLOCK_WIDTH + (ENEMY_BULLET_SIZE * 0.5f)
					&& pBullet->pos.y >= pBlock->pos.y - BLOCK_HEIGHT - (ENEMY_BULLET_SIZE * 0.5f)
					&& pBullet->pos.y <= pBlock->pos.y + BLOCK_HEIGHT + (ENEMY_BULLET_SIZE * 0.5f))
				{
					if (pBullet->type == BULLETTYPE_PLAYER)
					{
						SetParticle(pBullet->pos, pBullet->col, 10, D3DX_PI, -D3DX_PI, 5);
					}

					pBullet->bUse = false;
				}
			}
		}
	}
}

