//================================================================================================================
//
// DirectX�̃A�C�e���t�@�C�� [item.cpp]
// Author : TENMA
//
//================================================================================================================
#include "item.h"
#include "player.h"
#include "score.h"
#include "heart.h"
#include "sound.h"
#include "stage.h"
#include "playerframe.h"

// �}�N����`
#define MAX_ITEM		(64)			// �u���b�N�̍ő吔

// �v���g�^�C�v�錾
void CollisionPlayer(ITEM *pItem);

// �O���[�o���ϐ�
LPDIRECT3DTEXTURE9		g_pTextureItem = NULL;	// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffItem = NULL;	// ���_�o�b�t�@�̃|�C���^
ITEM g_aItem[MAX_ITEM];						// �u���b�N�\����

//================================================================================================================
// �u���b�N�̏���������
//================================================================================================================
void InitItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	ITEM *pItem = &g_aItem[0];
	int nCntItem;

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		pItem->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pItem->col = D3DXCOLOR_NULL;
		pItem->nAlphaItem = 1;
		pItem->type = ITEMTYPE_COIN;
		pItem->bUse = false;
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\CHARACTER\\ITEM\\ITEM.png",
			&g_pTextureItem);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ITEM,			// sizeof�̌�K�� * ���_�� ���������ƁI
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffItem,
		NULL);

	AddFunctionLog("END : VertexBuffer Create");

	VERTEX_2D* pVtx = NULL;					// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	AddFunctionLog("START : VertexBuffer Setting");

	pItem = &g_aItem[0];

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
		pVtx[0].pos.x = pItem->pos.x - ITEM_WIDTH;
		pVtx[0].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = pItem->pos.x + ITEM_WIDTH;
		pVtx[1].pos.y = pItem->pos.y - ITEM_HEIGHT;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = pItem->pos.x - ITEM_WIDTH;
		pVtx[2].pos.y = pItem->pos.y + ITEM_HEIGHT;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = pItem->pos.x + ITEM_WIDTH;
		pVtx[3].pos.y = pItem->pos.y + ITEM_HEIGHT;
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
		pVtx[0].tex = D3DXVECTOR2(0.125f * pItem->type, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.125f * pItem->type, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();

	AddFunctionLog("END : VertexBuffer Create");
}

//================================================================================================================
// �u���b�N�̏I������
//================================================================================================================
void UninitItem(void)
{
	// �e�N�X�`���̔j��(�K���s�����ƁI�I�I)
	for (int nCntItem = 0; nCntItem < ITEMTYPE_MAX; nCntItem++)
	{
		if (g_pTextureItem != NULL)
		{
			g_pTextureItem->Release();
			g_pTextureItem = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��(�K���s�����ƁI�I�I)
	if (g_pVtxBuffItem != NULL)
	{
		g_pVtxBuffItem->Release();
		g_pVtxBuffItem = NULL;
	}
}

//================================================================================================================
// �u���b�N�̍X�V����
//================================================================================================================
void UpdateItem(void)
{
	VERTEX_2D *pVtx = NULL;
	ITEM *pItem = &g_aItem[0];
	PLAYER *pPlayer = GetPlayer();
	
	D3DXVECTOR3 pos = D3DXVECTOR3_NULL;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			// �����蔻��
			CollisionPlayer(pItem);

			pos = pPlayer->moveposPlayer;

			pItem->col.a += 0.01f * pItem->nAlphaItem;
			if (pItem->col.a >= 1.0f || pItem->col.a <= 0.2f)
			{
				pItem->nAlphaItem *= -1;
			}

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pItem->pos.x + pos.x) - ITEM_WIDTH;
			pVtx[0].pos.y = (pItem->pos.y + pos.y) - ITEM_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pItem->pos.x + pos.x) + ITEM_WIDTH;
			pVtx[1].pos.y = (pItem->pos.y + pos.y) - ITEM_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pItem->pos.x + pos.x) - ITEM_WIDTH;
			pVtx[2].pos.y = (pItem->pos.y + pos.y) + ITEM_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pItem->pos.x + pos.x) + ITEM_WIDTH;
			pVtx[3].pos.y = (pItem->pos.y + pos.y) + ITEM_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			// rhw�̐ݒ�
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pItem->col;
			pVtx[1].col = pItem->col;
			pVtx[2].col = pItem->col;
			pVtx[3].col = pItem->col;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.125f * pItem->type, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * pItem->type, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 1.0f);
		}

		pVtx += 4;				// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffItem->Unlock();
}

//================================================================================================================
// �u���b�N�̕`�揈��
//================================================================================================================
void DrawItem(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			// �f�o�C�X�̃|�C���^
	ITEM *pItem = &g_aItem[0];

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffItem, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		if (pItem->bUse == true)
		{
			// �e�N�X�`���̐ݒ�(�g��Ȃ��Ȃ�NULL������I�I�I�I)
			pDevice->SetTexture(0, g_pTextureItem);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,		// �v���~�e�B�u�̎��
				nCntItem * 4,								// �`�悷��ŏ��̒��_�C���f�b�N�X
				2);											// �`�悷��v���~�e�B�u�̐�
		}
	}
}

//================================================================================================================
// �u���b�N�̐ݒu
//================================================================================================================
void SetItem(ITEMTYPE type, D3DXVECTOR3 pos)
{
	ITEM *pItem = &g_aItem[0];
	PLAYER* pPlayer = GetPlayer();
	D3DXVECTOR3 Camerapos = D3DXVECTOR3_NULL;
	VERTEX_2D* pVtx = NULL;
	int nCntItem = 0;

	AddFunctionLog("START : Item Set");

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffItem->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++,pItem++)
	{
		if (pItem->bUse == false)
		{
			Camerapos = pPlayer->moveposPlayer;

			pItem->pos = pos;
			pItem->col = D3DXCOLOR(1.0f,1.0f,1.0f,0.3f);
			pItem->nAlphaItem = 1;
			pItem->type = type;

			// ���_���W�̐ݒ�(���W�ݒ�͕K���E���ŁI�I�I)
			pVtx[0].pos.x = (pItem->pos.x + Camerapos.x) - ITEM_WIDTH;
			pVtx[0].pos.y = (pItem->pos.y + Camerapos.y) - ITEM_HEIGHT;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = (pItem->pos.x + Camerapos.x) + ITEM_WIDTH;
			pVtx[1].pos.y = (pItem->pos.y + Camerapos.y) - ITEM_HEIGHT;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = (pItem->pos.x + Camerapos.x) - ITEM_WIDTH;
			pVtx[2].pos.y = (pItem->pos.y + Camerapos.y) + ITEM_HEIGHT;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = (pItem->pos.x + Camerapos.x) + ITEM_WIDTH;
			pVtx[3].pos.y = (pItem->pos.y + Camerapos.y) + ITEM_HEIGHT;
			pVtx[3].pos.z = 0.0f;

			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.125f * pItem->type, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * pItem->type, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * pItem->type + 0.125f, 1.0f);

			// ���_�o�b�t�@���A�����b�N����
			g_pVtxBuffItem->Unlock();

			pItem->bUse = true;

			break;
		}

		pVtx += 4;
	}
}

//================================================================================================================
// �u���b�N�̏���(�S��)
//================================================================================================================
void DestroyItem(void)
{
	ITEM *pItem = &g_aItem[0];

	for (int nCntItem = 0; nCntItem < MAX_ITEM; nCntItem++, pItem++)
	{
		if (pItem->bUse == true)
		{
			pItem->bUse = false;
		}
	}
}

//================================================================================================================
// �u���b�N�̓����蔻��(Player)
//================================================================================================================
void CollisionPlayer(ITEM* pItem)
{
	PLAYER* pPlayer = GetPlayer();

	if (pPlayer->posPlayer.x >= pItem->pos.x - ITEM_WIDTH - (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.x <= pItem->pos.x + ITEM_WIDTH + (PLAYER_SIZE * 0.25f)
		&& pPlayer->posPlayer.y >= pItem->pos.y - ITEM_HEIGHT - (PLAYER_SIZE * 0.5f)
		&& pPlayer->posPlayer.y <= pItem->pos.y + ITEM_HEIGHT + (PLAYER_SIZE * 0.5f))
	{
		switch (pItem->type)
		{
		case ITEMTYPE_COIN:
			// �R�C��
  			AddScore(1000);

			PlaySound(SOUND_LABEL_SE_GETCOIN);

			pItem->bUse = false;

			break;

		case ITEMTYPE_HEAL:

			PlaySound(SOUND_LABEL_SE_HEAL);
			
			if (pPlayer->nLife < MAX_LIFE)
			{	// �q�[��
				AddHeart(1);

				pPlayer->nLife++;
			}

			pItem->bUse = false;

			break;

		case ITEMTYPE_HOMING:

			ChangeModeFrame(FRAMESTATE_POWER_ITEM);

			PlaySound(SOUND_LABEL_SE_POWER_CHANGE);

			SetPlayerShotType(SHOTTYPE_HOMING);

			pItem->bUse = false;

			break;

		case ITEMTYPE_BOMB:

			ChangeModeFrame(FRAMESTATE_POWER_ITEM);

			PlaySound(SOUND_LABEL_SE_POWER_CHANGE);

			SetPlayerShotType(SHOTTYPE_BOMB);

			pItem->bUse = false;

			break;

		case ITEMTYPE_LASER:

			ChangeModeFrame(FRAMESTATE_POWER_ITEM);

			PlaySound(SOUND_LABEL_SE_POWER_CHANGE);

			SetPlayerShotType(SHOTTYPE_LASER);

			pItem->bUse = false;

			break;

		case ITEMTYPE_KEY:

			pPlayer->bHaveKey = true;
			pItem->bUse = false;
			SetEnableHaveKeyExac(true);

			PlaySound(SOUND_LABEL_SE_GETKEY);

			break;

		case ITEMTYPE_ULTIMATE_COIN:

			// �X�[�p�[�A���e�B���b�g�R�C��
			AddScore(100000);

			PlaySound(SOUND_LABEL_SE_GETULTIMATECOIN);

			pItem->bUse = false;

			break;

		case ITEMTYPE_SECRET:

			PlaySound(SOUND_LABEL_SE_GETULTIMATECOIN);

			SetEnableHaveSecret(true);

			pItem->bUse = false;

			break;
		}
	}
}