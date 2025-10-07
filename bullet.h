//================================================================================================================
//
// DirectX�̒e�̔��˃w�b�_�[�t�@�C�� [bullet.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

// �}�N����`
#define MAX_BULLET		(512)								// �e�̍ő吔
#define BULLET_SPD		(-50.0f)							// �e�̑��x
#define PLAYER_BULLET_SIZE		(30.0f)						// �e�̃T�C�Y
#define ENEMY_BULLET_SIZE		(20.0f)						// �e�̃T�C�Y
#define BULLET_COUNT	(100)								// �e�𔭎˂����b�Ԋu

#define HOMING_SPD			(-15.0f)						// �z�[�~���O�̑���
#define HOMINGBULLET_COUNT	(150)							// �e�𔭎˂����b�Ԋu
#define BOMB_SPD			(-5.0f)							// �{���̑��x
#define BOMBBULLET_COUNT	(200)							// �e�𔭎˂����b�Ԋu
#define LASER_SPD			(0.0f)							// ���[�U�[�n�_�̑���

#define HOMING_LIFE		(600)								// �z�[�~���O�̗̑�
#define BOMB_LIFE		(100)								// �{���̗̑�
#define LASER_LIFE		(200)								// ���[�U�[�̗̑�

#define FULL_BURST_LIFE	(1)									// �t���o�[�X�g�̗̑�
#define FULL_BURST_SPD	(0)									// ���x

// �e�̎��
typedef enum
{	
	BULLETTYPE_PLAYER = 0,			// �v���C���[�̒e
	BULLETTYPE_ENEMY_1,				// �G1(�G)�̒e
	BULLETTYPE_MAX
}BULLETTYPE;

// �ˌ��̎��
typedef enum
{
	SHOTTYPE_NORMAL = 0,			// �꒼���ɔ��
	SHOTTYPE_CHARGE,				// �`���[�W�V���b�g(�̗͂������ђʒe�����_���[�W)
	SHOTTYPE_HOMING,				// �z�[�~���O����e
	SHOTTYPE_BOMB,					// ��莞�ԂŔ������A�e���g�U���锚�e
	SHOTTYPE_BOMBBULLET,			// �{���ɂ���Ċg�U����e
	SHOTTYPE_LASER,					// ���[�U�[�r�[���𔭎˂���n�_��ݒ肷��
	SHOTTYPE_LASERBULLET,			// ���[�U�[�r�[���ɂ���Ĕ��˂��ꂽ�ђʒe
	SHOTTYPE_FULLBURST,				// �S�e���ˁI�I�I
	SHOTTYPE_MAX
}SHOTTYPE;

// �e�̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			// �e�̈ʒu
	D3DXVECTOR3 move;			// �e�̈ړ���
	int nLife;					// �e�̎���
	BULLETTYPE type;			// ���
	SHOTTYPE shot;				// ���˂̎��
	D3DXCOLOR col;				// �e�̐F
	float fLength;				// �Ίp���̑傫��
	float fAngle;				// �Ίp���̊p�x
	int nEnemy;					// �z�[�~���O���̓G�̈ʒu
	int nCounterHoming;			// �z�[�~���O�e�̏�������
	int nCounterHomingtime;		// �z�[�~���O���鎞��
	bool bEffect;				// �G�t�F�N�g���g�p���邩�ǂ���
	bool bUse;					// �e���g�p���Ă��邩�ǂ���
}BULLET;

// �v���g�^�C�v�錾�@
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);

void SetBullet(D3DXVECTOR3 pos, float speed, float rot, int nLife, BULLETTYPE type, SHOTTYPE shot, D3DXCOLOR col,bool bEffect);
BULLET* GetBullet(void);
int GetBulletAll(void);

void BombBullet(void);

#endif // !_BULLET_H_
