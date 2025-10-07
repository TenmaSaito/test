//================================================================================================================
//
// DirectX�̃v���C���[�w�b�_�t�@�C�� [player.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "bullet.h"

// �}�N����`
#define PLAYER_SIZE				(100.0f)		// �v���C���[�̒��S�����X���̋���
#define PLAYER_SPOWN_X	(SCREEN_WIDTH * 0.5f)		// �v���C���[�̏����ʒu(X)
#define PLAYER_SPOWN_Y	(SCREEN_HEIGHT * 0.5f)		// �v���C���[�̏����ʒu(Y)
#define MAX_LIFE		(5)							// �v���C���[�̗̑͂̍ő�l
#define MAX_STOCK		(3)							// �v���C���[�̎c�@�̍ő�l

// �v���C���[�̌����̗񋓌^�̒�`
typedef enum
{
	PLAYERDIRECTION_UP = 0,			// �����		
	PLAYERDIRECTION_DOWN,			// ������
	PLAYERDIRECTION_LEFT,			// ������
	PLAYERDIRECTION_RIGHT,			// �E����
	PLAYERDIRECTION_UPLEFT,			// �������
	PLAYERDIRECTION_UPRIGHT,		// �E�����
	PLAYERDIRECTION_DOWNLEFT,		// �E������
	PLAYERDIRECTION_DOWNRIGHT,		// ��������
	PLAYERDIRECTION_MAX				// �����̑���
}PLAYERDIRECTION;

// �����̕␳�p�񋓌^
typedef enum
{
	PLAYERTEX_LEFT = 0,				// �����	
	PLAYERTEX_RIGHT,				// ������
	PLAYERTEX_UP,					// ������
	PLAYERTEX_DOWN,					// �E����
	PLAYERTEX_LEFT_ATK,				// ������̍U��
	PLAYERTEX_RIGHT_ATK,			// ������̍U��
	PLAYERTEX_UP_ATK,				// �������̍U��
	PLAYERTEX_DOWN_ATK,				// �������̍U��
	PLAYERTEX_MAX					
}PLAYERTEX;

// �v���C���[�̏��
typedef enum
{
	PLAYERSTATE_APPEAR = 0,		// �o�����(�������G����)
	PLAYERSTATE_NORMAL,			// �ʏ�
	PLAYERSTATE_DAMAGE,			// �q�b�g��
	PLAYERSTATE_WAIT,			// �o���҂����(��\��)
	PLAYERSTATE_DEATH,			// ���S��
	PLAYERSTATE_BARRIER,		// �o���A
	PLAYERSTATE_UNMOVE,			// ���[�U�[�r�[�����ˎ��̑���s�\����
	PLAYERSTATE_TELEPORT_COOLDOWN,	//�X�e�[�W�ړ���̈ꎞ�ҋ@����
	PLAYERSTATE_MAX
}PLAYERSTATE;

// �v���C���[�̑���\��Ԃ̗񋓌^��`
typedef enum
{
	PLAYERDO_MOVE = 0,			// ����
	PLAYERDO_SHOT,				// �ˌ�
	PLAYERDO_CHARGE,			// �`���[�W
	PLAYERDO_SP,				// ����Z
	PLAYERDO_BARRIAR,			// �o���A
	PLAYERDO_MAX
}PLAYERDO;

// �v���C���[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 posPlayer;							// �v���C���[�̈ʒu(bg,enemy�̈ʒu)
	D3DXVECTOR3 movePlayer;							// �v���C���[�̈ړ���
	D3DXVECTOR3 moveposPlayer;						// �v���C���[�̉ߋ��̈ʒu����ړ�������(bg,enemy�̈ړ���)
	PLAYERDIRECTION rotPlayer;						// �v���C���[�̌���(��/��/��/�E)
	PLAYERTEX tex;
	PLAYERSTATE state;								// �v���C���[�̏��
	SHOTTYPE type;									// �`���[�W�Z�̃^�C�v
	float fBulletSpeed;								// �`���[�W�Z�̑��x
	float fLengthPlayer;							// �Ίp���̒���
	float fAnglePlayer;								// �Ίp���̊p�x
	int nBulletLife;								// �e�̗̑�
	int nCounterState;								// ��Ԏ�������
	int nLife;										// �v���C���[�̗̑�
	int nStock;										// �v���C���[�̎c�@
	int nCounterAnimPlayer;							// �A�j���[�V�����J�E���^�[
	int nPatternAnimPlayer;							// �A�j���[�V�����p�^�[��No.
	int nCounterBullPlayer;							// �e�̘A�ˑ��x
	int nCounterBulletCharge;						// �e�̃`���[�W����
	int nGaugeNo;									// �Q�[�W�̔ԍ�
	int nCounterParticle;							// �`���[�W�V���b�g��̃p�[�e�B�N���̔����������鎞��
	int nCounterBarrier;							// �o���A�̃`���[�W����
	int nCounterBarrierTime;						// �o���A�̎�������
	int nBarrierNo;									// �o���A�̃Q�[�W�̔ԍ�
	int nCounterCoolTime;							// �o���A�̓W�J���̃N�[���^�C��
	int aBlockHealthNumber[MAX_LIFE];				// �n�[�g�u���b�N�̕Ԃ�l��ۊǂ��A�Ǘ�����ϐ�
	int nBlockStockID;								// �c�@�̃u���b�N��ID
	int g_nIDDirection;								// �ˌ��̌����������u���b�N��ID
	bool bDisp;										// �\�����邩���Ȃ���
	bool aCouldDo[PLAYERDO_MAX];					// ���͂������삪�󂯕t�����邩
	bool bCharge;									// �`���[�W����������
	bool bHaveKey;									// ���������Ă��邩
}PLAYER;
		
// �v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
PLAYER *GetPlayer(void);
void HitPlayer(int nCntDamage);
void SetPlayerDo(PLAYERDO playerDo, bool bCould);

float GetPlayerRot(PLAYER* pPlayer);
PLAYERDIRECTION GetPlayerDirection(void);
void SetPlayerShotType(SHOTTYPE type);
void SetEnablePlayerFullburst(bool bUse);

#endif