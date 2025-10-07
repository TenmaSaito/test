//================================================================================================================
//
// DirectX�̃t�F�[�h���� [fade.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "fade.h"
#include "input.h"
#include "sound.h"

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureFade = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;	// ���_�o�b�t�@�̃|�C���^
FADE g_fade;									// �t�F�[�h�̎��
MODE g_modeNext;								// ���(���[�h)�̎��
D3DXCOLOR g_colorFade;							// �|���S���̐F
FADE_TYPE g_typeFade;							// �t�F�[�h�̃^�C�v
bool g_bUseTexture;								// �e�N�X�`�����g����

//================================================================================================================
// �w�i�̏���������
//================================================================================================================
void InitFade(MODE modeNext)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	g_fade = FADE_IN;					// �t�F�[�h�C����Ԃ�

	g_modeNext = modeNext;				// ���̉�ʂ�ݒ�

	g_colorFade.a = 1.0f;				// alpha�l��������

	g_typeFade = FADE_TYPE_NORMAL;		//�@�ʏ�t�F�[�h�ɐݒ�

	g_bUseTexture = false;				// �e�N�X�`���𖢎g�p�ɐݒ�

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\FADE\\LODING.png",
		&g_pTextureFade);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,			// sizeof�̌�K�� * ���_�� ���������ƁI
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffFade,
								NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_colorFade.a);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//================================================================================================================
// �w�i�̏I������
//================================================================================================================
void UninitFade(void)
{
	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//================================================================================================================
// �w�i�̍X�V����
//================================================================================================================
void UpdateFade(void)
{
	if (g_fade != FADE_NONE)
	{
		if (g_fade == FADE_IN)
		{// �t�F�[�h�C��
			g_colorFade.a -= 0.01f;
			if (g_colorFade.a <= 0.0f)
			{// �t�F�[�h�C���I��
				g_colorFade.a = 0.0f;
				g_fade = FADE_NONE;
			}

			if ((GetKeyboardTrigger(DIK_RETURN) == true
				|| GetJoypadTrigger(JOYKEY_A) == true
				|| GetJoypadTrigger(JOYKEY_START) == true) 
				&& g_modeNext == MODE_TITLE)
			{
				g_colorFade.a = 0.0f;
				SetVolume(SOUND_STANDARD, SETSOUND_MAX);
			}
		}
		else if(g_fade == FADE_OUT)
		{// �t�F�[�h�A�E�g
			g_colorFade.a += 0.01f;
			if (g_colorFade.a >= 1.0f)
			{// �t�F�[�h�A�E�g�I��
				g_colorFade.a = 1.0f;
				g_fade = FADE_IN;

				// ���[�h�ݒ�
				SetMode(g_modeNext);
			}
		}

		VERTEX_2D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		// ���_�o�b�t�@���A�����b�N����
		g_pVtxBuffFade->Unlock();
	}
}

//================================================================================================================
// �w�i�̕`�揈��
//================================================================================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	switch (g_typeFade)
	{
	case FADE_TYPE_NORMAL:
		// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
		pDevice->SetTexture(0, NULL);

		break;

	case FADE_TYPE_TEXTURE:
		// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
		pDevice->SetTexture(0, g_pTextureFade);

		break;
	}

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
					       0,						// �`�悷��ŏ��̒��_�C���f�b�N�X
						   2);						// �`�悷��v���~�e�B�u�̐�
}

// �t�F�[�h�̐ݒ�
void SetFade(MODE modeNext, FADE_TYPE type)
{
	g_fade = FADE_OUT;

	g_modeNext = modeNext;

	switch (type)
	{
	case FADE_TYPE_NORMAL:

		g_colorFade = NORMAL_FADE;
		g_typeFade = FADE_TYPE_NORMAL;
		g_bUseTexture = false;

		break;

	case FADE_TYPE_TEXTURE:

		g_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_typeFade = FADE_TYPE_TEXTURE;
		g_bUseTexture = true;

		break;

	default:

		break;
	}
}

// �t�F�[�h�̎擾
FADE GetFade(void)
{
	return g_fade;
}