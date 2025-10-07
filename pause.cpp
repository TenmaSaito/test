//================================================================================================================
//
// DirectX�̃|�[�Y���� [pause.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "pause.h"
#include "input.h"
#include "sound.h"
#include "block.h"
#include "fade.h"
#include "game.h"
#include "settings.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTexturePause[SELECTTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;					// ���_�o�b�t�@�̃|�C���^
PAUSE g_Pause;													// �t�F�[�h�̎��
SELECTTYPE g_Select;											// ���ݑI������Ă���I����
D3DXCOLOR g_colorPause;											// �|���S���̐F
int g_aVibration[2];											// ���݂̍��E�̃o�C�u���[�V�����̒l
XINPUT_VIBRATION* g_pVibration;									// ���݂̃o�C�u���[�V�����̒l
bool g_bUse;													// �|�[�Y��ʂ��g���Ă��邩�ǂ���							

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	// �e�ϐ��̏�����
	g_Pause = PAUSE_NONE;
	g_Select = SELECTTYPE_NONE;
	g_colorPause.a = 0.0f;
	g_bUse = false;
	g_aVibration[0] = 0;
	g_aVibration[1] = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_continue.png",
		&g_apTexturePause[SELECTTYPE_CONTINUE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_settings.png",
		&g_apTexturePause[SELECTTYPE_SETTINGS]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_restart.png",
		&g_apTexturePause[SELECTTYPE_RESTART]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause_exit.png",
		&g_apTexturePause[SELECTTYPE_EXIT]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\pause.png",
		&g_apTexturePause[SELECTTYPE_NONE]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorPause.a);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitPause(void)
{
	for (int nCntPause = 0; nCntPause < SELECTTYPE_MAX; nCntPause++)
	{
		if (g_apTexturePause[nCntPause] != NULL)
		{
			g_apTexturePause[nCntPause]->Release();
			g_apTexturePause[nCntPause] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdatePause(void)
{
	bool pPause = GetGamePause();
	XINPUT_STATE *pState = GetJoypadState();

	if (g_Pause != PAUSE_NONE)
	{
		if (g_Pause == PAUSE_IN)
		{// �t�F�[�h�C��
			g_colorPause.a += 0.175f;
			if (g_colorPause.a >= 1.0f)
			{// �t�F�[�h�C���I��
				g_colorPause.a = 1.0f;
				g_Pause = PAUSE_NONE;
			}
		}
		else if (g_Pause == PAUSE_OUT)
		{// �t�F�[�h�A�E�g
			g_colorPause.a -= 0.175f;
			if (g_colorPause.a <= 0.0f)
			{// �t�F�[�h�A�E�g�I��
				g_colorPause.a = 0.0f;
				g_Pause = PAUSE_NONE;

				g_pVibration->wLeftMotorSpeed = g_aVibration[0];
				g_pVibration->wRightMotorSpeed = g_aVibration[1];

				XInputSetState(0, g_pVibration);
			}
		}

		VERTEX_2D* pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorPause.a);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPause->Unlock();
	}
	else if(pPause == true)
	{
		if (GetKeyboardRepeat(DIK_W) == true
			|| GetKeyboardRepeat(DIK_UP) == true
			|| GetJoypadRepeat(JOYKEY_UP) == true
			|| (GetJoyThumbSlow(JOYTHUMB_LY_UP) == true && pState->Gamepad.sThumbLY >= 1000.0f))
		{
			switch (g_Select)
			{
			case SELECTTYPE_CONTINUE:

				g_Select = SELECTTYPE_EXIT;

				break;
#if 0
			case SELECTTYPE_SETTINGS:

				g_Select = SELECTTYPE_CONTINUE;

				break;
#endif
			case SELECTTYPE_RESTART:

				g_Select = SELECTTYPE_CONTINUE;

				break;

			case SELECTTYPE_EXIT:

				g_Select = SELECTTYPE_RESTART;

				break;

			case SELECTTYPE_NONE:

				break;
			}

			PlaySound(SOUND_LABEL_SE_SELECT);
		}
		else
		{
			if (GetKeyboardRepeat(DIK_S) == true
				|| GetKeyboardRepeat(DIK_DOWN) == true
				|| GetJoypadRepeat(JOYKEY_DOWN) == true
				|| (GetJoyThumbSlow(JOYTHUMB_LY_DOWN) == true && pState->Gamepad.sThumbLY <= -15000.0f))
			{
				switch (g_Select)
				{
				case SELECTTYPE_CONTINUE:

					g_Select = SELECTTYPE_RESTART;

					break;
#if 0
				case SELECTTYPE_SETTINGS:

					g_Select = SELECTTYPE_RESTART;

					break;
#endif
				case SELECTTYPE_RESTART:

					g_Select = SELECTTYPE_EXIT;

					break;

				case SELECTTYPE_EXIT:

					g_Select = SELECTTYPE_CONTINUE;

					break;

				case SELECTTYPE_NONE:

					break;
				}

				PlaySound(SOUND_LABEL_SE_SELECT);
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true
			|| GetJoypadTrigger(JOYKEY_A) == true)
		{
			switch (g_Select)
			{
			case SELECTTYPE_CONTINUE:

				g_Pause = PAUSE_OUT;

				SetGamePause(false);

				break;
#if 0
			case SELECTTYPE_SETTINGS:

				SetSettings();
				g_Select = SELECTTYPE_NONE;

				break;
#endif
			case SELECTTYPE_RESTART:

				FadeSound(SOUND_LABEL_GAME_NORMAL);
				SetFade(MODE_GAME, FADE_TYPE_NORMAL);
				g_Select = SELECTTYPE_NONE;

				break;

			case SELECTTYPE_EXIT:

				FadeSound(SOUND_LABEL_TITLE);
				SetFade(MODE_TITLE, FADE_TYPE_NORMAL);
				g_Select = SELECTTYPE_NONE;

				break;

			case SELECTTYPE_NONE:

				break;
			}

			PlaySound(SOUND_LABEL_SE_ENTER);
		}

		if (GetJoypadTrigger(JOYKEY_START) == true)
		{
			g_Pause = PAUSE_OUT;

			SetGamePause(false);
		}
	}
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
	pDevice->SetTexture(0, g_apTexturePause[g_Select]);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
		0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
		2);											// �`�悷��v���~�e�B�u�̐�
	
}

// �t�F�[�h�̐ݒ�
void SetPause(bool bUse)
{
	g_bUse = bUse;
	if (g_bUse == true)
	{
		g_Select = SELECTTYPE_CONTINUE;
		g_Pause = PAUSE_IN;
		g_bUse = true;
		g_pVibration = GetJoyVibration();
		

		g_aVibration[0] = g_pVibration->wLeftMotorSpeed;
		g_aVibration[1] = g_pVibration->wRightMotorSpeed;

		g_pVibration->wLeftMotorSpeed = 0;
		g_pVibration->wRightMotorSpeed = 0;

		XInputSetState(0, g_pVibration);
	}
	else
	{
		g_Pause = PAUSE_OUT;
		
		g_bUse = false;
	}
}

// �t�F�[�h�̎擾
PAUSE GetPause(void)
{
	return g_Pause;
}

SELECTTYPE *GetSelect(void)
{
	return &g_Select;
}