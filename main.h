//================================================================================================================
//
// DirectX�̃��C���w�b�_�t�@�C�� [main.h]
// Author : TENMA
//
//================================================================================================================
#ifndef _MAIN_H_				// ���̃}�N����`������Ă��Ȃ����
#define _MAIN_H_				// 2�d�C���N���[�h�h�~�̃}�N�����`

#include <windows.h>
#include "d3dx9.h"							// �`�揈���ɕK�v
#define DIRECTINPUT_VERSION		(0x0800)	// �r���h���̌x���Ώ��p�}�N��
#include "dinput.h"							// ���͏����ɕK�v(��ɏ�̃}�N�����`����)
#include "xaudio2.h"						// �T�E���h�����ɕK�v
#include "xinput.h"							// �W���C�p�b�h�����ɕK�v

// ���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")				// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")			// d3d9.lib�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")			// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")			// �V�X�e���̎����擾�ɕK�v
#pragma comment(lib,"dinput8.lib")			// ���͏����ɕK�v
#pragma comment(lib,"xinput.lib")			// �W���C�p�b�h�����ɕK�v

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

// �}�N����`
#define SCREEN_WIDTH	(1280)						// �E�B���h�E�̕�
#define SCREEN_HEIGHT	(720)						// �E�B���h�E�̍���
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)		// ���_�t�H�[�}�b�g
#define D3DXVECTOR3_NULL	D3DXVECTOR3(0.0f,0.0f,0.0f)						// D3DXVECTOR3��NULL
#define D3DXCOLOR_NULL		D3DXCOLOR(1.0f,1.0f,1.0f,1.0f)					// COLOR��NULL
#define STRING_MAX		(1024)						// ������̍ő啶����
#define WINDOW_MID		D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f)	// �E�B���h�E�̒��S
#define SWAP(a,b)		a^=b^=a^=b

// ��ʃ��[�h�̎��
typedef enum
{
	MODE_TITLE = 0,			// �^�C�g�����
	MODE_TUTORIAL,			// �`���[�g���A���t���Q�[�����
	MODE_GAME,				// �Q�[�����
	MODE_RESULT,			// ���U���g���
	MODE_GAMEOVER,			// �Q�[���I�[�o�[���
	MODE_GAMECLEAR,			// �Q�[���N���A���
	MODE_CREDIT,			// �N���W�b�g���
	MODE_MAX
}MODE;

// �Q�[���̓�Փx
typedef enum
{
	GAMEDIFFICULTY_EASY = 0,		// EASY
	GAMEDIFFICULTY_NORMAL,			// NORMAL
	GAMEDIFFICULTY_HARD,			// HARD
	GAMEDIFFICULTY_MAX
}GAMEDIFFICULTY;

// ���_���(2D)�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		// ���_���(x,y,z)
	float rhw;				// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;			// ���_�J���[(R,G,B,a)
	D3DXVECTOR2 tex;		// �e�N�X�`�����W(x,y)
} VERTEX_2D;

// @brief �|���S���̃T�C�Y�w��\���� 
// @param x : ����
// @param y : �c��
// @param hx : �����̔���
// @param hy : �c���̔���
typedef struct POLY_SIZE
{
	float x;
	float y;
	float hx;
	float hy;

	POLY_SIZE() : x(0.0f), y(0.0f), hx(0.0f), hy(0.0f) {};
	POLY_SIZE(float px, float py) : x(px), y(py), hx(px * 0.5f), hy(py * 0.5f) {};
}POLY_SIZE, *LPPOLY_SIZE;

// �v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
MODE GetModeExac(void);
HRESULT GetHandleWindow(HWND *phWnd);
void SetGameDifficulty(GAMEDIFFICULTY difficulty);
GAMEDIFFICULTY GetGameDifficulty(void);

#endif