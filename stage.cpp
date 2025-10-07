//================================================================================================================
//
// DirectX�̃X�e�[�W���� [stage.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "stage.h"
#include "player.h"
#include "enemy.h"
#include "block.h"
#include "timer.h"
#include "editer.h"
#include "bullet.h"
#include "fade_stage.h"
#include "item.h"
#include "effect.h"

// �}�N����`
#define MAX_TURN			(3)			// �X�e�[�W�̍ő�^�[����
#define SET_EXIT_LEFT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_LEFT + BLOCK_WIDTH, (SCREEN_HEIGHT * 0.5f), 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))		// �X�e�[�W�̍����̏o��(x:BLOCKTYPE)
#define SET_EXIT_RIGHT(x)	(SetBlock(x, D3DXVECTOR3(STAGE_RIGHT - BLOCK_WIDTH, (SCREEN_HEIGHT * 0.5f), 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))		// �X�e�[�W�̉E���̏o��(x:BLOCKTYPE)
#define SET_EXIT_UPPER(x)	(SetBlock(x, D3DXVECTOR3((SCREEN_WIDTH * 0.5f), STAGE_CEILING + BLOCK_WIDTH, 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))	// �X�e�[�W�̏㑤�̏o��(x:BLOCKTYPE)
#define SET_EXIT_UNDER(x)	(SetBlock(x, D3DXVECTOR3((SCREEN_WIDTH * 0.5f), STAGE_FLOOR - BLOCK_WIDTH, 0.0f),BLOCK_WIDTH,BLOCK_HEIGHT))	// �X�e�[�W�̉����̏o��(x:BLOCKTYPE)
#define TELEPORT_COOLTIME	(180)		// �X�e�[�W�ړ���̖��G+�X�e�[�W�ړ��K������

// �X�e�[�W�\����
typedef struct
{
	STAGE stage;		// �X�e�[�W
	int nTurn;			// �o�߃^�[��
	bool bUse;			// �g���Ă��邩
}STAGE_MAP, *LPSTAGE_MAP;

// �v���g�^�C�v�錾
void UniteFileName(const char* pFileName, const char* pFiletype);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureStage = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStage = NULL;				// ���_�o�b�t�@�̃|�C���^
D3DXVECTOR3 g_posStage;										// �X�e�[�W�̌��݈ʒu
STAGE_MAP g_aStageMap[STAGE_MAX];							// �e�X�e�[�W
STAGE g_stageExac = STAGE_GRASS;											// ���݂̃X�e�[�W
int g_aTurn[STAGE_MAX];										// �e�X�e�[�W�̌��݂̃^�[��
float g_fLengthStage;										// �X�e�[�W�̑Ίp���̒���
float g_fAngleStage;										// �X�e�[�W�̑Ίp���̊p�x
char g_aEnemyFileName[STRING_MAX];							// ��Փx�ʂ̃t�@�C����
char g_aFileName[STRING_MAX];			
bool g_bAllClear;											// ���S�N���A������

int g_nCounterTimer;
bool g_bHaveKeyExac = false;								// �v���C���[�͌��������Ă�����
bool g_bHaveSecret = false;									// �v���C���[�̓V�[�N���b�g�A�C�e���������Ă��邩

//================================================================================================================
// �X�e�[�W�̏���������
//================================================================================================================
void InitStage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	HWND hWnd = NULL;
	memset(g_aEnemyFileName, NULL, sizeof(g_aEnemyFileName));
	memset(g_aFileName, NULL, sizeof(g_aEnemyFileName));

	switch (GetGameDifficulty())
	{
	case GAMEDIFFICULTY_EASY:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_EASY);

		break;

	case GAMEDIFFICULTY_NORMAL:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_NORMAL);

		break;

	case GAMEDIFFICULTY_HARD:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_HARD);

		break;

	default:

		strcpy(&g_aEnemyFileName[0], ENEMY_FILETYPE_NORMAL);

		break;
	}

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		g_aStageMap[nCntStage].stage = (STAGE)nCntStage;
		g_aStageMap[nCntStage].nTurn = 0;
		g_aStageMap[nCntStage].bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\STAGE\\STAGE.jpg",
		&g_pTextureStage);

	g_nCounterTimer = 0;
	g_bAllClear = false;
	g_bHaveKeyExac = false;
	g_stageExac = STAGE_GRASS;

	// �Ίp���̒������擾
	g_fLengthStage = sqrtf(STAGE_WIDTH * STAGE_WIDTH + STAGE_HEIGHT * STAGE_HEIGHT) * 0.5f;

	// �Ίp���̊p�x���Z�o
	g_fAngleStage = atan2f(STAGE_WIDTH, STAGE_HEIGHT);

	g_posStage.x = SCREEN_WIDTH * 0.5f;
	g_posStage.y = SCREEN_HEIGHT * 0.5f;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * STAGE_MAX,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffStage,
								NULL);

	VERTEX_2D* pVtx = NULL;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = g_posStage.x + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.y = g_posStage.y + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_posStage.x + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.y = g_posStage.y + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_posStage.x + sinf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.y = g_posStage.y + cosf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_posStage.x + sinf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.y = g_posStage.y + cosf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.z = 0.0f;

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		if (nCntStage < STAGE_GRASS || nCntStage >= STAGE_MAX)
		{
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{ // �X�e�[�W�̎�ނ�������͈͊O�������ꍇ�A�����𒆒f
#ifdef  _DEBUG
				MessageBox(hWnd, "���o�C", "���H", MB_ICONWARNING);
#endif
				return;
			}
		}
		else
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.125f * nCntStage, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * nCntStage + 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * nCntStage, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * nCntStage + 0.125f, 1.0f);
		}

		pVtx += 4;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// �X�e�[�W�̏I������
//================================================================================================================
void UninitStage(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureStage != NULL)
	{
		g_pTextureStage->Release();
		g_pTextureStage = NULL;
	}
	

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffStage != NULL)
	{
		g_pVtxBuffStage->Release();
		g_pVtxBuffStage = NULL;
	}
}

//================================================================================================================
// �X�e�[�W�̍X�V����
//================================================================================================================
void UpdateStage(void)
{
	//��ŏ���
	VERTEX_2D *pVtx = NULL;					// ���_���ւ̃|�C���^
	PLAYER *pPlayer = GetPlayer();		// �v���C���[�ւ̃|�C���^
	HWND hWnd = NULL;
	int nCnt = 0;

	D3DXVECTOR3 pos = pPlayer->moveposPlayer;		// �v���C���[�̈ړ��ʂ���

	if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED
		&& GetTotalEnemy() == NULL
		&& GetFadeStage() == FADESTAGE_NONE)
	{
		// �X�e�[�W�̃^�[���ɂ�菈����ύX

		// �G��z�u
		switch (g_aStageMap[g_stageExac].stage)
		{
		case STAGE_GRASS:

			UniteFileName(GRASS_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_DESERT:

			UniteFileName(DESERT_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_ICE:

			UniteFileName(ICE_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_FOREST:

			UniteFileName(FOREST_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_VOLCANO:

			UniteFileName(VOLCANO_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;

		case STAGE_SEA:

			UniteFileName(SEA_FILENAME, &g_aEnemyFileName[0]);
			LoadEnemy(&g_aFileName[0], g_aStageMap[g_stageExac].nTurn);

			break;
		}
		
		if (g_stageExac < STAGE_GRASS || g_stageExac >= STAGE_MAX)
		{
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{
#ifdef  _DEBUG
				MessageBox(hWnd, "���ꂳ����Ă�ł�������","���H",MB_ICONWARNING);
#endif
				return;
			}
		}
		else
		{
			if (g_aStageMap[g_stageExac].nTurn < MAX_TURN)
			{
				g_aStageMap[g_stageExac].nTurn++;
			}
			else if (g_aStageMap[g_stageExac].nTurn >= MAX_TURN)
			{
				g_aStageMap[g_stageExac].nTurn = ALREADY_CLEARED;
			}
		}
	}

	if (g_bAllClear == false)
	{
		for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
		{
			if (g_aStageMap[nCntStage].nTurn == ALREADY_CLEARED) nCnt++;

			if (nCnt >= STAGE_MAX)
			{
				SetGameState(GAMESTATE_CLEAREND, 120);
				g_bAllClear = true;
			}
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI + g_fAngleStage) * g_fLengthStage;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = (g_posStage.x + pos.x) + sinf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.y = (g_posStage.y + pos.y) + cosf(D3DX_PI - g_fAngleStage) * g_fLengthStage;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = (g_posStage.x + pos.x) + sinf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.y = (g_posStage.y + pos.y) + cosf(-g_fAngleStage) * g_fLengthStage;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = (g_posStage.x + pos.x) + sinf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.y = (g_posStage.y + pos.y) + cosf(g_fAngleStage) * g_fLengthStage;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStage->Unlock();
}

//================================================================================================================
// �X�e�[�W�̕`�揈��
//================================================================================================================
void DrawStage(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffStage, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < STAGE_MAX; nCnt++)
	{
		if (g_aStageMap[nCnt].bUse == true)
		{
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureStage);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCnt,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);						// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// �X�e�[�W�̐ݒ�
//================================================================================================================
void SetStage(STAGE stage)
{
	PLAYER *pPlayer = GetPlayer();
	HWND hWnd = NULL;
	static bool g_bHaveKeyExac = false;

	if (g_stageExac < STAGE_GRASS || g_stageExac >= STAGE_MAX)
	{
		if (SUCCEEDED(GetHandleWindow(&hWnd)))
		{ // �X�e�[�W�̎�ނ�������͈͊O�������ꍇ�A�����𒆒f
#ifdef  _DEBUG
			MessageBox(hWnd, "���o�C", "���H", MB_ICONWARNING);
#endif
			return;
		}
	}
	else
	{
		g_aStageMap[g_stageExac].bUse = false;

		g_stageExac = stage;

		g_aStageMap[g_stageExac].bUse = true;

		// ���ݔz�u����Ă���u���b�N�����ׂď���
		ResetBlock();

		// ���ݔz�u���̃A�C�e��������
		DestroyItem();

		// ���ݐ����Ă���e�ۂ����ׂ�Kill
		BombBullet();

		// �G�t�F�N�g������
		DestroyEffect();

		UniteFileName(FRAME_FILENAME, FRAME_LEFT_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		UniteFileName(FRAME_FILENAME, FRAME_RIGHT_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		UniteFileName(FRAME_FILENAME, FRAME_UP_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		UniteFileName(FRAME_FILENAME, FRAME_DOWN_FILETYPE);
		LoadBlock(&g_aFileName[0]);

		pPlayer->posPlayer.x += (pPlayer->moveposPlayer.x * 2.0f);
		pPlayer->posPlayer.y += (pPlayer->moveposPlayer.y * 2.0f);
		if (pPlayer->state != PLAYERSTATE_APPEAR && pPlayer->state != PLAYERSTATE_WAIT)
		{
			pPlayer->state = PLAYERSTATE_TELEPORT_COOLDOWN;
			pPlayer->nCounterState = TELEPORT_COOLTIME;
		}

		// �w�肳�ꂽ�X�e�[�W�ֈڍs
		switch (g_aStageMap[g_stageExac].stage)
		{
		//================================================================================================================
		case STAGE_GRASS:

			UniteFileName(GRASS_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			// �����N���A�ς݂łȂ����
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// �^�[����������
				g_aTurn[STAGE_GRASS] = 0;
			}

			break;
		//================================================================================================================
		case STAGE_DESERT:

			UniteFileName(DESERT_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			if (pPlayer->bHaveKey != true && g_bHaveKeyExac == false)
			{
				SetItem(ITEMTYPE_KEY, D3DXVECTOR3(-110.0f, 710.0, 0.0));
			}

			// �����N���A�ς݂łȂ����
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// �^�[����������
				g_aTurn[STAGE_DESERT] = 0;
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(390.0f, 10.0, 0.0));
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(390.0f, 210.0, 0.0));
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(590.0f, 210.0, 0.0));
				SetItem(ITEMTYPE_COIN, D3DXVECTOR3(790.0f, 210.0, 0.0));
			}

			break;
		//================================================================================================================
		case STAGE_ICE:

			UniteFileName(ICE_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			// �����N���A�ς݂łȂ����
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// �^�[����������
				g_aTurn[STAGE_ICE] = 0;
				SetItem(ITEMTYPE_ULTIMATE_COIN, D3DXVECTOR3(1590.0f, 610.0, 0.0));
			}

			break;
		//================================================================================================================
		case STAGE_FOREST:

			UniteFileName(FOREST_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			// �����N���A�ς݂łȂ����
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// �^�[����������
				g_aTurn[STAGE_FOREST] = 0;
			}

			break;
		//================================================================================================================
		case STAGE_VOLCANO:

			UniteFileName(VOLCANO_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			if (g_bHaveSecret == false)
			{
				SetItem(ITEMTYPE_SECRET, D3DXVECTOR3(-310.0f, 610.0f, 0.0f));
			}

			// �����N���A�ς݂łȂ����
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// �^�[����������
				g_aTurn[STAGE_VOLCANO] = 0;
			}

			break;
		//================================================================================================================
		case STAGE_SEA:

			UniteFileName(SEA_FILENAME, BLOCK_FILETYPE);
			LoadBlock(&g_aFileName[0]);

			SetItem(ITEMTYPE_ULTIMATE_COIN, D3DXVECTOR3(1490.0f, 350.0f, 0.0f));
			

			// �����N���A�ς݂łȂ����
			if (g_aStageMap[g_stageExac].nTurn != ALREADY_CLEARED)
			{// �^�[����������
				g_aTurn[STAGE_SEA] = 0;
			}

			break;
		//================================================================================================================
		default:

			break;
		}
	}
}

//================================================================================================================
// ���݂̃X�e�[�W�̎擾
//================================================================================================================
STAGE GetStage(void)
{
	return g_stageExac;
}

//================================================================================================================
// ���݂̃^�[�����擾
//================================================================================================================
int GetTurn(void)
{
	return g_aStageMap[g_stageExac].nTurn;
}

//================================================================================================================
// �t�@�C���A�h���X�̍���
//================================================================================================================
void UniteFileName(const char* pFileName, const char* pFiletype)
{
	memset(&g_aFileName[0], 0, sizeof(STRING_MAX));

	strcat(&g_aFileName[0], pFileName);

	strcat(&g_aFileName[0], pFiletype);
}

void SetClearBossStage(bool bClear)
{
	g_bAllClear = bClear;
}

//================================================================================================================
// �v���C���[���L�[���E������
//================================================================================================================
void SetEnableHaveKeyExac(bool bHave)
{
	g_bHaveKeyExac = bHave;
}

//================================================================================================================
// �v���C���[���V�[�N���b�g�A�C�e�����E������
//================================================================================================================
void SetEnableHaveSecret(bool bHave)
{
	g_bHaveSecret = bHave;
}

//================================================================================================================
// ���v�N���A�X�e�[�W���̎擾
//================================================================================================================
int GetClearStageNum(void)
{
	LPSTAGE_MAP pStage = &g_aStageMap[0];
	int nCleared = 0;

	for (int nCntStage = 0; nCntStage < STAGE_MAX; nCntStage++, pStage++)
	{
		if (pStage->nTurn == ALREADY_CLEARED) nCleared++;
	}

	return nCleared;
}

//================================================================================================================
// �B���A�C�e�����擾�����̎擾
//================================================================================================================
bool GetHaveSecret(void)
{
	return g_bHaveSecret;
}