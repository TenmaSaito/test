//================================================================================================================
//
// DirectX�̐����ݒu���� [number.cpp]
// Author : TENMA
//
//================================================================================================================
#include "number.h"

// �}�N����`
#define MAX_NUM			(128)				// �ݒu�\�ő吔
#define NUM_WIDTH		(20)				// �����̏�������
#define NUM_HEIGHT		(30)				// �����̏����c��

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureNumber = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNumber = NULL;	// ���_�o�b�t�@�̃|�C���^

NUMBER g_aNumber[MAX_NUM];							

// �^�C�}�[�̏���������
void InitNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	NUMBER* pNumber = &g_aNumber[0];
	int nCntNumber;

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++,pNumber++)
	{
		pNumber->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pNumber->col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pNumber->nNumber = 0;
		pNumber->fLength = 0.0f;
		pNumber->bUse = false;
	}

	// �Ίp���̒������擾
	pNumber->fLength = sqrtf(powf(NUM_WIDTH, 2.0f) + powf(NUM_HEIGHT, 2.0f)) * 0.5f;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\CHARACTER\\number003.png",
		&g_pTextureNumber);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUM,			// NUM_PLACE���̒��_���쐬
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNumber,
		NULL);

	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.75f) * pNumber->fLength;
		pVtx[0].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.75f) * pNumber->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.75f) * pNumber->fLength;
		pVtx[1].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.75f) * pNumber->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.25f) * pNumber->fLength;
		pVtx[2].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.25f) * pNumber->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.25f) * pNumber->fLength;
		pVtx[3].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.25f) * pNumber->fLength;
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

	g_pVtxBuffNumber->Unlock();

}

// �^�C�}�[�̏I������
void UninitNumber(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	if (g_pTextureNumber != NULL)
	{
		g_pTextureNumber->Release();
		g_pTextureNumber = NULL;
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffNumber != NULL)
	{
		g_pVtxBuffNumber->Release();
		g_pVtxBuffNumber = NULL;
	}
}

// �^�C�}�[�̍X�V����
void UpdateNumber(void)
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^
	NUMBER* pNumber = &g_aNumber[0];
	// ��ŏ��������Ꮡ��
	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++, pNumber++)
	{
		if (pNumber->bUse == true)
		{
			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.75f) * pNumber->fLength;
			pVtx[0].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.75f) * pNumber->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.75f) * pNumber->fLength;
			pVtx[1].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.75f) * pNumber->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pNumber->pos.x + sinf(D3DX_PI * -0.25f) * pNumber->fLength;
			pVtx[2].pos.y = pNumber->pos.y + cosf(D3DX_PI * -0.25f) * pNumber->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pNumber->pos.x + sinf(D3DX_PI * 0.25f) * pNumber->fLength;
			pVtx[3].pos.y = pNumber->pos.y + cosf(D3DX_PI * 0.25f) * pNumber->fLength;
			pVtx[3].pos.z = 0.0f;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((0.1f * pNumber->nNumber), 0.0f);
			pVtx[1].tex = D3DXVECTOR2((0.1f * pNumber->nNumber) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((0.1f * pNumber->nNumber), 1.0f);
			pVtx[3].tex = D3DXVECTOR2((0.1f * pNumber->nNumber) + 0.1f, 1.0f);
		}

		pVtx += 4;
	}
	g_pVtxBuffNumber->Unlock();
}

// �^�C�}�[�̕`�揈��
void DrawNumber(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	int nCntNumber;
	NUMBER* pNumber = &g_aNumber[0];


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffNumber, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureNumber);

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++,pNumber++)
	{
		if (pNumber->bUse == true)
		{
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				4 * nCntNumber,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

// �^�C�}�[�̐ݒ菈��
int SetNumber(int nNumber, D3DXVECTOR3 pos)
{
	VERTEX_2D* pVtx;		// ���_���ւ̃|�C���^
	int nCntNumber;
	NUMBER* pNumber = &g_aNumber[0];

	for (nCntNumber = 0; nCntNumber < MAX_NUM; nCntNumber++, pNumber++)
	{
		if (pNumber->bUse == false)
		{
			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

			pNumber->nNumber = nNumber;
			pNumber->pos = pos;

			pVtx += 4 * nCntNumber;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

			pNumber->bUse = true;

			break;
		}

		
	}

	g_pVtxBuffNumber->Unlock();

	return nCntNumber;
}

// �^�C�}�[�̎擾
int GetNumber(int nID)
{
	return g_aNumber[nID].nNumber;
}

// �^�C�}�[�̒ǉ�����
void AddNumber(int nID, int nValue)
{
	VERTEX_2D* pVtx;					// ���_���ւ̃|�C���^

	g_aNumber[nID].nNumber += nValue;

	if (g_aNumber[nID].nNumber <= 0)
	{
		g_aNumber[nID].nNumber = 0;
	}

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNumber->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nID;

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber), 0.0f);
	pVtx[1].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber) + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber), 1.0f);
	pVtx[3].tex = D3DXVECTOR2((0.1f * g_aNumber[nID].nNumber) + 0.1f, 1.0f);

	g_pVtxBuffNumber->Unlock();
}

void DestroyNumber(int nID)
{
	g_aNumber[nID].bUse = false;
}