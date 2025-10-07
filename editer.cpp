//================================================================================================================
//
// DirectX�̃G�f�B�^��cpp�t�@�C�� [editer.h]
// Author : TENMA
//
//================================================================================================================
#include "editer.h"
#include "input.h"
#include "enemy.h"
#include "block.h"
#include "stage.h"
#include "fade_stage.h"

// �}�N����`
#define EDITER_SPD			(50)		// �G�f�B�^�u���b�N�̑��x
#define FILE_POS			"POS = "	// �t�@�C���ł�POS
#define FILE_TYPE			"TYPE = "	// �t�@�C���ł�TYPE
#define FILE_BULLET			"BULLET = "	// �t�@�C���ł�BULLET
#define FILE_TEX			"TEX = "	// �t�@�C���ł�TEX
#define FILE_MOVE			"MOVE = "	// �t�@�C���ł�MOVE
#define FILE_LIFE			"LIFE = "	// �t�@�C���ł�LIFE	

// �G�f�B�^�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos[MAX_EDITER_BLOCK];		// �ݒu�ʒu
	BLOCKTYPE typeBlock[MAX_EDITER_BLOCK];	// �u���b�N�̃^�C�v
	ENEMYTYPE typeEnemy[MAX_EDITER_BLOCK];	// �G�̎��
	ENEMYTEX texEnemy[MAX_EDITER_BLOCK];	// �G�̃e�N�X�`���̎��
	D3DXVECTOR3 move[MAX_EDITER_BLOCK];		// �ړ���
	int nTurn[MAX_EDITER_BLOCK];			// �ݒu����^�[��
	bool bUseBlock[MAX_EDITER_BLOCK];		// �u���b�N���ݒu����Ă��邩(false = enemy)
	int nCounterBlock;		// �u���b�N��ݒu������
	int nCounterEnemy;		// �G��ݒu������
}EDITER;

// �O���[�o���ϐ�
int g_nID;					// �G�f�B�^�u���b�N��ID
int g_nCounterEditer;		// �G�f�B�^�u���b�N�̔z�u������
bool g_bUseEditer;
EDITER g_Editer;
LPD3DXFONT g_pFontEditer = NULL;							// �t�H���g�ւ̃|�C���^


//================================================================================================================
// �X�e�[�W�G�f�B�^�̏���������
//================================================================================================================
void InitEditer(void)
{ 
	EDITER* pEditer = &g_Editer;

	for (int nCntEditer = 0; nCntEditer < MAX_EDITER_BLOCK; nCntEditer++)
	{
		pEditer->pos[nCntEditer] = D3DXVECTOR3_NULL;
		pEditer->typeBlock[nCntEditer] = BLOCKTYPE_WALL;
		pEditer->typeEnemy[nCntEditer] = ENEMYTYPE_FIX;
		pEditer->texEnemy[nCntEditer] = ENEMYTEX_SLIME;
		pEditer->move[nCntEditer] = D3DXVECTOR3_NULL;
		pEditer->bUseBlock[nCntEditer] = false;
		pEditer->nTurn[nCntEditer] = 0;
	}

	pEditer->nCounterBlock = 0;
	pEditer->nCounterEnemy = 0;
	g_nCounterEditer = 0;

	// �f�o�b�O�p�t�H���g�̐���
	D3DXCreateFont(GetDevice(),
		30,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"terminal",
		&g_pFontEditer);
}

//================================================================================================================
// �X�e�[�W�G�f�B�^�̏I������
//================================================================================================================
void UninitEditer(void)
{ // ����

}

//================================================================================================================
// �X�e�[�W�G�f�B�^�̍X�V����
//================================================================================================================
void UpdateEditer(void)
{
	BLOCK *pBlock;
	EDITER* pEditer = &g_Editer;
	HWND hWnd;

	if (g_bUseEditer == true)
	{
		pBlock = GetBlock(g_nID);
		hWnd = GetActiveWindow();

		if (pBlock->type == BLOCKTYPE_EDITER)
		{// �X�e�[�W�G�f�B�^�u���b�N
			if (GetKeyboardRepeat(DIK_TAB) != true)
			{
				// �L�[�{�[�h���͂̊e����
				if (GetKeyboardRepeat(DIK_UP) == true)
				{// W���������Ƃ�
					// �ǉ�����
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// ����ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * -0.75f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * -0.75f) * EDITER_SPD;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// �E��ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * 0.75f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * 0.75f) * EDITER_SPD;
					}
					else
					{// ��ֈړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.y -= EDITER_SPD;
					}
				}
				else if (GetKeyboardRepeat(DIK_DOWN) == true)
				{// S���������Ƃ�
					// �ǉ�����
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// �����ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * -0.25f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * -0.25f) * EDITER_SPD;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// �E���ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * 0.25f) * EDITER_SPD;
						pBlock->pos.y += cosf(D3DX_PI * 0.25f) * EDITER_SPD;
					}
					else
					{// ���ֈړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.y += EDITER_SPD;
					}
				}
				else if (GetKeyboardRepeat(DIK_LEFT) == true)
				{// A���������Ƃ�

					// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
					pBlock->pos.x -= EDITER_SPD;
				}
				else if (GetKeyboardRepeat(DIK_RIGHT) == true)
				{// D���������Ƃ�
					// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
					pBlock->pos.x += EDITER_SPD;
				}
			}
			else
			{
				// �L�[�{�[�h���͂̊e����
				if (GetKeyboardRepeat(DIK_UP) == true)
				{// W���������Ƃ�
					// �ǉ�����
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// ����ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * -0.75f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * -0.75f) * 1;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// �E��ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * 0.75f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * 0.75f) * 1;
					}
					else
					{// ��ֈړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.y -= 1;
					}
				}
				else if (GetKeyboardRepeat(DIK_DOWN) == true)
				{// S���������Ƃ�
					// �ǉ�����
					if (GetKeyboardRepeat(DIK_LEFT) == true)
					{// �����ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * -0.25f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * -0.25f) * 1;
					}
					else if (GetKeyboardRepeat(DIK_RIGHT) == true)
					{// �E���ړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.x += sinf(D3DX_PI * 0.25f) * 1;
						pBlock->pos.y += cosf(D3DX_PI * 0.25f) * 1;
					}
					else
					{// ���ֈړ�
						// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
						pBlock->pos.y += 1;
					}
				}
				else if (GetKeyboardRepeat(DIK_LEFT) == true)
				{// A���������Ƃ�

					// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
					pBlock->pos.x -= 1;
				}
				else if (GetKeyboardRepeat(DIK_RIGHT) == true)
				{// D���������Ƃ�
					// �G�f�B�^�u���b�N�̈ړ��ʂ��X�V
					pBlock->pos.x += 1;
				}
			}
		}

		if (GetKeyboardTrigger(DIK_O) == true)
		{
			int nType = pEditer->typeBlock[g_nCounterEditer];
			int nTypeEnemy = pEditer->typeEnemy[g_nCounterEditer];

			nTypeEnemy++;
			nType++;
			if (nType >= BLOCKTYPE_MAX)
			{
				nType = BLOCKTYPE_WALL;
			}

			if (nTypeEnemy >= ENEMYTYPE_MAX)
			{
				nTypeEnemy = ENEMYTYPE_FIX;
			}

			pEditer->typeBlock[g_nCounterEditer] = (BLOCKTYPE)nType;
			pEditer->typeEnemy[g_nCounterEditer] = (ENEMYTYPE)nTypeEnemy;
		}
		else if (GetKeyboardTrigger(DIK_P) == true)
		{
			int nType = pEditer->typeBlock[g_nCounterEditer];
			int nTypeEnemy = pEditer->typeEnemy[g_nCounterEditer];

			nTypeEnemy--;
			nType--;
			if (nType < BLOCKTYPE_WALL)
			{
				nType = BLOCKTYPE_MAX - 1;
			}

			if (nTypeEnemy < ENEMYTYPE_FIX)
			{
				nTypeEnemy = ENEMYTYPE_MAX - 1;
			}

			pEditer->typeBlock[g_nCounterEditer] = (BLOCKTYPE)nType;
			pEditer->typeEnemy[g_nCounterEditer] = (ENEMYTYPE)nTypeEnemy;
		}

		if (GetKeyboardTrigger(DIK_K) == true)
		{
			int nTexEnemy = pEditer->texEnemy[g_nCounterEditer];

			nTexEnemy++;

			if (nTexEnemy >= ENEMYTEX_MAX)
			{
				nTexEnemy = ENEMYTEX_SLIME;
			}


			pEditer->texEnemy[g_nCounterEditer] = (ENEMYTEX)nTexEnemy;
		}
		else if (GetKeyboardTrigger(DIK_L) == true)
		{
			int nTexEnemy = pEditer->texEnemy[g_nCounterEditer];

			nTexEnemy--;

			if (nTexEnemy < ENEMYTEX_SLIME)
			{
				nTexEnemy = ENEMYTEX_MAX - 1;
			}


			pEditer->texEnemy[g_nCounterEditer] = (ENEMYTEX)nTexEnemy;
		}

		if (GetKeyboardTrigger(DIK_COMMA) == true)
		{
			pEditer->nTurn[g_nCounterEditer]++;
		}
		else if (GetKeyboardTrigger(DIK_COLON) == true)
		{
			pEditer->nTurn[g_nCounterEditer]--;
		}

		if (GetKeyboardTrigger(DIK_N) == true)
		{
			pEditer->move[g_nCounterEditer].z++;
		}
		else if (GetKeyboardTrigger(DIK_M) == true)
		{
			pEditer->move[g_nCounterEditer].z--;
		}

		if (g_nCounterEditer < MAX_EDITER_BLOCK)
		{
			if (GetKeyboardTrigger(DIK_RSHIFT) == true)
			{
				if (g_nCounterEditer >= MAX_EDITER_BLOCK)
				{
					MessageBox(hWnd, "�u���b�N�̔z�u�\������𒴂��Ă��܂��I", "�x���I", MB_ICONWARNING);
					return;
				}

				SetBlock(BLOCKTYPE_EDITER_WALL, pBlock->pos, BLOCK_WIDTH, BLOCK_HEIGHT);
				pEditer->nCounterBlock++;
				pEditer->pos[g_nCounterEditer] = pBlock->pos;
				pEditer->bUseBlock[g_nCounterEditer] = true;
				g_nCounterEditer++;
			}
			else if (GetKeyboardTrigger(DIK_LSHIFT) == true)
			{
				if (g_nCounterEditer >= MAX_EDITER_BLOCK)
				{
					MessageBox(hWnd, "�u���b�N�̔z�u�\������𒴂��Ă��܂��I", "�x���I", MB_ICONWARNING);
					return;
				}

				SetBlock(BLOCKTYPE_EDITER_ENEMY, pBlock->pos, BLOCK_WIDTH, BLOCK_HEIGHT);
				pEditer->nCounterEnemy++;
				pEditer->pos[g_nCounterEditer] = pBlock->pos;
				pEditer->bUseBlock[g_nCounterEditer] = false;
				g_nCounterEditer++;
			}
		}

		if (GetKeyboardTrigger(DIK_F5) == true && g_nCounterEditer != 0)
		{
			char aStr[10][STRING_MAX] = {};

			if (SUCCEEDED(SaveBlock()) && SUCCEEDED(SaveEnemy()))
			{
				strcat(&aStr[0][0], "�Z�[�u����");

				wsprintf(&aStr[1][0], "�z�u�����G�f�B�^�u���b�N�̑��� : %d\n", g_nCounterEditer);
				wsprintf(&aStr[2][0], "�z�u�����u���b�N�̑��� : %d\n", pEditer->nCounterBlock);
				wsprintf(&aStr[3][0], "�z�u�����G�̑��� : %d\n", pEditer->nCounterEnemy);

				strcat(&aStr[0][0], &aStr[1][0]);
				strcat(&aStr[0][0], &aStr[2][0]);
				strcat(&aStr[0][0], &aStr[3][0]);

				MessageBox(hWnd, &aStr[0][0], "����", MB_ICONINFORMATION);
			}
			else
			{
				MessageBox(hWnd, "����ɃZ�[�u����܂���ł����B", "�x���I", MB_ICONWARNING);
			}

			g_nCounterEditer = 0;
			g_bUseEditer = false;
			DeleteBlock(g_nID);

			SetFadeStage(STAGE_GRASS, FADESTAGE_OUT);
		}

		if ((pBlock->pos.x) <= STAGE_LEFT + (BLOCK_WIDTH))
		{
			pBlock->pos.x = STAGE_LEFT + (BLOCK_WIDTH);
		}

		if (pBlock->pos.x >= STAGE_RIGHT - (BLOCK_WIDTH))
		{
			pBlock->pos.x = (float)(STAGE_RIGHT - (BLOCK_WIDTH));
		}

		if (pBlock->pos.y <= STAGE_CEILING + (BLOCK_HEIGHT))
		{
			pBlock->pos.y = STAGE_CEILING + (BLOCK_HEIGHT);
		}

		if (pBlock->pos.y > STAGE_FLOOR - (BLOCK_HEIGHT))
		{
			pBlock->pos.y = (float)(STAGE_FLOOR - (BLOCK_HEIGHT));
		}
	}
}

//================================================================================================================
// �X�e�[�W�G�f�B�^�̕`�揈��
//================================================================================================================
void DrawEditer(void)
{ // ����
	EDITER* pEditer = &g_Editer;
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			// ��ʃT�C�Y
	char aStr[15][256];										// ��ʂɕ\�����镶����

	// ������ɑ��
	wsprintf(aStr[0], "�ݒu�����G�f�B�^�u���b�N�̐� : %d\n" ,g_nCounterEditer);

	wsprintf(aStr[1], "�z�u�����u���b�N�̑��� : %d\n", pEditer->nCounterBlock);

	wsprintf(aStr[2], "�z�u�����G�̑��� : %d\n", pEditer->nCounterEnemy);

	wsprintf(aStr[3], "���݂̃u���b�N�̎�� : %d\n", (int)pEditer->typeBlock[g_nCounterEditer]);

	wsprintf(aStr[4], "���݂̓G�̎�� : %d\n", (int)pEditer->typeEnemy[g_nCounterEditer]);

	wsprintf(aStr[5], "���݂̓G�̃e�N�X�`���̎�� : %d\n", (int)pEditer->texEnemy[g_nCounterEditer]);

	wsprintf(aStr[6], "���݂̃^�[�� : %d\n", (int)pEditer->nTurn[g_nCounterEditer]);

	// �����������
	for (int nCntDebug = 0; nCntDebug < 6; nCntDebug++)
	{
		strcat(&aStr[0][0], &aStr[nCntDebug + 1][0]);
	}

	// �e�L�X�g�̕`��
	//g_pFontEditer->DrawText(NULL, &aStr[0][0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(0, 0, 0, 255));

}

//================================================================================================================
// �X�e�[�W�G�f�B�^�̗L����
//================================================================================================================
void SetEnableEditer(bool bUse, int nID)
{
	g_bUseEditer = bUse;
	g_nID = nID;
}

//================================================================================================================
// �u���b�N�̏����o������
//================================================================================================================
HRESULT SaveBlock(void)
{
	char aStr[STRING_MAX] = {};
	char aNum[STRING_MAX] = {};
	int nBlock = 0;
	FILE* pFile;
	D3DXVECTOR3 pos;
	EDITER* pEditer = &g_Editer;
	HWND hWnd = GetActiveWindow();

	pFile = fopen("data\\STAGE\\DEBUG\\BLOCK.txt", "w");
	if (pFile != NULL)
	{
		AddFunctionLog("START : BLOCK SAVE");

		fprintf(pFile, "START_SCRIPT\n\n");
		fprintf(pFile, "#========================================\n");
		fprintf(pFile, "# �u���b�N�̔z�u\n");
		fprintf(pFile, "#========================================\n");

		for (int nCntBlock = 0; nCntBlock < g_nCounterEditer; nCntBlock++)
		{
			if (pEditer->bUseBlock[nCntBlock] == false)
			{
				continue;
			}

			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "# %d �Ԗڂ̃u���b�N\n", nBlock + 1);
			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "START_SETBLOCK\n\n");

			fprintf(pFile, FILE_POS);
			fprintf(pFile, "%.1f %.1f %.1f\n", pEditer->pos[nCntBlock].x, pEditer->pos[nCntBlock].y, pEditer->pos[nCntBlock].z);

			fprintf(pFile, FILE_TYPE);
			fprintf(pFile, "%d\n\n", pEditer->typeBlock[nCntBlock]);

			fprintf(pFile, "END_SETBLOCK\n");
			fprintf(pFile, "#========================================\n\n");

			nBlock++;
		}

		fprintf(pFile, "END_SCRIPT");

		fclose(pFile);

		MessageBox(hWnd, "�t�@�C���f�[�^�̏����o���ɐ���", "����", MB_ICONINFORMATION);

		return S_OK;
	}
	else
	{
		MessageBox(hWnd, "�t�@�C���f�[�^�̏����o���Ɏ��s�I(BLOCK)", "�x���I", MB_ICONWARNING);

		return E_FAIL;
	}
}

//================================================================================================================
// �G�̏����o������
//================================================================================================================
HRESULT SaveEnemy(void)
{
	char aStr[STRING_MAX] = {};
	char aNum[STRING_MAX] = {};
	int nEnemy = 0;
	FILE* pFile;
	D3DXVECTOR3 pos;
	EDITER* pEditer = &g_Editer;
	HWND hWnd = GetActiveWindow();

	pFile = fopen("data\\STAGE\\DEBUG\\ENEMY.txt", "w");
	if (pFile != NULL)
	{
		fprintf(pFile, "START_SCRIPT\n\n");

		for (int nCntTurn = 0; nCntTurn < 3; nCntTurn++)
		{

			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "# �^�[��%d\n", nCntTurn);
			fprintf(pFile, "#========================================\n");

			fprintf(pFile, "START_TURN%d\n\n",nCntTurn);

			fprintf(pFile, "#========================================\n");
			fprintf(pFile, "# �G�̔z�u\n");
			fprintf(pFile, "#========================================\n");

			for (int nCntEnemy = 0; nCntEnemy < g_nCounterEditer; nCntEnemy++)
			{
				if (pEditer->bUseBlock[nCntEnemy] != false || pEditer->nTurn[nCntEnemy] != nCntTurn)
				{
					continue;
				}

				fprintf(pFile, "#========================================\n");
				fprintf(pFile, "# %d �Ԗڂ̓G\n", nEnemy);
				fprintf(pFile, "#========================================\n");
				fprintf(pFile, "START_SETENEMY\n\n");

				fprintf(pFile, FILE_POS);
				fprintf(pFile, "%.1f %.1f %.1f\n", pEditer->pos[nCntEnemy].x, pEditer->pos[nCntEnemy].y, pEditer->pos[nCntEnemy].z);

				fprintf(pFile, FILE_TYPE);
				fprintf(pFile, "%d\n", pEditer->typeEnemy[nCntEnemy]);

				fprintf(pFile, FILE_BULLET);
				fprintf(pFile, "0\n");

				fprintf(pFile, FILE_TEX);
				fprintf(pFile, "%d\n", pEditer->texEnemy[nCntEnemy]);

				fprintf(pFile, FILE_MOVE);
				fprintf(pFile, "%.1f %.1f %.1f\n", pEditer->move[nCntEnemy].x, pEditer->move[nCntEnemy].y, pEditer->move[nCntEnemy].z);

				fprintf(pFile, FILE_LIFE);
				fprintf(pFile, "0\n\n");

				fprintf(pFile, "END_SETENEMY\n");
				fprintf(pFile, "#========================================\n\n");

				nEnemy++;
			}

			fprintf(pFile, "END_TURN%d\n\n", nCntTurn);
		}

		fprintf(pFile, "END_SCRIPT");

		fclose(pFile);

		MessageBox(hWnd, "�t�@�C���f�[�^�̏����o���ɐ���", "����", MB_ICONINFORMATION);

		return S_OK;
	}
	else
	{
		MessageBox(hWnd, "�t�@�C���f�[�^�̏����o���Ɏ��s�I(BLOCK)", "�x���I", MB_ICONWARNING);

		return E_FAIL;
	}
}