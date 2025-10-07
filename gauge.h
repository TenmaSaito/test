//================================================================================================================
//
// DirectX�̃Q�[�W�w�b�_�[�t�@�C�� [gauge.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"

// �}�N����`
#define GAUGE_VERTEX	(152)				// �Q�[�W�̒��_��
#define GAUGE_MAX		(GAUGE_VERTEX - 2)	// �Q�[�W�̍ő�l
#define NOMORE_GAUGE	(-1)				// �g����Q�[�W�������ꍇ�ɕԂ�l
		
// �Q�[�W�̎��
typedef enum
{
	GAUGETYPE_HOMING = 0,			// �z�[�~���O�V���b�g
	GAUGETYPE_CHARGE,				// �`���[�W�V���b�g
	GAUGETYPE_BARRIER,				// �o���A
	GAUGETYPE_TIMER,				// �^�C�}�[
	GAUGETYPE_MAX
}GAUGETYPE;

// �Q�[�W�\����
typedef struct
{
	D3DXVECTOR3 pos;				// �Q�[�W�̈ʒu
	D3DXCOLOR col;					// �Q�[�W�̐F
	GAUGETYPE type;					// �Q�[�W�̎��
	int nPercentGauge;				// �Q�[�W�̊���(���݂̃Q�[�W)
	float fLength;					// �Q�[�W�̔��a
	bool bUse;						// �Q�[�W���g���Ă��邩
}Gauge;

// �v���g�^�C�v�錾
void InitGauge(void);
void UninitGauge(void);
void UpdateGauge(void);
void DrawGauge(void);

int SetGauge(D3DXVECTOR3 pos, D3DXCOLOR col, GAUGETYPE type, int start, float fLength);
Gauge *GetGauge(void);
void AddGauge(int value,int nNo);

#endif // !_GAUGE_H_
