//================================================================================================================
//
// DirectX�̃��C������ [main.cpp]
// Author : TENMA
//
//================================================================================================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "title.h"
#include "game.h"
#include "result.h"
#include "enemy.h"
#include "player.h"
#include "fade.h"
#include "gauge.h"
#include "gameover.h"
#include "gameclear.h"
#include "credit.h"
#include "block.h"
#include "bullet.h"
#include "resource.h"
#include <crtdbg.h>

// �}�N����`
#define CLASS_NAME		"WindowClass"				// �E�B���h�E�N���X�̖��O
#define WINDOW_NAME		"MapMaker"					// �L���v�V�����ɕ\������閼�O(��o����O�ɕς��邱�ƁI�I�I)

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawDebug(void);
void ToggleFullscreen(HWND hWnd);

// �O���[�o���ϐ�
LPDIRECT3D9				g_pD3D = NULL;					// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9		g_pD3DDevice = NULL;			// Direct3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_TITLE;								// ���݂̉��
MODE g_modeExac = MODE_TITLE;							// �ЂƂO�̉ߋ��̉��
LPD3DXFONT g_pFont = NULL;								// �t�H���g�ւ̃|�C���^
HWND g_hWnd = NULL;										// �l�������E�B���h�E�n���h��
int g_nCountFPS = 0;									// FPS�J�E���^
bool g_isFullscreen = false;							// �t���X�N���[���̎g�p��
RECT g_windowRect;										// �E�B���h�E�T�C�Y
GAMEDIFFICULTY g_Difficulty = GAMEDIFFICULTY_NORMAL;	// �Q�[���̓�Փx

//================================================================================================================
// ���C���֐�
//================================================================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{

#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);			// ���������[�N���m�p�̃t���O
#endif // _DEBUG

	HWND hWnd;									// �E�B���h�E�n���h��
	MSG msg;									// ���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;						// ���ݎ���
	DWORD dwExecLastTime;						// �Ō�ɏ�����������
	DWORD dwFrameCount;							// �t���[���J�E���g
	DWORD dwFPSLastTime;						// �Ō��FPS���v����������
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ���O�̏���������
	InitLog();

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),						// �E�B���h�E�N���X�̃������T�C�Y
		CS_CLASSDC,								// �E�B���h�E�̃X�^�C��
		WindowProc,								// �E�B���h�E�v���V�[�W��
		0,										// 0
		0,										// 0
		hInstance,								// �C���X�^���X�n���h��
		LoadIcon(hInstance,(LPCSTR)IDI_ICON2),	// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),				// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),				// �N���C�A���g�̈�̔w�i�F
		NULL,									// ���j���[�o�[
		CLASS_NAME,								// �E�B���h�E�N���X�̖��O
		LoadIcon(hInstance,(LPCSTR)IDI_ICON2)	// �t�@�C���̃A�C�R��
	}; 

	AddFunctionLog("END : WindowClass Setting");

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	AddFunctionLog("END : WindowClass Register");

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	AddFunctionLog("END : WindowRect Setting");

	// �E�B���h�E�̐���
	hWnd = CreateWindowEx(
		0,										// �g���E�B���h�E�X�^�C��
		CLASS_NAME,								// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,							// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,					// �E�B���h�E�X�^�C��
		CW_USEDEFAULT,							// �E�B���h�E�̍���X���W
		CW_USEDEFAULT,							// �E�B���h�E�̍���Y���W
		(rect.right - rect.left),				// �E�B���h�E�̕�
		(rect.bottom - rect.top),				// �E�B���h�E�̍���
		NULL,									// �e�E�B���h�E�̃n���h��
		NULL,									// ���j���[(�������͎q�E�B���h�E)�n���h��
		hInstance,								// �C���X�^���X�n���h��
		NULL);									// �E�B���h�E�쐬�f�[�^

	AddFunctionLog("END : Window Create");

	if(hWnd == NULL)
	{
		MessageBox(hWnd, "�E�B���h�E�n���h�����m�ۂ���Ă��܂���!", "�x���I", MB_ICONWARNING);
		return -1;
	}

	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// ���������������s�����ꍇ
		return -1;
	}

	AddFunctionLog("CLEAR : DirectX Init");

	//����\��ݒ�
	timeBeginPeriod(1);

	AddFunctionLog("END : timeBegin Setting");

	dwCurrentTime = 0;							// ������
	dwExecLastTime = timeGetTime();				// ���ݎ������擾
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);					// �E�B���h�E�̕\����Ԃ�ݒ�

	AddFunctionLog("END : Window Show");

	UpdateWindow(hWnd);							// �N���C�A���g�̈���X�V

	AddFunctionLog("END : Window Update");
	
	g_hWnd = hWnd;

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0,PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{// WM_QUIT���b�Z�[�W���󂯂�ƁA���b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);					// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�

				DispatchMessage(&msg);					// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();				// ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;			// FPS���v�������������擾
				dwFrameCount = 0;						// �t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��

				dwExecLastTime = dwCurrentTime;			//�����J�n����[���ݎ���]��ۑ�

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;							// �t���[���J�E���g�����Z
			}
		}
		
	}

	// �I������
	Uninit();

	AddFunctionLog("END : DirextX Uninit");

	// �E�B���h�E�N���X�̓o�^����
	UnregisterClass(CLASS_NAME, wcex.hInstance); 

	AddFunctionLog("END : WindowClass Unregister");

	// ���O�̏I������
	UninitLog();

	return (int)msg.wParam;
}

//================================================================================================================
// �E�B���h�E�v���V�[�W��
//================================================================================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_DESTROY:		// �E�B���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	// �L�[�����̃��b�Z�[�W
	case WM_KEYDOWN:
		// �����ꂽ�L�[�̔���
		switch (wParam)
		{
		case VK_ESCAPE:
			// �I���m�F
			nID = MessageBox(hWnd, "�I�����܂����H", "�I���m�F���b�Z�[�W", (MB_YESNO | MB_ICONINFORMATION));
			if (nID == IDYES)
			{// ����YES�������ꍇ
				// �E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}

			break;

		case VK_F11:
			ToggleFullscreen(hWnd);// F11�Ńt���X�N���[��
			break;
		}
		break;

	case WM_CLOSE:		// ����{�^�������̃��b�Z�[�W
		
		// �E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
		DestroyWindow(hWnd);
		
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//================================================================================================================
//����������
//================================================================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^		

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Direct3DObject Create");

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,
		&d3ddm)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : DisplayMode Get");

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));			// �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;		// �Q�[���̉�ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;		// �Q�[���̉�ʃT�C�Y(����)
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;		// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;					// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �_�u���o�b�t�@�̐؂�ւ�(�f���M���Ɠ���)
	d3dpp.EnableAutoDepthStencil = TRUE;		// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;	// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;					// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;			// �C���^�[�o��

	// Direct3D�f�o�C�X�̍쐬
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
			else
			{
				AddFunctionLog("CLEAR : Device->SOFTWARE Create");
			}
		}
		else
		{
			AddFunctionLog("CLEAR : Device->SOFTWARE HALF Create");
		}
	}
	else
	{
		AddFunctionLog("CLEAR : Device->HARDWARE Create");
	}

	// �����_�[�X�e�[�g�̐ݒ�(�����Ȃ����ƁIALPHA�l�̐ݒ��K�p����ׂ̐ݒ�I)
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���̊g�k��Ԃ̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// �e�N�X�`���̌J��Ԃ��̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�(�e�N�X�`���̃A���t�@�u�����h�̐ݒ�[�e�N�X�`���ƃ|���S����ALPHA�l��������I])
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	// �f�o�b�O�p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice,
		30,
		0,
		0,
		0,
		FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"PixelMplus12",
		&g_pFont);

	AddFunctionLog("END : D3DXCreateFont Create");

	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Keyboard Init");

	// �W���C�p�b�h�̏���������
	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Joypad Init");

	// �}�E�X�̏���������
	if (FAILED(InitMouse(hWnd)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Mouse Init");

	// �T�E���h�̏���������
	if (FAILED(InitSound(hWnd)))
	{
		return E_FAIL;
	}

	AddFunctionLog("CLEAR : Sound Init");

	SetMode(g_mode);

	AddFunctionLog("END : Mode Set");

	InitFade(g_mode);

	AddFunctionLog("END : Fade Init");

	SetEnablePlayerFullburst(false);

	return S_OK;
}

//================================================================================================================
// �I������
//================================================================================================================
void Uninit(void)
{
	// �^�C�g����ʂ̏I������
	UninitTitle();

	AddFunctionLog("END : Title Uninit");

	// �Q�[����ʂ̏I������
	UninitGame();

	AddFunctionLog("END : Game Uninit");

	// ���U���g��ʂ̏I������
	UninitResult();

	AddFunctionLog("END : Result Uninit");

	// �Q�[���I�[�o�[��ʂ̏I������
	UninitGameover();

	AddFunctionLog("END : Gameover Uninit");

	// �t�F�[�h�̏I������
	UninitFade();

	AddFunctionLog("END : Fade Uninit");

	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	AddFunctionLog("END : Keyboard Uninit");

	// �W���C�p�b�h�̏I������
	UninitJoypad();

	AddFunctionLog("END : Joypad Uninit");

	// �}�E�X�̏I������
	UninitMouse();

	AddFunctionLog("END : Mouse Uninit");

	// �f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;

		AddFunctionLog("END : Font Release");
	}

	// �T�E���h�̏I������
	UninitSound();

	AddFunctionLog("END : Sound Uninit");

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();

		g_pD3DDevice = NULL;

		AddFunctionLog("END : DirectXDevice Release");
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();

		g_pD3D = NULL;

		AddFunctionLog("END : DirextXObject Release");
	}
}

//================================================================================================================
// �X�V����
//================================================================================================================
void Update(void)
{
	//�L�[�{�[�h�̍X�V����(�������ɏ�����)
	UpdateKeyboard();

	//�W���C�p�b�h�̍X�V����
	UpdateJoypad();

	// �}�E�X�̍X�V����
	UpdateMouse();

	// ���݂̉��(���[�h)�̍X�V����
	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();

		break;

	case MODE_GAME:
		UpdateGame();

		break;

	case MODE_RESULT:
		UpdateResult();

		break;

	case MODE_GAMEOVER:
		UpdateGameover();

		break;

	case MODE_GAMECLEAR:
		UpdateGameclear();

		break;

	case MODE_CREDIT:
		UpdateCredit();

		break;
	}

	// �t�F�[�h�̍X�V����
	UpdateFade();

	// �T�E���h�̍X�V����
	UpdateSound();

	// ���O�̍X�V����
	UpdateLog();
}

//================================================================================================================
// �`�揈��
//================================================================================================================
void Draw(void)
{
	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// �`��J�n�����������ꍇ

		// ���݂̉��(���[�h)�̕`�揈��
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;

		case MODE_GAME:
			DrawGame();
			break;

		case MODE_RESULT:
			DrawResult();
			break;

		case MODE_GAMEOVER:
			DrawGameover();
			break;

		case MODE_GAMECLEAR:
			DrawGameclear();
			break;

		case MODE_CREDIT:
			DrawCredit();
			break;
		}

		// �t�F�[�h�̕`�揈��
		DrawFade();

#ifdef _DEBUG

		// �f�o�b�O�\��
		DrawDebug();

#endif // _DEBUG

		//DrawTutorial();

		// ���O�̏���������
		DrawLog();

		// �`��I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
   	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

// �f�o�C�X�̎擾
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

// ��ʂ̐ݒ�
void SetMode(MODE mode)
{
	// ���݂̉��(���[�h)�̏I��
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		AddFunctionLog("END : Title Uninit");
		break;

	case MODE_GAME:
		UninitGame();
		AddFunctionLog("END : Game Uninit");
		break;

	case MODE_RESULT:
		UninitResult();
		AddFunctionLog("END : Result Uninit");
		break;

	case MODE_GAMEOVER:
		UninitGameover();
		AddFunctionLog("END : Gameover Uninit");
		break;

	case MODE_GAMECLEAR:
		UninitGameclear();
		break;

	case MODE_CREDIT:
		UninitCredit();
		break;
	}

	// ���݂̃��[�h��ۑ�
	g_modeExac = g_mode;

	// �w��̉��(���[�h)�̏���������
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		AddFunctionLog("END : Title Init");
		break;

	case MODE_GAME:
		InitGame(g_Difficulty);
		AddFunctionLog("END : Game Init");
		break;

	case MODE_RESULT:
		InitResult();
		AddFunctionLog("END : Result Init");
		break;

	case MODE_GAMEOVER:
		InitGameover();
		AddFunctionLog("END : Gameover Init");
		break;

	case MODE_GAMECLEAR:
		InitGameclear();
		break;

	case MODE_CREDIT:
		InitCredit();
		break;
	}

	AddFunctionLog("END : Mode Set");

	// ���[�h��ۑ�
	g_mode = mode;

	AddFunctionLog("END : Mode Save");
}

// ���݂̃��[�h���擾
MODE GetMode(void)
{
	AddFunctionLog("END : Mode Get");

	return g_mode;
}

// ���O�̃��[�h���擾
MODE GetModeExac(void)
{
	return g_modeExac;
}

// �l���ς݃E�B���h�E�̎擾
HRESULT GetHandleWindow(HWND* phWnd)
{
	if (g_hWnd != NULL)
	{
		*phWnd = g_hWnd;
		return S_OK;
	}
	else
	{
		MessageBox(g_hWnd, "�E�B���h�E�n���h�����m�ۂ���Ă��܂���!", "�x���I", MB_ICONWARNING);
		return E_FAIL;
	}
}

// �f�o�b�O�\��
void DrawDebug(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };			// ��ʃT�C�Y
	char aStr[16][256];										// ��ʂɕ\�����镶����
	int nCntEnemy = GetTotalEnemy();						// �G�̑���
	PLAYER* pPlayer = GetPlayer();							// �v���C���[�̏��
	XINPUT_VIBRATION *pVibration = GetJoyVibration();		// �o�C�u���[�V�����̏��
	XINPUT_STATE *pState = GetJoypadState();				// �W���C�p�b�h�̏��
	Gauge *pGauge = GetGauge();								// �Q�[�W�̏��
	POINT pos = GetMousePos();								// �}�E�X�̈ʒu���
	int nBullet = GetBulletAll();

	// ������ɑ��
	wsprintf(&aStr[0][0], "FPS:%d\n", g_nCountFPS);

	wsprintf(&aStr[1][0], "EnemyAll:%d\n", nCntEnemy);

	wsprintf(&aStr[2][0], "PlayerPos:%d / %d\n", (int)pPlayer->posPlayer.x, (int)pPlayer->posPlayer.y);

	wsprintf(&aStr[3][0], "Left:%d\n", pVibration->wLeftMotorSpeed);

	wsprintf(&aStr[4][0], "Right:%d\n", pVibration->wRightMotorSpeed);

	wsprintf(&aStr[5][0], "Key:%d\n", pState->Gamepad.wButtons);

	wsprintf(&aStr[6][0], "Gauge:%d\n", pGauge[pPlayer->nGaugeNo].nPercentGauge);

	wsprintf(&aStr[7][0], "Count:%d\n", pPlayer->nCounterBulletCharge);

	wsprintf(&aStr[8][0], "X:%d\n", pos.x);

	wsprintf(&aStr[9][0], "Y:%d\n", pos.y);

	wsprintf(&aStr[10][0], "Volume:%d\n", (int)(GetVolume(SETSOUND_GAME) * 100.0f));


	// �����������
	for (int nCntDebug = 0; nCntDebug < 10; nCntDebug++)
	{
		strcat(&aStr[0][0], &aStr[nCntDebug + 1][0]);
	}

	// �e�L�X�g�̕`��
    g_pFont->DrawText(NULL, &aStr[0][0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//================================================
// �E�B���h�E�t���X�N���[������
//================================================
void ToggleFullscreen(HWND hWnd)
{
	// ���݂̃E�B���h�E�X�^�C�����擾
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (g_isFullscreen)
	{
		// �E�B���h�E���[�h�ɐ؂�ւ�
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// �t���X�N���[�����[�h�ɐ؂�ւ�
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}

//================================================
// �Q�[���̓�Փx�ݒ菈��
//================================================
void SetGameDifficulty(GAMEDIFFICULTY difficulty)
{
	g_Difficulty = difficulty;
}

//================================================
// �Q�[���̓�Փx�ݒ菈��
//================================================
GAMEDIFFICULTY GetGameDifficulty(void)
{
	return g_Difficulty;
}