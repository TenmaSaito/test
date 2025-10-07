//================================================================================================================
//
// DirectX�̃^�C�g���Z���N�g���� [titleSelect.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "title.h"
#include "titleSelect.h"
#include "input.h"
#include "sound.h"
#include "block.h"
#include "fade.h"
#include "game.h"
#include "settings.h"
#include "title_Mage.h"
#include "titlelogo.h"

// �}�N����`
#define SELECTWAIT_STATE		(100)							// �I����̑ҋ@����
#define SHADOW_ALPHA			(0.05f)							// �e�̔Z��
#define SELECT_NORMAL			D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 0.0f)			// �I�������I�΂�Ă���Ƃ��̈ʒu
#define SELECT_UNSELECT			D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f + 200.0f, 0.0f)	// �I�������I�΂�Ă��Ȃ����̈ʒu
#define SELECT_UPPER			(SELECT_NORMAL.y)				// �I�����̕�����ł���Ƃ��̈ʒu
#define SELECT_LOWER			(SELECT_UPPER - 200.0f)	// �I�����̑I��ł��Ȃ���Ԃ̍���(����)
#define SELECTSHADOW_NORMAL		D3DXVECTOR3(SCREEN_WIDTH * 0.3f, 416.0f, 0.0f)		// �e�̏����ʒu
#define SELECT_MOVE_X				(0.95f)							// ���ɓ�����
#define SELECT_MOVE_Y				(1.5f)							// �c�ɓ�����
#define SELECT_MOVE_X_MAX			(15.0f)							// ���ɓ������̏㉺��
#define SELECT_MOVE_Y_MAX			(10.0f)							// �c�ɓ������̏㉺��
#define SELECT_BOOK_NORMAL		D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.4f + (50.0f * (nCntTitleSelect + 1)), 0.0f)		// �{�̑I�����̈ʒu

// �^�C�g���Z���N�g�\����
typedef struct
{
	D3DXVECTOR3 pos;			// �I�����̈ʒu
	D3DXCOLOR col;				// �I�����̐F
	TITLESELECTTYPE type;		// �I�����̎��
	bool bSelect;				// �I������Ă��邩
}TITLESELECT;

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureTitleSelect = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitleSelect = NULL;						// ���_�o�b�t�@�̃|�C���^
TITLESELECT g_aTitleSelect[TITLESELECTTYPE_MAX];							// �^�C�g���Z���N�g�\���̂̐錾
int g_nTitleSelect;															// ���ݑI�΂�Ă���I����
int g_nCounterSelectState;													// �I����̑ҋ@����
int g_nCounterSelect;														// �Z���N�g��ʂ̔ėp�J�E���^�[
bool g_bUseSelect;															// �|�[�Y��ʂ��g���Ă��邩�ǂ���							
bool g_bMoveSelectUpper;													// ������ɓ������ǂ���
bool g_bMoveSelectLeft;														// �������ɓ������ǂ���
bool g_bMoveSelect[2];															// �I�����������Ă邩�ǂ���

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitTitleSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	TITLESELECT *pTitleSelect = &g_aTitleSelect[0];
	int nCntTitleSelect;

	// �e�ϐ��̏�����
	for (nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_BOOKSTART; nCntTitleSelect++, pTitleSelect++)
	{
		pTitleSelect->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, SCREEN_HEIGHT * 0.3f, 0.0f);
		pTitleSelect->col = D3DXCOLOR_NULL;
		pTitleSelect->type = (TITLESELECTTYPE)nCntTitleSelect;
		pTitleSelect->bSelect = false;
	}

	for (nCntTitleSelect = TITLESELECTTYPE_BOOKSTART; nCntTitleSelect < TITLESELECTTYPE_START_SHADOW; nCntTitleSelect++, pTitleSelect++)
	{
		pTitleSelect->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.7f, SCREEN_HEIGHT * 0.4f + (50.0f * (nCntTitleSelect + 1)), 0.0f);
		pTitleSelect->col = D3DXCOLOR_NULL;
		pTitleSelect->type = (TITLESELECTTYPE)nCntTitleSelect;
		pTitleSelect->bSelect = false;
	}

	for (nCntTitleSelect = TITLESELECTTYPE_START_SHADOW; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++, pTitleSelect++)
	{
		pTitleSelect->pos = D3DXVECTOR3(SCREEN_WIDTH * 0.3f, 416.0f, 0.0f);
		pTitleSelect->col = D3DXCOLOR(0.0f,0.0f,0.0f, SHADOW_ALPHA);
		pTitleSelect->type = (TITLESELECTTYPE)nCntTitleSelect;
		pTitleSelect->bSelect = false;
	}


	D3DXCreateTextureFromFile(pDevice,
		TITLE_FILENAME,
		&g_pTextureTitleSelect);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLESELECTTYPE_MAX,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitleSelect,
		NULL);

	g_nTitleSelect = TITLESELECTTYPE_START;
	g_nCounterSelectState = SELECTWAIT_STATE;
	g_nCounterSelect = 0;
	g_aTitleSelect[g_nTitleSelect].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
	g_bMoveSelectUpper = true;
	g_bMoveSelectLeft = true;
	g_bUseSelect = false;
	g_bMoveSelect[0] = false;
	g_bMoveSelect[1] = false;

	VERTEX_2D* pVtx = NULL;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

	pTitleSelect = &g_aTitleSelect[0];

	for (nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[0].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[1].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[2].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[3].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW));
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW));
		pVtx[2].tex = D3DXVECTOR2(0.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW) + 0.125f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 0.125f * (nCntTitleSelect % TITLESELECTTYPE_START_SHADOW) + 0.125f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleSelect->Unlock();

}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitTitleSelect(void)
{
	
	if (g_pTextureTitleSelect != NULL)
	{
		g_pTextureTitleSelect->Release();
		g_pTextureTitleSelect = NULL;
	}
	
	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffTitleSelect != NULL)
	{
		g_pVtxBuffTitleSelect->Release();
		g_pVtxBuffTitleSelect = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateTitleSelect(void)
{
	TITLESELECT *pTitleSelect = &g_aTitleSelect[0];
	VERTEX_2D *pVtx = NULL;
	HWND hWnd;

	for (int nCntSelect = 0; nCntSelect < TITLESELECTTYPE_MAX; nCntSelect++, pTitleSelect++)
	{
		if (g_nCounterSelectState != SELECTWAIT_STATE || GetFade() != FADE_NONE) continue;
		if (pTitleSelect->type >= TITLESELECTTYPE_START && pTitleSelect->type < TITLESELECTTYPE_BOOKSTART)
		{
			if (pTitleSelect->bSelect == true)
			{
				if (pTitleSelect->col.a >= 1.0f) { g_bMoveSelect[0] = false; continue; }
				pTitleSelect->col.a += 0.05f;
				pTitleSelect->pos.y -= 10.0f;
			}
			else
			{
				if (pTitleSelect->col.a <= 0.0f) { g_bMoveSelect[1] = false; continue; }
				pTitleSelect->col.a -= 0.05f;
				pTitleSelect->pos.y += 10.0f;
			}
		}
		else if (pTitleSelect->type >= TITLESELECTTYPE_BOOKSTART && pTitleSelect->type < TITLESELECTTYPE_START_SHADOW)
		{
			if (pTitleSelect->bSelect == false)
			{
				if (pTitleSelect->col.a <= 0.3f) 
				{ 
					continue; 
				}

				pTitleSelect->col.a -= 0.05f;
			}
			else
			{
				if (pTitleSelect->col.a >= 1.0f) continue;
				pTitleSelect->col.a += 0.05f;
			}
		}
		else if (pTitleSelect->type >= TITLESELECTTYPE_START_SHADOW && pTitleSelect->type < TITLESELECTTYPE_MAX)
		{
			if (pTitleSelect->bSelect == true)
			{
				if (pTitleSelect->col.a >= SHADOW_ALPHA) { continue; }
				pTitleSelect->col.a += 0.05f;
			}
			else
			{
				if (pTitleSelect->col.a <= 0.0f) continue;
				pTitleSelect->col.a -= 0.025f;
			}
		}
	}

	if (g_nTitleSelect != TITLESELECTTYPE_MAX 
		&& g_nCounterSelectState == SELECTWAIT_STATE 
		&& GetEnableTitleSelect() == true
		&& g_bMoveSelect[0] == false
		&& g_bMoveSelect[1] == false
		&& GetFade() == FADE_NONE)
	{
		if (GetKeyboardRepeat(DIK_S) == true
			|| GetJoypadRepeat(JOYKEY_DOWN) == true
			|| GetJoyThumbRepeat(JOYTHUMB_LY_DOWN) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT);

			// �I�𒆂��������������A���I����Ԃ�
			g_aTitleSelect[g_nTitleSelect].bSelect = false;
			g_aTitleSelect[g_nTitleSelect].pos = SELECT_NORMAL;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos = SELECTSHADOW_NORMAL;

			g_nTitleSelect++;
			if (g_nTitleSelect >= TITLESELECTTYPE_BOOKSTART)
			{
				g_nTitleSelect = TITLESELECTTYPE_START;
			}

			g_aTitleSelect[g_nTitleSelect].pos = SELECT_UNSELECT;
			SetTitleMage(D3DXVECTOR3(g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.x - 150.0f, g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.y, 0.0f), TITLE_MAGETYPE_SELECT);
			g_aTitleSelect[g_nTitleSelect].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
			SetTitleMage(SELECTSHADOW_NORMAL, TITLE_MAGETYPE_TEXT);

			g_bMoveSelect[0] = true;
			g_bMoveSelect[1] = true;
		}
		else if (GetKeyboardRepeat(DIK_W) == true
			|| GetJoypadRepeat(JOYKEY_UP) == true
			|| GetJoyThumbRepeat(JOYTHUMB_LY_UP) == true)
		{
			PlaySound(SOUND_LABEL_SE_SELECT);

			// �I�𒆂��������������A���I����Ԃ�
			g_aTitleSelect[g_nTitleSelect].bSelect = false;
			g_aTitleSelect[g_nTitleSelect].pos = SELECT_NORMAL;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = false;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos = SELECTSHADOW_NORMAL;

			g_nTitleSelect--;
			if (g_nTitleSelect < TITLESELECTTYPE_START)
			{
				g_nTitleSelect = TITLESELECTTYPE_EXIT;
			}

			g_aTitleSelect[g_nTitleSelect].pos = SELECT_UNSELECT;
			SetTitleMage(D3DXVECTOR3(g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.x - 150.0f, g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.y, 0.0f), TITLE_MAGETYPE_SELECT);
			g_aTitleSelect[g_nTitleSelect].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
			g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
			SetTitleMage(SELECTSHADOW_NORMAL, TITLE_MAGETYPE_TEXT);

			g_bMoveSelect[0] = true;
			g_bMoveSelect[1] = true;
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true
			|| GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_nCounterSelectState--;
			PlaySound(SOUND_LABEL_SE_ENTER);
			LightUpTitleMage(TITLE_MAGETYPE_TEXT);
		}
	}
	else if ((GetKeyboardTrigger(DIK_RETURN) == true
		|| GetJoypadTrigger(JOYKEY_A) == true
		|| GetJoypadTrigger(JOYKEY_START) == true)
		&& GetEnableTitleSelect() == false)
	{
		for (int nCntSelect = (g_nTitleSelect + 1); nCntSelect < TITLESELECTTYPE_BOOKSTART; nCntSelect++)
		{
			g_aTitleSelect[(g_nTitleSelect + nCntSelect) % TITLESELECTTYPE_BOOKSTART].pos.y = SELECT_LOWER;
			g_aTitleSelect[(g_nTitleSelect + nCntSelect) % TITLESELECTTYPE_BOOKSTART].col.a = 0.0f;
		}

		for (int nCntTitle = TITLESELECTTYPE_BOOKSTART; nCntTitle < TITLESELECTTYPE_START_SHADOW; nCntTitle++)
		{
			g_aTitleSelect[nCntTitle].col.a = 0.3f;
		}

		g_bMoveSelect[0] = false;
		g_bMoveSelect[1] = false;
	}

	if(g_nCounterSelectState < SELECTWAIT_STATE)
	{
		// �I�����Ă��鍀�ڂɂ���ď�����ύX
		switch (g_nTitleSelect)
		{
		case TITLESELECTTYPE_START:
			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{ // �Q�[���֐i��
#ifdef SELECTTYPETUTORIAL_TRUE
					SetGameTutorial(false);
#else
					SetGameTutorial(true);
#endif
					SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
					FadeSound(SOUND_LABEL_GAME_TUTORIAL);
				}
			}
			else
			{
				if (g_nCounterSelectState % 20 <= 9)
				{
					g_aTitleSelect[TITLESELECTTYPE_START].col.a = 0.0f;
				}
				else
				{
					g_aTitleSelect[TITLESELECTTYPE_START].col.a = 1.0f;
				}
			}

			g_nCounterSelectState--;

			break;

#ifdef SELECTTYPETUTORIAL_TRUE
		case TITLESELECTTYPE_TUTORIAL:

			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{ // �`���[�g���A���֐i��
					SetGameTutorial(true);
					SetFade(MODE_GAME, FADE_TYPE_TEXTURE);
					FadeSound(SOUND_LABEL_GAME_TUTORIAL);
				}
			}
			else
			{
				if (g_nCounterSelectState % 20 <= 9)
				{
					g_aTitleSelect[TITLESELECTTYPE_TUTORIAL].col.a = 0.0f;
				}
				else
				{
					g_aTitleSelect[TITLESELECTTYPE_TUTORIAL].col.a = 1.0f;
				}
			}

			g_nCounterSelectState--;

			break;
#endif

		case TITLESELECTTYPE_CREDIT:

			if (g_nCounterSelectState <= 0)
			{
				if (GetFade() == FADE_NONE)
				{ // �N���W�b�g�֐i��
					SetFade(MODE_CREDIT, FADE_TYPE_TEXTURE);
					FadeSound(SOUND_LABEL_CREDIT);
				}
			}
			else
			{
				if (g_nCounterSelectState % 20 <= 9)
				{
					g_aTitleSelect[TITLESELECTTYPE_CREDIT].col.a = 0.0f;
				}
				else
				{
					g_aTitleSelect[TITLESELECTTYPE_CREDIT].col.a = 1.0f;
				}
			}

			g_nCounterSelectState--;

			break;

		case TITLESELECTTYPE_EXIT:
			if (SUCCEEDED(GetHandleWindow(&hWnd)))
			{ // �Q�[�����I��
				DestroyWindow(hWnd);
			}

			break;

		default:

			break;
		}
	}

	if (g_nCounterSelect % 5 == 0)
	{
		if (g_nTitleSelect >= TITLESELECTTYPE_START && g_nTitleSelect <= TITLESELECTTYPE_EXIT)
		{
			if (g_bMoveSelectUpper == true)
			{
				g_aTitleSelect[g_nTitleSelect].pos.y -= SELECT_MOVE_Y;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.y -= SELECT_MOVE_Y;
				if (g_aTitleSelect[g_nTitleSelect].pos.y < SELECT_UPPER - SELECT_MOVE_Y_MAX) g_bMoveSelectUpper = false;
			}
			else
			{
				g_aTitleSelect[g_nTitleSelect].pos.y += SELECT_MOVE_Y;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.y += SELECT_MOVE_Y;
				if (g_aTitleSelect[g_nTitleSelect].pos.y > SELECT_UPPER + SELECT_MOVE_Y_MAX) g_bMoveSelectUpper = true;
			}
		}
	}
	else if (g_nCounterSelect % 16 == 0)
	{
		if (g_nTitleSelect >= TITLESELECTTYPE_START && g_nTitleSelect <= TITLESELECTTYPE_EXIT)
		{
			if (g_bMoveSelectUpper == true)
			{
				g_aTitleSelect[g_nTitleSelect].pos.x -= SELECT_MOVE_X;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.x -= SELECT_MOVE_X;
				if (g_aTitleSelect[g_nTitleSelect].pos.x < SELECT_NORMAL.x - SELECT_MOVE_X_MAX) g_bMoveSelectLeft = false;
			}
			else
			{
				g_aTitleSelect[g_nTitleSelect].pos.x += SELECT_MOVE_X;
				g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].pos.x += SELECT_MOVE_X;
				if (g_aTitleSelect[g_nTitleSelect].pos.x > SELECT_NORMAL.x + SELECT_MOVE_X_MAX) g_bMoveSelectLeft = true;
			}
		}
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitleSelect->Lock(0, 0, (void**)&pVtx, 0);

	pTitleSelect = &g_aTitleSelect[0];

	for (int nCntVtx = 0; nCntVtx < TITLESELECTTYPE_MAX; nCntVtx++,pTitleSelect++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[0].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[1].pos.y = pTitleSelect->pos.y - (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pTitleSelect->pos.x - (SELECTSIZE_WIDTH * 0.5f);
		pVtx[2].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pTitleSelect->pos.x + (SELECTSIZE_WIDTH * 0.5f);
		pVtx[3].pos.y = pTitleSelect->pos.y + (SELECTSIZE_HEIGHT * 0.5f);
		pVtx[3].pos.z = 0.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pTitleSelect->col;
		pVtx[1].col = pTitleSelect->col;
		pVtx[2].col = pTitleSelect->col;
		pVtx[3].col = pTitleSelect->col;
	
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitleSelect->Unlock();

	g_nCounterSelect++;
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawTitleSelect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	TITLESELECT* pSelect = &g_aTitleSelect[0];

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTitleSelect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	for (int nCntTitleSelect = 0; nCntTitleSelect < TITLESELECTTYPE_MAX; nCntTitleSelect++, pSelect++)
	{
		if (g_bUseSelect == true)
		{
			// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
			pDevice->SetTexture(0, g_pTextureTitleSelect);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntTitleSelect,						// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

TITLESELECTTYPE GetTitleSelect(void)
{
	return (TITLESELECTTYPE)g_nTitleSelect;
}

void SetTitleSelect(bool bUse)
{
	g_bUseSelect = bUse;

	SetTitleMage(D3DXVECTOR3(g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.x - 150.0f, g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].pos.y, 0.0f), TITLE_MAGETYPE_SELECT);
	g_aTitleSelect[g_nTitleSelect].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_BOOKSTART].bSelect = true;
	g_aTitleSelect[g_nTitleSelect + TITLESELECTTYPE_START_SHADOW].bSelect = true;
	SetTitleMage(SELECTSHADOW_NORMAL, TITLE_MAGETYPE_TEXT);
}