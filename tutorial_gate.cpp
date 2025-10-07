//================================================================================================================
//
// DirectX�̃u���b�N�t�@�C�� [block.cpp]
// Author : TENMA
//
//================================================================================================================
#include "tutorial_gate.h"
#include "game.h"
#include "sound.h"
#include "player.h"
#include "fade.h"
#include "showDifficulty.h"

// �}�N����`
#define MAX_GATE		(10)			// �Q�[�g�̍ő吔

// �v���g�^�C�v�錾
void CollisionPlayer(GATE *pGate);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureTutorialGate = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialGate = NULL;	// ���_�o�b�t�@�̃|�C���^
GATE g_aTutorialGate[MAX_GATE];							// �Q�[�g�\����
int g_nAlphaGate;										// ���n�ϓ��l�ϐ�

//================================================================================================================
// �u���b�N�̏���������
//================================================================================================================
void InitTutorialGate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	GATE *pTutorialGate = &g_aTutorialGate[0];
	int nCntTutorialGate;

	for (nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		pTutorialGate->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pTutorialGate->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pTutorialGate->size = POLY_SIZE(GATE_WIDTH, GATE_HEIGHT);
		pTutorialGate->difficulty = GAMEDIFFICULTY_EASY;
		pTutorialGate->type = GATETYPE_EXIT_EASY;
		pTutorialGate->bUse = false;
	}

	g_nAlphaGate = 1;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
							"data\\TEXTURE\\CHARACTER\\TITLE\\TITLE_MAGE.png",
							&g_pTextureTutorialGate);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_GATE,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialGate,
		NULL);

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx = NULL;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialGate->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	pTutorialGate = &g_aTutorialGate[0];

	for (nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pTutorialGate->pos.x - pTutorialGate->size.hx;
		pVtx[0].pos.y = pTutorialGate->pos.y - pTutorialGate->size.hy;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTutorialGate->pos.x + pTutorialGate->size.hx;
		pVtx[1].pos.y = pTutorialGate->pos.y - pTutorialGate->size.hy;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTutorialGate->pos.x - pTutorialGate->size.hx;
		pVtx[2].pos.y = pTutorialGate->pos.y + pTutorialGate->size.hy;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTutorialGate->pos.x + pTutorialGate->size.hx;
		pVtx[3].pos.y = pTutorialGate->pos.y + pTutorialGate->size.hy;
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialGate->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// �u���b�N�̏I������
//================================================================================================================
void UninitTutorialGate(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureTutorialGate != NULL)
	{
		g_pTextureTutorialGate->Release();
		g_pTextureTutorialGate = NULL;
	}
	

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffTutorialGate != NULL)
	{
		g_pVtxBuffTutorialGate->Release();
		g_pVtxBuffTutorialGate = NULL;
	}
}

//================================================================================================================
// �u���b�N�̍X�V����
//================================================================================================================
void UpdateTutorialGate(void)
{
	VERTEX_2D *pVtx = NULL;
	GATE *pTutorialGate = &g_aTutorialGate[0];
	PLAYER *pPlayer = GetPlayer();
	
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialGate->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++, pTutorialGate++)
	{
		if (pTutorialGate->bUse == true)
		{
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f,0.0f,0.0f);

			pTutorialGate->col.a += 0.01f * g_nAlphaGate;
			if (pTutorialGate->col.a >= 1.0f || pTutorialGate->col.a <= 0.1f)
			{
				g_nAlphaGate *= -1;
			}

			// �����蔻��
			CollisionPlayer(pTutorialGate);

			pos = pPlayer->moveposPlayer;

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pTutorialGate->pos.x + pos.x) - pTutorialGate->size.hx;
			pVtx[0].pos.y = (pTutorialGate->pos.y + pos.y) - pTutorialGate->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pTutorialGate->pos.x + pos.x) + pTutorialGate->size.hx;
			pVtx[1].pos.y = (pTutorialGate->pos.y + pos.y) - pTutorialGate->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pTutorialGate->pos.x + pos.x) - pTutorialGate->size.hx;
			pVtx[2].pos.y = (pTutorialGate->pos.y + pos.y) + pTutorialGate->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pTutorialGate->pos.x + pos.x) + pTutorialGate->size.hx;
			pVtx[3].pos.y = (pTutorialGate->pos.y + pos.y) + pTutorialGate->size.hy;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pTutorialGate->col;
			pVtx[1].col = pTutorialGate->col;
			pVtx[2].col = pTutorialGate->col;
			pVtx[3].col = pTutorialGate->col;

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialGate->Unlock();
}

//================================================================================================================
// �u���b�N�̕`�揈��
//================================================================================================================
void DrawTutorialGate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	GATE *pTutorialGate = &g_aTutorialGate[0];
	HWND hWnd = NULL;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialGate, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		if (pTutorialGate->bUse == true)
		{
			// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
			pDevice->SetTexture(0, g_pTextureTutorialGate);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				nCntTutorialGate * 4,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// �u���b�N�̓ǂݍ��ݏ���
//================================================================================================================
void LoadTutorialGate(char* pFileName)
{
	char aStr[256];
	char aTrash[3];
	FILE *pFile;
	D3DXVECTOR3 pos;
	HWND hWnd = GetActiveWindow();
	GATE lGate = {};

	pFile = fopen(pFileName, "r");
	if (pFile != NULL)
	{
		(void)fscanf(pFile, "%s", &aStr[0]);
		if (strcmp(&aStr[0], "START_SCRIPT") == 0)
		{
			while (1)
			{
				(void)fscanf(pFile, "%s", &aStr[0]);
				if (strcmp(&aStr[0], "START_SETGATE") == 0)
				{
					while (1)
					{
						(void)fscanf(pFile, "%s", &aStr[0]);
 						if (strcmp(&aStr[0], "POS") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%f %f %f", &lGate.pos.x, &lGate.pos.y, &lGate.pos.z);
						}
						else if (strcmp(&aStr[0], "TYPE") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%d", &lGate.type);
						}
						else if (strcmp(&aStr[0], "SIZE") == 0)
						{
							fread(&aTrash[0], 1, sizeof(aTrash), pFile);
							(void)fscanf(pFile, "%f %f", &lGate.size.x, &lGate.size.y);
							lGate.size = POLY_SIZE(lGate.size.x, lGate.size.y);
						}

						if (strcmp(&aStr[0], "END_SETGATE") == 0)
						{
							SetTutorialGate(lGate.type, lGate.pos, lGate.size);
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
		MessageBox(hWnd, "�t�@�C���f�[�^�̓ǂݍ��݂Ɏ��s�I(GATE)", "�x���I", MB_ICONWARNING);
	}
}

//================================================================================================================
// �u���b�N�̐ݒu
//================================================================================================================
int SetTutorialGate(GATETYPE type, D3DXVECTOR3 pos, POLY_SIZE size)
{
	GATE* pTutorialGate = &g_aTutorialGate[0];
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos;
	VERTEX_2D* pVtx;
	HWND hWnd = NULL;
	int nCntTutorialGate;

	if (type < GATETYPE_EXIT_EASY || type >= GATETYPE_MAX)
	{
		if (SUCCEEDED(GetHandleWindow(&hWnd)))
		{ // �u���b�N�̎�ނ�������K��O�̒l�������ꍇ�A�����𒆒f
#ifdef  _DEBUG
			MessageBox(hWnd, "���o�C", "���H", MB_ICONWARNING);
#endif //  _DEBUG
			return 0;
		}
	}

	AddFunctionLog("START : TutorialGate Set");

	for (nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++,pTutorialGate++)
	{
		if (pTutorialGate->bUse == false)
		{
			Camerapos = pPlayer->moveposPlayer;

			pTutorialGate->pos = pos;
			pTutorialGate->type = type;
			pTutorialGate->difficulty = (GAMEDIFFICULTY)type;
			pTutorialGate->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
			pTutorialGate->size = size;
			
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffTutorialGate->Lock(0, 0, (void**)&pVtx, 0);

			pVtx += 4 * nCntTutorialGate;

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pTutorialGate->pos.x + Camerapos.x) - pTutorialGate->size.hx;
			pVtx[0].pos.y = (pTutorialGate->pos.y + Camerapos.y) - pTutorialGate->size.hy;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pTutorialGate->pos.x + Camerapos.x) + pTutorialGate->size.hx;
			pVtx[1].pos.y = (pTutorialGate->pos.y + Camerapos.y) - pTutorialGate->size.hy;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pTutorialGate->pos.x + Camerapos.x) - pTutorialGate->size.hx;
			pVtx[2].pos.y = (pTutorialGate->pos.y + Camerapos.y) + pTutorialGate->size.hy;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pTutorialGate->pos.x + Camerapos.x) + pTutorialGate->size.hx;
			pVtx[3].pos.y = (pTutorialGate->pos.y + Camerapos.y) + pTutorialGate->size.hy;
			pVtx[3].pos.z = 0.0f;

			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.5f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.5f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pTutorialGate->col;
			pVtx[1].col = pTutorialGate->col;
			pVtx[2].col = pTutorialGate->col;
			pVtx[3].col = pTutorialGate->col;

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffTutorialGate->Unlock();

			pTutorialGate->bUse = true;

			SetShowDifficulty(D3DXVECTOR3(pTutorialGate->pos.x, pTutorialGate->pos.y + 70.0f, pTutorialGate->pos.z), pTutorialGate->difficulty);

			break;
		}
	}

	return nCntTutorialGate;
}

//================================================================================================================
// �u���b�N�̏���(�S��)
//================================================================================================================
void DestroyTutorialGate(void)
{
	GATE *pTutorialGate = &g_aTutorialGate[0];

	for (int nCntTutorialGate = 0; nCntTutorialGate < MAX_GATE; nCntTutorialGate++, pTutorialGate++)
	{
		if (pTutorialGate->bUse == true)
		{
			pTutorialGate->bUse = false;
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Player)
//================================================================================================================
void CollisionPlayer(GATE* pTutorialGate)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->posPlayer.x >= pTutorialGate->pos.x - pTutorialGate->size.hx - (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.x <= pTutorialGate->pos.x + pTutorialGate->size.hx + (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.y >= pTutorialGate->pos.y - pTutorialGate->size.hy - (PLAYER_SIZE * 0.5f)
		&& pPlayer->posPlayer.y <= pTutorialGate->pos.y + pTutorialGate->size.hy + (PLAYER_SIZE * 0.5f))
	{
		switch (pTutorialGate->type)
		{
		case GATETYPE_EXIT_EASY:

			pPlayer->movePlayer.x = 0.0f;
			pPlayer->movePlayer.y = 0.0f;

			pPlayer->state = PLAYERSTATE_UNMOVE;

			if (GetFade() == FADE_NONE)
			{
				SetGameTutorial(false);
				PlaySound(SOUND_LABEL_SE_TELEPORT);
				SetGameDifficulty(GAMEDIFFICULTY_EASY);
				SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
				FadeSound(SOUND_LABEL_GAME_NORMAL);
			}

			break;

		case GATETYPE_EXIT_NORMAL:

			pPlayer->movePlayer.x = 0.0f;
			pPlayer->movePlayer.y = 0.0f;

			pPlayer->state = PLAYERSTATE_UNMOVE;

			if (GetFade() == FADE_NONE)
			{
				SetGameTutorial(false);
				PlaySound(SOUND_LABEL_SE_TELEPORT);
				SetGameDifficulty(GAMEDIFFICULTY_NORMAL);
				SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
				FadeSound(SOUND_LABEL_GAME_NORMAL);
			}

			break;

		case GATETYPE_EXIT_HARD:

			pPlayer->movePlayer.x = 0.0f;
			pPlayer->movePlayer.y = 0.0f;

			pPlayer->state = PLAYERSTATE_UNMOVE;

			if (GetFade() == FADE_NONE)
			{
				SetGameTutorial(false);
				PlaySound(SOUND_LABEL_SE_TELEPORT);
				SetGameDifficulty(GAMEDIFFICULTY_HARD);
				SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
				FadeSound(SOUND_LABEL_GAME_NORMAL);
			}

			break;
		}
	}
}
