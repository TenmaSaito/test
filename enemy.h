//================================================================================================================
//
// DirectX�̓G�w�b�_�[�t�@�C�� [enemy.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"

// �}�N����`
#define MAX_ENEMY		(64)	// �G�̍ő吔
#define ENEMY_SIZE		(100)	// �G�̃T�C�Y

// �G�̏��
typedef enum
{
	ENEMYSTATE_NORMAL = 0,		// �ʏ���
	ENEMYSTATE_DAMAGE,			// �_���[�W���
	ENEMYSTATE_APPEAR,			// �o�����
	ENEMYSTATE_DEATH,			// ���S���
	ENEMYSTATE_DOWN,			// �_�E�����
	ENEMYSTATE_MAX				// ��Ԃ̑���
}ENEMYSTATE;

// �G�̎��
typedef enum
{
	ENEMYTYPE_FIX = 0,			// �Œ�^�̓G
	ENEMYTYPE_HOMING,			// �v���C���[�ǔ��^�̓G
	ENEMYTYPE_MOVE,				// �f�r���G
	ENEMYTYPE_BOSS,				// ���X�{�X
	ENEMYTYPE_MAX
}ENEMYTYPE;

// �G�̒e���˂̎��
typedef enum
{
	ENEMYBULLET_NONE = 0,			// �e�𔭎˂��Ȃ�
	ENEMYBULLET_NORMALBULLET,		// �ʏ�e�𔭎˂���
	ENEMYBULLET_BOMBBULLET,			// �{���𔭎˂���
	ENEMYBULLET_HOMINGBULLET,		// �z�[�~���O�e�𔭎˂���
	ENEMYBULLET_BOSS,				// �{�X��p
	ENEMYBULLET_MAX
}ENEMYBULLET;

// �G�̃e�N�X�`���̎��
typedef enum
{
	ENEMYTEX_SLIME = 0,				// �X���C��
	ENEMYTEX_WATER_ELEMENTS,		// ���̐���
	ENEMYTEX_FIRE_ELEMENTS,			// ���̐���
	ENEMYTEX_TREE_ELEMENTS,			// �؂̐���
	ENEMYTEX_ICE_ELEMENTS,			// �X�̐���
	ENEMYTEX_BOSS,					// �{�X
	ENEMYTEX_MAX
}ENEMYTEX;

// �G�̃t�F�[�Y�̎��
typedef enum
{
	ENEMYPHASE_1 = 0,			// �t�F�[�Y1
	ENEMYPHASE_2,				// �t�F�[�Y2
	ENEMYPHASE_3,				// �t�F�[�Y3
	ENEMYPHASE_MAX
}ENEMYPHASE;

// �G�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			// �G�̈ʒu
	D3DXVECTOR3 move;			// �G�̈ړ���
	float fLength;				// �G�̈ړ��̊p�x
	POLY_SIZE size;				// �G�̑傫��
	float fLengthEnemy;			// �G�̑Ίp���̒���
	float fAngleEnemy;			// �G�̑Ίp���̊p�x
	ENEMYTYPE type;				// �G�̎��
	ENEMYBULLET bullet;			// �e�̎��
	ENEMYTEX tex;				// �G�̃e�N�X�`���̎��
	int nLife;					// �G�̗̑�
	ENEMYSTATE state;			// �G�̏��
	ENEMYPHASE phaseExac;		// ���݂̃t�F�[�Y
	ENEMYPHASE phaseMax;		// �t�F�[�Y�̍ő吔
	int nCounterState;			// ��ԃJ�E���^�[(��Ԃ̌o�ߕb��)
	int nCounterBullet;			// �e�𔭎˂���Ԋu
	int nBulletRand;			// �e�̔��ˊԊu�̂��炵
	bool bUse;					// �g�p����Ă��邩
} ENEMY, *PENEMY;

// �v���g�^�C�v�錾�@
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void DrawEnemy(void);

/// @brief �t�@�C�����G�̔z�u���y�яo���^�[����ǂݍ��݁A�����̃^�[���Ɋ�Â��z�u���܂��B
/// @param pFileName �t�@�C����
/// @param nTurn �z�u�^�[��
/// @return ����
void LoadEnemy(char* pFileName, int nTurn);

/// @brief �������󂫂̂���G�\���̂ɑ�����A������ꂽ��������ɓG��L�������܂��B
/// @param pos �z�u���W
/// @param move �ړ���
/// @param size �G�̑傫��(x = ����: y = �c��)
/// @param type �G�̎��
/// @param bullet ���˂���e�̎��
/// @param tex �G�̃e�N�X�`��
/// @param nLife �G�̗̑�
/// @param nCounterBullet �G�̒e��̔��ˊԊu
/// @return ����
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 move, POLY_SIZE size, ENEMYTYPE type, ENEMYBULLET bullet, ENEMYTEX tex,  int nLife, int nCounterBullet);

/// @brief �G�\���̂̐擪�A�h���X���擾���܂��B
/// @return �G�\���̂̐擪�A�h���X�̃|�C���^
ENEMY *GetEnemy(void);

/// <summary>�G�\���̂̃A�h���X���w�肵�A�������̃_���[�W��^���܂��B</summary>
/// <param name="nCntEnemy">�G�\���̂̔ԍ�</param>
/// <param name="nCntDamage">�^����_���[�W</param>
void HitEnemy(int nCntEnemy, int nCntDamage);

/// <summary>���ݑ��݂��Ă���G�̑������擾���܂��B</summary>
/// <returns>���݂̓G�̑���</returns>
int GetTotalEnemy(void);

/// <summary>�֐����Ăяo�����܂łɃv���C���[�ɂ���ē|���ꂽ�G�̑������擾���܂��B</summary>
/// <returns>�|���ꂽ�̓G�̑���</returns>
int GetKillcountEnemy(void);

//***************************************************************************************
// �f�o�b�O�p�v���g�^�C�v�錾
//***************************************************************************************
void DestroyEnemy(void);

#endif // !_Enemy_H_
