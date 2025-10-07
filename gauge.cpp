//================================================================================================================
//
// DirectX�̃Q�[�W���� [Gauge.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "game.h"
#include "player.h"
#include "gauge.h"
#include "sound.h"
#include "timer.h"

// �}�N����`
#define MAX_GAUGE			(16)				// �Q�[�W�̍ő吔
#define GAUGE_WIDTH			(200.0f)			// �Q�[�W�̉��̃T�C�Y
#define GAUGE_HEIGHT		(200.0f)			// �Q�[�W�̍���
#define GAUGE_LENGTH		(50.0f)				// �Q�[�W�̔��a

// �O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGauge = NULL;	// ���_�o�b�t�@�̃|�C���^
Gauge g_aGauge[MAX_GAUGE];						// �Q�[�W�̏��

//================================================================================================================
// �Q�[�W�̏���������
//================================================================================================================
void InitGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^, �f�o�C�X�̎擾

	Gauge *pGauge = &g_aGauge[0];						// �Q�[�W�\���̂̐擪�A�h���X���擾

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++,pGauge++)
	{// �Q�[�W�\���̂̏�����
		pGauge->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pGauge->col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		pGauge->type = GAUGETYPE_CHARGE;
		pGauge->nPercentGauge = 0;
		pGauge->fLength = 0.0f;
		pGauge->bUse = false;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * GAUGE_VERTEX * MAX_GAUGE,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGauge,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++)
	{
		float fAngleGauge = 0;
		pVtx[0].pos.x = SCREEN_WIDTH * 0.5f;
		pVtx[0].pos.y = SCREEN_HEIGHT * 0.5f;
		pVtx[0].rhw = 1.0f;
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		for (int nCntPos = 1; nCntPos < GAUGE_VERTEX; nCntPos++,fAngleGauge += (D3DX_PI * 2.0f) / GAUGE_MAX)
		{
			pVtx[nCntPos].pos.x = sinf(D3DX_PI - fAngleGauge) * pGauge->fLength;
			pVtx[nCntPos].pos.y = cosf(D3DX_PI - fAngleGauge) * pGauge->fLength;
			pVtx[nCntPos].rhw = 1.0f;
			pVtx[nCntPos].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}

		pVtx += GAUGE_VERTEX;
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//================================================================================================================
// �Q�[�W�̏I������
//================================================================================================================
void UninitGauge(void)
{
	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffGauge != NULL)
	{
		g_pVtxBuffGauge->Release();
		g_pVtxBuffGauge = NULL;
	}
}

//================================================================================================================
// �Q�[�W�̍X�V����
//================================================================================================================
void UpdateGauge(void)
{
	Gauge *pGauge = &g_aGauge[0];
	VERTEX_2D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
    g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == true)
		{// �Q�[�W���g���Ă����
			if (GetTimer() == NOMORE_TIME && pGauge->type == GAUGETYPE_TIMER)
			{
				pGauge->col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f);
			}

			float fAngleGauge = 0;
			pVtx[0].pos.x = pGauge->pos.x;
			pVtx[0].pos.y = pGauge->pos.y;
			pVtx[0].rhw = 1.0f;
			pVtx[0].col = pGauge->col;

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			for (int nCntPos = 1; nCntPos < GAUGE_VERTEX; nCntPos++, fAngleGauge += (D3DX_PI * 2.0f) / (GAUGE_MAX))
			{
				pVtx[nCntPos].pos.x = pGauge->pos.x + sinf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				pVtx[nCntPos].pos.y = pGauge->pos.y + cosf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				// rhw�̐ݒ�
				pVtx[nCntPos].rhw = 1.0f;
				// rhw�̐ݒ�
				pVtx[nCntPos].col = pGauge->col;
			}
		}

		pVtx += GAUGE_VERTEX;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();
}

//================================================================================================================
// �Q�[�W�̕`�揈��
//================================================================================================================
void DrawGauge(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�̃|�C���^
	Gauge *pGauge = &g_aGauge[0];		// �Q�[�W�̐擪�A�h���X
		
	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGauge, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, NULL);

	for (int nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,		// �v���~�e�B�u�̎��(�~�`)
				GAUGE_VERTEX * nCntGauge,					// �`�悷��ŏ��̒��_�C���f�b�N�X
				pGauge->nPercentGauge);						// �`�悷��v���~�e�B�u�̐�(�\�����鐔�͕ϓ�)
		}
	}
}

//================================================================================================================
// �Q�[�W�̍쐬����
// �Ԃ�l : �g��ꂽ�Q�[�W��No (NOMORE_GAUGE�̏ꍇ�̓Q�[�W�ɋ󂫂��Ȃ��ꍇ)
//================================================================================================================
int SetGauge(D3DXVECTOR3 pos, D3DXCOLOR col,GAUGETYPE type, int nStart, float fLength)
{
	Gauge *pGauge = &g_aGauge[0];
	VERTEX_2D* pVtx;
	int nCntGauge;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntGauge = 0; nCntGauge < MAX_GAUGE; nCntGauge++, pGauge++)
	{
		if (pGauge->bUse == false)
		{// �Q�[�W���g���Ă��Ȃ����
  			pGauge->type = type;
			pGauge->pos = pos;
			pGauge->col = col;
			pGauge->nPercentGauge = nStart;
			pGauge->fLength = fLength;

			float fAngleGauge = 0;
			// ���_���W�̐ݒ�(���_�̂ݓ���ݒ�)

			pVtx[0].pos.x = pGauge->pos.x;
			pVtx[0].pos.y = pGauge->pos.y;
			pVtx[0].rhw = 1.0f;
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			for (int nCntPos = 1; nCntPos < GAUGE_VERTEX; nCntPos++, fAngleGauge += (D3DX_PI * 2.0f) / (GAUGE_VERTEX))
			{
				pVtx[nCntPos].pos.x = pGauge->pos.x + sinf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				pVtx[nCntPos].pos.y = pGauge->pos.y + cosf(D3DX_PI - fAngleGauge) * pGauge->fLength;
				// rhw�̐ݒ�
				pVtx[nCntPos].rhw = 1.0f;
				// ���_�J���[�ݒ�
				pVtx[nCntPos].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			pGauge->bUse = true;

			break;
		}

		pVtx += GAUGE_VERTEX;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGauge->Unlock();

	if (nCntGauge < MAX_GAUGE)
	{
		return nCntGauge;
	}
	else
	{
		return NOMORE_GAUGE;
	}
}

//================================================================================================================
// �Q�[�W�̒ǉ�����
//================================================================================================================
void AddGauge(int nValue,int nNo)
{
	// �w�肵���Q�[�W�̒l�ɉ��Z
	g_aGauge[nNo].nPercentGauge += nValue;
}

//================================================================================================================
// �Q�[�W�̎擾
//================================================================================================================
Gauge *GetGauge(void)
{
	return &g_aGauge[0];
}