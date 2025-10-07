//================================================================================================================
//
// DirectX�̃����L���O���� [ranking.cpp]
// Author : TENMA
//
//================================================================================================================
#include "ranking.h"
#include "score.h"
#include "game.h"
#include "input.h"

// �}�N����`
#define NUM_PLACE			(8)				// �X�R�A�̌���
#define NUM_WIDTH			(25)			// �ꌅ�̉���
#define NUM_HEIGHT			(35)			// �ꌅ�̏c��
#define MAX_RANKING			(5)				// �\�����鏇�ʂ̐�
#define FIRSTMOVE_RANKING	(4)				// �ŏ��ɉE����o�ė��鏇��
#define MOVE_SPD			D3DXVECTOR3(15.0f,0.0f,0.0f)								// �ړ����鑬��
#define GOAL_POS			D3DXVECTOR3(700.0f, 120.0f * (nCntRanking + 1), 0.0f)		// �ŏI�I�ȃX�R�A�̈ʒu
#define FILENAME_RANKING_EASY	"data\\RANKING\\Ranking_E.bin"			// �����L���O�ۑ��p�t�@�C����(EASY)
#define FILENAME_RANKING_NORMAL	"data\\RANKING\\Ranking_N.bin"			// �����L���O�ۑ��p�t�@�C����(NORMAL)
#define FILENAME_RANKING_HARD	"data\\RANKING\\Ranking_H.bin"			// �����L���O�ۑ��p�t�@�C����(HARD)
#define FILENAME_RESET		"data\\RANKING\\Ranking_Reset.bin"			// ���Z�b�g�p�o�C�i���t�@�C����
#define RESET_COMMAND_NUM	(7)				// �X�R�A���Z�b�g���ɓ��͂���L�[�̍��v��

// �����L���O�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;						// �X�R�A�\���̈ʒu
	D3DXCOLOR col;							// �F
	int nScore;								// �X�R�A�̒l
	int nRanking;							// ����
	bool bAdd;								// �ǉ����ꂽ�X�R�A��
	bool bUse;								// �g���Ă��邩
	bool bMove;								// ��ʓ��ɓ����Ă��邩
}RANKING, *PRANKING;

// �v���g�^�C�v�錾
void CheckRanking(PRANKING pRanking);
int Compare(const void* a, const void* b);
void ResetRanking(void);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureRanking = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;						// ���_�o�b�t�@�̃|�C���^
RANKING g_aRanking[GAMEDIFFICULTY_MAX][MAX_RANKING];					// �����L���O�\���̂̐錾
int g_nScoreCurrent;													// ����̃X�R�A
int g_nCounterRanking;													// �ėp�J�E���^�[
int g_nCurrentMoveRanking;												// ���ݓ����Ă��鏇��
char g_aRankingFileName[STRING_MAX];									// �����L���O�̃t�@�C����
bool g_aTrigger[RESET_COMMAND_NUM];										// �w��̃L�[�����͂��ꂽ��
	
//================================================================================================================
// �����L���O�̏���������
//================================================================================================================
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntRanking;
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �\���̂̏�����
	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		pRanking->pos = D3DXVECTOR3(1380.0f, 120.0f * (nCntRanking + 1), 0.0f);
		pRanking->col = D3DXCOLOR_NULL;
		pRanking->nScore = 0;
		pRanking->nRanking = 1;
		pRanking->bAdd = false;
		pRanking->bUse = false;
	}

	switch (GetGameDifficulty())
	{
	case GAMEDIFFICULTY_EASY:

		strcpy(g_aRankingFileName, FILENAME_RANKING_EASY);

		break;

	case GAMEDIFFICULTY_NORMAL:

		strcpy(g_aRankingFileName, FILENAME_RANKING_NORMAL);

		break;

	case GAMEDIFFICULTY_HARD:

		strcpy(g_aRankingFileName, FILENAME_RANKING_HARD);

		break;

	default:

		strcpy(g_aRankingFileName, FILENAME_RANKING_NORMAL);

		break;
	}

	g_nCounterRanking = 0;
	g_nCurrentMoveRanking = FIRSTMOVE_RANKING;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureRanking);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (NUM_PLACE * MAX_RANKING),			// NUM_PLACE * MAX_RANKING���̒��_���쐬
	D3DUSAGE_WRITEONLY,
	FVF_VERTEX_2D,
	D3DPOOL_MANAGED,
	& g_pVtxBuffRanking,
	NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) - NUM_WIDTH;
			pVtx[0].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[0].pos.z = 0.0f;
	
			pVtx[1].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) + NUM_WIDTH;
			pVtx[1].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[1].pos.z = 0.0f;
	
			pVtx[2].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) - NUM_WIDTH;
			pVtx[2].pos.y = pRanking->pos.y + NUM_HEIGHT;
			pVtx[2].pos.z = 0.0f;
	
			pVtx[3].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntScore) + NUM_WIDTH;
			pVtx[3].pos.y = pRanking->pos.y + NUM_HEIGHT;
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
	}
	
	g_pVtxBuffRanking->Unlock();
	
	if (GetModeExac() == MODE_GAMECLEAR || GetModeExac() == MODE_GAMEOVER)
	{
		AddRanking(GetScore());
	}
	else if (GetModeExac() == MODE_TITLE)
	{
		CheckRanking(&g_aRanking[GetGameDifficulty()][0]);
	}
}

//================================================================================================================
// �����L���O�̏I������
//================================================================================================================
void UninitRanking(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}
}

//================================================================================================================
// �����L���O�̍X�V����
//================================================================================================================
void UpdateRanking(void)
{
	// ��ŏ��������Ꮡ��
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];;
	VERTEX_2D* pVtx;

	if ((GetKeyboardPress(DIK_R) == true
		&& GetKeyboardPress(DIK_RSHIFT) == true)
		||(GetJoypadPress(JOYKEY_A) == true
			&& GetJoypadPress(JOYKEY_B) == true
			&& GetJoypadPress(JOYKEY_X) == true
			&& GetJoypadPress(JOYKEY_Y) == true))
	{
		ResetRanking();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true)
	{
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
		{
			pRanking->pos = GOAL_POS;
		}

		g_nCurrentMoveRanking = -1;
		pRanking = &g_aRanking[GetGameDifficulty()][0];
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		if (g_nCurrentMoveRanking == nCntRanking)
		{
			pRanking->pos -= MOVE_SPD;
			if (pRanking->pos.x <= 700.0f)
			{
				pRanking->pos.x = 700.0f;
				g_nCurrentMoveRanking--;
			}
		}

		if (pRanking->bAdd == true)
		{
			if ((g_nCounterRanking % 15) <= 7)
			{
				pRanking->col.a -= 0.08f;
				if (pRanking->col.a < 0.5f)
				{
					pRanking->col.a = 0.5f;
				}
			}
			else
			{
				pRanking->col.a += 0.08f;
				if (pRanking->col.a > 1.0f)
				{
					pRanking->col.a = 1.0f;
				}
			}
		}

		for (int nCntCol = 0; nCntCol < NUM_PLACE; nCntCol++)
		{
			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) - NUM_WIDTH;
			pVtx[0].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) + NUM_WIDTH;
			pVtx[1].pos.y = pRanking->pos.y - NUM_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) - NUM_WIDTH;
			pVtx[2].pos.y = pRanking->pos.y + NUM_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pRanking->pos.x + ((NUM_WIDTH * 2) * nCntCol) + NUM_WIDTH;
			pVtx[3].pos.y = pRanking->pos.y + NUM_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, pRanking->col.a);

			pVtx += 4;
		}

		g_pVtxBuffRanking->Unlock();
	}

	g_nCounterRanking++;
}

//================================================================================================================
// �����L���O�̕`�揈��
//================================================================================================================
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	int nCntRanking;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRanking);

	for (nCntRanking = 0; nCntRanking < (NUM_PLACE * MAX_RANKING); nCntRanking++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			4 * nCntRanking,							// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

//================================================================================================================
// �����L���O�̕\������
//================================================================================================================
void SetRanking(void)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];
	int aTexU[MAX_RANKING][NUM_PLACE];				//�e���̐��������[

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++,pRanking++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			aTexU[nCntRanking][nCntScore] = pRanking->nScore % (int)powf(10.0f, (float)(NUM_PLACE - nCntScore)) / (int)powf(10.0f, (float)(NUM_PLACE - nCntScore) - 1.0f);
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * aTexU[nCntRanking][nCntScore]) + 0.1f, 1.0f);

			pVtx += 4;
		}
	}

	g_pVtxBuffRanking->Unlock();
}

//================================================================================================================
// �����L���O�̎擾
//================================================================================================================
int GetRanking(void)
{
	return g_nScoreCurrent;
}

//================================================================================================================
// �����L���O�̒ǉ�����
//================================================================================================================
void AddRanking(int nScore)
{
	PRANKING pRanking = &g_aRanking[GetGameDifficulty()][0];
	FILE* pFile;
	HWND hWnd = GetActiveWindow();
	int aScore[MAX_RANKING + 1] = {};
	int nCntRanking;

	g_nScoreCurrent = nScore;

	CheckRanking(pRanking);
	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		aScore[nCntRanking] = pRanking->nScore;
	}

	aScore[nCntRanking] = g_nScoreCurrent;

	qsort(&aScore[0], MAX_RANKING + 1, sizeof(int), Compare);

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		pRanking->nRanking = nCntRanking;
		pRanking->nScore = aScore[nCntRanking];
	}

	pRanking = &g_aRanking[GetGameDifficulty()][0];

	for (nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		if (pRanking->nScore == g_nScoreCurrent)
		{
			pRanking->bAdd = true;
			break;
		}
	}

	pFile = fopen(&g_aRankingFileName[0], "wb");
	if (pFile == NULL) 
	{
		MessageBox(hWnd, "�����L���O�f�[�^�̏����o���Ɏ��s�I", "�x��", MB_ICONWARNING);
		return;
	}

	fwrite(&aScore[0], sizeof(int), MAX_RANKING, pFile);

	fclose(pFile);
}

//================================================================================================================
// �����L���O�̓ǂݍ��ݏ���
//================================================================================================================
void CheckRanking(PRANKING pRanking)
{
	FILE* pFile;
	HWND hWnd = NULL;
	int aBuffer[MAX_RANKING];		// �X�R�A�̈ꎞ�ۊǏꏊ

	pFile = fopen(&g_aRankingFileName[0], "rb");
	if (pFile == NULL) 
	{ 
		// �t�@�C���̓ǂݍ��݂Ɏ��s�����ꍇ�A����̃����L���O�f�[�^����
		for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
		{
			pRanking->nRanking = nCntRanking;
			pRanking->nScore = 155000 * (MAX_RANKING - nCntRanking);
			if (pRanking->nScore == 0) pRanking->bUse = false;
		}

		return;
	}

	fread(aBuffer, sizeof(int), MAX_RANKING, pFile);
	fclose(pFile);

	// �擾���������L���O�f�[�^����
	for (int nCntRanking = 0; nCntRanking < MAX_RANKING; nCntRanking++, pRanking++)
	{
		pRanking->nRanking = nCntRanking;
		pRanking->nScore = aBuffer[nCntRanking];
		if (pRanking->nScore == 0) pRanking->bUse = false;
	}
}

//================================================================================================================
// ��r�֐�
//================================================================================================================
int Compare(const void* a, const void* b)
{
	int ret;
	const int* p_int1 = (int *)a;
	const int* p_int2 = (int *)b;

	if (*p_int1 < *p_int2) {
		ret = 1;
	}
	else if (*p_int1 > *p_int2) {
		ret = -1;
	}
	else {
		ret = 0;
	}

	return ret;
}

//================================================================================================================
// �����L���O�̃��Z�b�g����
//================================================================================================================
void ResetRanking(void)
{
	HWND hWnd; 
	BOOL BSUCCEED;
	switch (GetGameDifficulty())
	{
	case GAMEDIFFICULTY_EASY:

		BSUCCEED = CopyFileA(FILENAME_RESET, FILENAME_RANKING_EASY, FALSE);		// ���Z�b�g�p�̃o�C�i���t�@�C���Ō��݂̃X�R�A�t�@�C�����㏑��

		break;

	case GAMEDIFFICULTY_NORMAL:

		BSUCCEED = CopyFileA(FILENAME_RESET, FILENAME_RANKING_EASY, FALSE);		// ���Z�b�g�p�̃o�C�i���t�@�C���Ō��݂̃X�R�A�t�@�C�����㏑��

		break;

	case GAMEDIFFICULTY_HARD:

		BSUCCEED = CopyFileA(FILENAME_RESET, FILENAME_RANKING_EASY, FALSE);		// ���Z�b�g�p�̃o�C�i���t�@�C���Ō��݂̃X�R�A�t�@�C�����㏑��

		break;

	default:

		BSUCCEED = FALSE;

		break;
	}

	GetHandleWindow(&hWnd);
#ifdef _DEBUG
	if (BSUCCEED == FALSE)
	{
		MessageBox(hWnd, "FAILED", "ERORR", MB_ICONERROR);
	}
#endif
}