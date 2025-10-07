//================================================================================================================
//
// DirectX�̊e��Q�[�����ݒ菈�� [settings.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "settings.h"
#include "input.h"
#include "sound.h"
#include "block.h"
#include "fade.h"
#include "game.h"
#include "volume.h"
#include "pause.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_apTextureSettings[SETTINGTYPE_MAX] = {};	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSettings = NULL;					// ���_�o�b�t�@�̃|�C���^
FADE g_FadeSettings;												// �t�F�[�h�̎��
SETTINGTYPE g_Setting;												// ���ݑI������Ă���I����
D3DXCOLOR g_colorSettings;											// �|���S���̐F
bool g_bUseSettings;												// �ݒ��ʂ��g���Ă��邩�ǂ���							

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitSettings(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	// �e�ϐ��̏�����
	g_FadeSettings = FADE_NONE;
	g_Setting = SETTINGTYPE_NONE;
	g_colorSettings.a = 0.0f;
	g_bUseSettings = false;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings.png",
		&g_apTextureSettings[SETTINGTYPE_NONE]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings_GameSound.png",
		&g_apTextureSettings[SETTINGTYPE_GAME_SOUND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings_SE.png",
		&g_apTextureSettings[SETTINGTYPE_SE_SOUND]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\PauseToSettings_exit.png",
		&g_apTextureSettings[SETTINGTYPE_EXIT]);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSettings,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSettings->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorSettings.a);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSettings->Unlock();
}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitSettings(void)
{
	for (int nCntSettings = 0; nCntSettings < SETTINGTYPE_MAX; nCntSettings++)
	{
		if (g_apTextureSettings[nCntSettings] != NULL)
		{
			g_apTextureSettings[nCntSettings]->Release();
			g_apTextureSettings[nCntSettings] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffSettings != NULL)
	{
		g_pVtxBuffSettings->Release();
		g_pVtxBuffSettings = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateSettings(void)
{
	bool pSettings = GetGamePause();
	XINPUT_STATE *pState = GetJoypadState();

	if (g_FadeSettings != FADE_NONE)
	{
		if (g_FadeSettings == FADE_IN)
		{// �t�F�[�h�C��
			g_colorSettings.a += 0.175f;
			if (g_colorSettings.a >= 1.0f)
			{// �t�F�[�h�C���I��
				g_colorSettings.a = 1.0f;
				g_FadeSettings = FADE_NONE;
				SetVolume();
			}
		}
		else if (g_FadeSettings == FADE_OUT)
		{// �t�F�[�h�A�E�g
			g_colorSettings.a -= 0.175f;
			if (g_colorSettings.a <= 0.0f)
			{// �t�F�[�h�A�E�g�I��
				g_colorSettings.a = 0.0f;
				g_FadeSettings = FADE_NONE;
				g_bUseSettings = false;
				DeleteVolume();
				SELECTTYPE* pSelect = GetSelect();
				*pSelect = SELECTTYPE_SETTINGS;
			}
		}

		VERTEX_2D* pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffSettings->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_colorSettings.a);

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffSettings->Unlock();
	}
	else if(g_bUseSettings == true)
	{
		if (GetKeyboardRepeat(DIK_W) == true
			|| GetKeyboardRepeat(DIK_UP) == true
			|| GetJoypadRepeat(JOYKEY_UP) == true
			|| (GetJoyThumbSlow(JOYTHUMB_LY_UP) == true && pState->Gamepad.sThumbLY >= 25000.0f))
		{
			switch (g_Setting)
			{
			case SETTINGTYPE_GAME_SOUND:

				g_Setting = SETTINGTYPE_EXIT;

				break;

			case SETTINGTYPE_SE_SOUND:

				g_Setting = SETTINGTYPE_GAME_SOUND;

				break;

			case SETTINGTYPE_EXIT:

				g_Setting = SETTINGTYPE_SE_SOUND;

				break;
			}
		}
		else
		{
			if (GetKeyboardRepeat(DIK_S) == true
				|| GetKeyboardRepeat(DIK_DOWN) == true
				|| GetJoypadRepeat(JOYKEY_DOWN) == true
				|| (GetJoyThumbSlow(JOYTHUMB_LY_DOWN) == true && pState->Gamepad.sThumbLY <= -25000.0f))
			{
				switch (g_Setting)
				{
				case SETTINGTYPE_GAME_SOUND:

					g_Setting = SETTINGTYPE_SE_SOUND;

					break;

				case SETTINGTYPE_SE_SOUND:

					g_Setting = SETTINGTYPE_EXIT;

					break;

				case SETTINGTYPE_EXIT:

					g_Setting = SETTINGTYPE_GAME_SOUND;

					break;
				}

			}
		}

		if ((GetJoypadTrigger(JOYKEY_A) == true && g_Setting == SETTINGTYPE_EXIT)
			|| GetJoypadTrigger(JOYKEY_B) == true
			|| GetKeyboardTrigger(DIK_RETURN) == true)
		{// �ݒ��ʂ����
			g_FadeSettings = FADE_OUT;
		}

		if (GetKeyboardRepeat(DIK_D) == true
			|| GetJoypadRepeat(JOYKEY_RIGHT) == true
			|| pState->Gamepad.sThumbLX >= 10000)
		{
			switch (g_Setting)
			{
			case SETTINGTYPE_GAME_SOUND:

				AddVolume(1, SETSOUND_GAME);

				break;

			case SETTINGTYPE_SE_SOUND:

				AddVolume(1, SETSOUND_MENU_SE);

				break;
			}
		}
		else if (GetKeyboardRepeat(DIK_A) == true
			|| GetJoypadRepeat(JOYKEY_LEFT) == true
			|| pState->Gamepad.sThumbLX <= -10000)
		{
			switch (g_Setting)
			{
			case SETTINGTYPE_GAME_SOUND:

				AddVolume(-1, SETSOUND_GAME);

				break;

			case SETTINGTYPE_SE_SOUND:

				AddVolume(-1, SETSOUND_MENU_SE);

				break;
			}
		}
	}
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawSettings(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSettings, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
	pDevice->SetTexture(0, g_apTextureSettings[g_Setting]);

	if (g_bUseSettings == true)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
			0,											// �`�悷��ŏ��̒��_�C���f�b�N�X
			2);											// �`�悷��v���~�e�B�u�̐�
	}
}

// �t�F�[�h�̐ݒ�
void SetSettings(void)
{
	g_bUseSettings = true;

	g_Setting = SETTINGTYPE_GAME_SOUND;
	g_FadeSettings = FADE_IN;
}

// �t�F�[�h�̎擾
FADE GetSettings(void)
{
	return g_FadeSettings;
}