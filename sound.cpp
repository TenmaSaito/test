//=============================================================================
//
// �T�E���h���� [sound.cpp]
// Author : TENMA SAITO (SPACIAL THANKS : AKIRA TANAKA)
//
//=============================================================================
#include "sound.h"
#include "fade.h"
#include "fade_stage.h"

//*****************************************************************************
// �T�E���h���̍\���̒�`
//*****************************************************************************
typedef struct
{
	const char *pFilename;	// �t�@�C����
	int nCntLoop;			// ���[�v�J�E���g
} SOUNDINFO;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
XAUDIO2_VOICE_DETAILS g_MasterVoiceDetails = {};						// �}�X�^�[�{�C�X�̉������
XAUDIO2_VOICE_DETAILS g_aSourceVoiceDetails[SOUND_LABEL_MAX] = {};		// �\�[�X�{�C�X�̉������
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y
bool g_aPlayAudio[SOUND_LABEL_MAX] = {};					// �Đ���
float g_fVolumeAudio;										// ���݂̉���
SOUND_LABEL g_PlaySound;									// �Đ����̉���
SOUND_LABEL	g_FadelabelExac;								// �t�F�[�h�A�E�g���ɍĐ�����Ă��鉹��
SOUND_LABEL g_Fadelabel;									// �t�F�[�h�C�����ɍĐ����鉹��
FADE g_CurrentFadeSound;									// ���݂̃t�F�[�h���
int g_nCounterSound;										// �ėp�J�E���^�[

// �T�E���h�̏��(sound.h��LABEL�ɒǉ������炱���ɂ��ǉ�����I)
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{"data/BGM/BGM_TITLE.wav", -1},					// �^�C�g����ʂ�BGM
	{"data/BGM/BGM_GAME_NORMAL.wav", -1},			// �Q�[���v���C���̑���BGM
	{"data/BGM/BGM_GAME_DESERT.wav", -1},			// �Q�[���v���C���̍���BGM
	{"data/BGM/BGM_GAME_ICE.wav", -1},				// �Q�[���v���C���̕X��BGM
	{"data/BGM/BGM_GAME_FOREST.wav", -1},			// �Q�[���v���C���̐X��BGM
	{"data/BGM/BGM_GAME_VOLCANO.wav", -1},			// �Q�[���v���C���̉ΎRBGM
	{"data/BGM/BGM_GAME_SEA.wav", -1},				// �Q�[���v���C���̊C�mBGM
	{"data/BGM/BGM_GAME_NOTIME.wav", -1},		// �������Ԃ�����������BGM
	{"data/BGM/BGM_GAMECLEAR.wav",-1},				// �Q�[���N���A����BGM
	{"data/BGM/BGM_GAMEOVER.wav",-1},				// �Q�[���I�[�o�[����BGM
	{"data/BGM/BGM_GAME_TUTORIAL.wav",-1},			// �`���[�g���A������BGM
	{"data/BGM/BGM_CREDIT.wav",-1},					// �N���W�b�g����BGM
	{"data/SE/shot000.wav", 0},			// �e���ˉ�
	{"data/SE/hit000.wav", 0},			// �q�b�g��
	{"data/SE/explosion000.wav", 0},	// ������
	{"data/SE/Enter.wav", 0},			// ���艹
	{"data/SE/BarriarSE.wav", 0},		// �o���A��
	{"data/SE/CoinSE.wav", 0},			// �m�[�}���R�C����
	{"data/SE/Ultimate_CoinSE.wav", 0},	// �����R�C����
	{"data/SE/GetKeySE.wav", 0},		// ���̎擾��
	{"data/SE/OpenDoorSE.wav", 0},		// �h�A���J���鉹
	{"data/SE/SelectSE.wav", 0},		// �I����
	{"data/SE/ChargeSE.wav", 0},		// �`���[�W���̉�
	{"data/SE/FullChargeSE.wav", -1},	// �`���[�W�}�b�N�X��
	{"data/SE/ShotSE.wav", 0},			// �V���b�g��
	{"data/SE/ChargeShotSE.wav", 0},	// �`���[�W�Z�̎ˌ���
	{"data/SE/HEAL_SE.wav", 0},			// �񕜂̎擾��
	{"data/SE/POWERITEM_SE.wav", 0},	// �`���[�W�Z�̎擾��
	{"data/SE/TELEPORT_SE.wav", 0},		// �X�e�[�W�ړ���
	{"data/SE/MirrorSE.wav", 0},		// ���ˉ�
};

//=============================================================================
// ����������
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COM���C�u�����̏�����
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "COM���C�u�����̏������Ɏ��s�I", "�x���I", MB_ICONWARNING);

		return E_FAIL;
	}

	// XAudio2�I�u�W�F�N�g�̍쐬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2�I�u�W�F�N�g�̍쐬�Ɏ��s�I", "�x���I", MB_ICONWARNING);

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}
	
	// �}�X�^�[�{�C�X�̐���
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�X�^�[�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2�I�u�W�F�N�g�̊J��
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COM���C�u�����̏I������
		CoUninitialize();

		return E_FAIL;
	}

	// �T�E���h�f�[�^�̏�����
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// �o�b�t�@�̃N���A
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// �T�E���h�f�[�^�t�@�C���̐���
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			return HRESULT_FROM_WIN32(GetLastError());
		}

		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// �t�@�C���|�C���^��擪�Ɉړ�
			MessageBox(hWnd, "�T�E���h�f�[�^�t�@�C���̐����Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVE�t�@�C���̃`�F�b�N
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}

		// �`�����N�f�[�^�̓ǂݍ���
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}

		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVE�t�@�C���̃`�F�b�N�Ɏ��s�I(3)", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}
	
		// �t�H�[�}�b�g�`�F�b�N
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}

		// �`�����N�f�[�^�̓ǂݍ���
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�t�H�[�}�b�g�`�F�b�N�Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}

		// �f�[�^�`�F�b�N
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(1)", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}

		// �`�����N�f�[�^�̓ǂݍ���
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�I�[�f�B�I�f�[�^�ǂݍ��݂Ɏ��s�I(2)", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}
	
		// �\�[�X�{�C�X�̐���
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "�\�[�X�{�C�X�̐����Ɏ��s�I", "�x���I", MB_ICONWARNING);

			return S_FALSE;
		}

		// �o�b�t�@�̒l�ݒ�
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		memset(g_aSourceVoiceDetails, 0, sizeof(XAUDIO2_VOICE_DETAILS));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;

		// �I�[�f�B�I�o�b�t�@�̓o�^
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		g_apSourceVoice[nCntSound]->GetVoiceDetails(&g_aSourceVoiceDetails[nCntSound]);

		// �t�@�C�����N���[�Y
		CloseHandle(hFile);
	}

	g_pMasteringVoice->GetVoiceDetails(&g_MasterVoiceDetails);

	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		g_aPlayAudio[nCntSound] = false;
	}

	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound] != NULL)
		{
			// ���ʂ�ݒ�
			g_apSourceVoice[nCntSound]->SetVolume(0.0f, XAUDIO2_COMMIT_NOW);
		}
	}

	// �������ʂ���
	g_fVolumeAudio = 0.0f;

	// �t�F�[�h�C�����̉�����ݒ�(����)
	g_Fadelabel = SOUND_LABEL_TITLE;

	// �t�F�[�h�A�E�g���̉�����ݒ�(����)
	g_FadelabelExac = SOUND_LABEL_TITLE;

	// �Đ����̉����̏�����
	g_PlaySound = SOUND_LABEL_TITLE;

	// �t�F�[�h�C���ɐݒ�
	g_CurrentFadeSound = FADE_IN;

	// �J�E���^�[��������
	g_nCounterSound = 0;

	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// �\�[�X�{�C�X�̔j��
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
			g_aSourceVoiceDetails[nCntSound] = {};
	
			// �I�[�f�B�I�f�[�^�̊J��
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// �}�X�^�[�{�C�X�̔j��
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	g_MasterVoiceDetails = {};
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2�I�u�W�F�N�g�̊J��
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COM���C�u�����̏I������
	CoUninitialize();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdateSound(void)
{
	if (g_CurrentFadeSound != FADE_NONE)
	{
		if (g_CurrentFadeSound == FADE_IN && (GetFade() == FADE_IN || GetFadeStage() == FADESTAGE_IN))
		{
			g_fVolumeAudio += 0.01f;
			if (g_nCounterSound == 0)
			{
				PlaySound(g_Fadelabel);
				g_nCounterSound--;
			}
			else
			{
				g_nCounterSound--;
			}

			for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
			{
				if (g_apSourceVoice[nCntSound] != NULL)
				{
					// ���ʂ�ݒ�
					g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
				}
			}

			if (g_fVolumeAudio >= SOUND_STANDARD)
			{
				g_CurrentFadeSound = FADE_NONE;
				g_fVolumeAudio = SOUND_STANDARD;
				for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
				{
					if (g_apSourceVoice[nCntSound] != NULL)
					{
						// ���ʂ�ݒ�
						g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
					}
				}
			}
		}
		else if (g_CurrentFadeSound == FADE_OUT)
		{
			g_fVolumeAudio -= 0.01f;
			for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
			{
				if (g_apSourceVoice[nCntSound] != NULL)
				{
					// ���ʂ�ݒ�
					g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
				}
			}

			if (g_fVolumeAudio <= 0.0f)
			{
				g_CurrentFadeSound = FADE_IN;
				g_fVolumeAudio = 0.0f;
				for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
				{
					if (g_apSourceVoice[nCntSound] != NULL)
					{
						// ���ʂ�ݒ�
						g_apSourceVoice[nCntSound]->SetVolume(g_fVolumeAudio, XAUDIO2_COMMIT_NOW);
					}
					//float af[12] = { 0.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0,0.0f,0.0f };
					//g_apSourceVoice[nCntSound]->GetVoiceDetails(&g_apSourceVoiceDetails[nCntSound]);
					//float f[51] = {};

					/* Set,GetOutputMatrix�̈���(1 : SourceVoice�̈���(MasterVoice)�B������MasterVoice�����݂���ꍇ�̂ݎg�p�B��݂̂̏ꍇ��NULL
												   2 : SourceVoice�̃`�����l�����B����������g_apSourceVoiceDetails�Ɏ擾�ς݁BInputChannels���Q�Ƃ���B
												   3 : MasterVoice�̃`�����l�����B����������g_pMasterVoiceDetails�Ɏ擾�ς݁BInputChannels���Q�Ƃ���B������MasterVoice�����݂���ꍇ�A�����MasterVoiceDetails���g�p���邱�ƁB
												   4 : �e�`�����l���̉��ʁB���������̔z��̃|�C���^��n���B����12�����A�I�[�o�[�t���[�h�~��24�ɂ��Ă���B
													   �z��̊e�ԍ����X�s�[�J�[�̏ꏊ������(�� : 0 = ����, 3 = �E��)�BGet�̏ꍇ�͔z��Ɍ��݂̉��ʂ̏�Ԃ���������B
													   Set�̏ꍇ�́A�z��̒l�����SourceVoice�̉��ʂ����������B)
					*/

					//g_apSourceVoice[nCntSound]->GetOutputMatrix(NULL, g_aSourceVoiceDetails[nCntSound].InputChannels, g_MasterVoiceDetails.InputChannels, &f[0]);
					//g_apSourceVoice[nCntSound]->SetOutputMatrix(g_aSourceVoice[nCntSound], g_aSourceVoiceDetails[nCntSound].InputChannels, g_MasterVoiceDetails.InputChannels, &af[0]);
				}

				g_nCounterSound = 6;

				StopSound();
			}
		}
	}
}

//=============================================================================
// �Z�O�����g�Đ�(�Đ����Ȃ��~)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// �o�b�t�@�̒l�ݒ�
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aSoundInfo[label].nCntLoop;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// �I�[�f�B�I�o�b�t�@�̓o�^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// �Đ�
	g_apSourceVoice[label]->Start(0);

	// �Đ��󋵂�true��
	g_aPlayAudio[label] = true;

	if (label >= SOUND_BGMSTART && label < SOUND_SESTART)
	{// �Đ�����̂�BGM�������ꍇ
		g_PlaySound = label;
	}

	return S_OK;
}

//=============================================================================
// �Z�O�����g��~(���x���w��)
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// ��Ԏ擾
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// �Đ���
		// �ꎞ��~
		g_apSourceVoice[label]->Stop(0);

		// �I�[�f�B�I�o�b�t�@�̍폜
		g_apSourceVoice[label]->FlushSourceBuffers();

		// �Đ��󋵂�false��
		g_aPlayAudio[label] = false;
	}
}

//=============================================================================
// �Z�O�����g��~(�S��)
//=============================================================================
void StopSound(void)
{
	// �ꎞ��~
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// �ꎞ��~
			g_apSourceVoice[nCntSound]->Stop(0);
			// �Đ��󋵂�false��
			g_aPlayAudio[nCntSound] = false;
		}
	}
}

//=============================================================================
// �`�����N�̃`�F�b�N
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^��擪�Ɉړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// �`�����N�f�[�^�̓ǂݍ���
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// �t�@�C���^�C�v�̓ǂݍ���
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// �t�@�C���|�C���^���`�����N�f�[�^���ړ�
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// �`�����N�f�[�^�̓ǂݍ���
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// �t�@�C���|�C���^���w��ʒu�܂ňړ�
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// �f�[�^�̓ǂݍ���
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//=============================================================================
// ���ʂ̐ݒ�
//=============================================================================
void SetVolume(float volume, SETSOUND sound)
{
	switch (sound)
	{

	case SETSOUND_GAME:

		for (int nCntSound = SOUND_BGMSTART; nCntSound < SOUND_MENUSTART; nCntSound++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// ���ʂ�ݒ�
				g_apSourceVoice[nCntSound]->SetVolume(volume, XAUDIO2_COMMIT_NOW);
			}
		}

		break;

	case SETSOUND_MENU_SE:

		for (int nCntSound = SOUND_MENUSTART; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// ���ʂ�ݒ�
				g_apSourceVoice[nCntSound]->SetVolume(volume, XAUDIO2_COMMIT_NOW);
			}
		}

		break;

	case SETSOUND_MAX:

		for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
		{
			if (g_apSourceVoice[nCntSound] != NULL)
			{
				// ���ʂ�ݒ�
				g_apSourceVoice[nCntSound]->SetVolume(volume, XAUDIO2_COMMIT_NOW);
			}
		}

		break;

	default:
		break;
	}

	g_fVolumeAudio = volume;
}

//=============================================================================
// ���ʂ̎擾
//=============================================================================
float GetVolume(SETSOUND sound)
{
	float fVolume = 0.0f;

	if (sound == SETSOUND_GAME)
	{
		g_apSourceVoice[SOUND_BGMSTART]->GetVolume(&fVolume);
	}
	else if (sound == SETSOUND_MENU_SE)
	{
		g_apSourceVoice[SOUND_MENUSTART]->GetVolume(&fVolume);
	}
	
	return fVolume;
}

//=============================================================================
// ���y�̃t�F�[�h����
//=============================================================================
void FadeSound(SOUND_LABEL label)
{
	g_CurrentFadeSound = FADE_OUT;			// �t�F�[�h�A�E�g��Ԃ�
	g_Fadelabel = label;					// �t�F�[�h�C������BGM���w��
	g_FadelabelExac = g_PlaySound;			// �t�F�[�h�A�E�g����BGM���擾
}

//=============================================================================
// ���ݍĐ�����BGM�̎擾
//=============================================================================
SOUND_LABEL GetPlaySound(void)
{
	return g_PlaySound;
}