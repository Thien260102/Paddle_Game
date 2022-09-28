#include "dxaudio.h"


CSoundManager* dsound;

int Init_DirectSound(HWND hwnd)
{
	dsound = new CSoundManager();

	//initialize CSoundManager
	dsound->Initialize(hwnd, DSSCL_PRIORITY);
	if (!dsound)
	{
		MessageBox(hwnd, L"Error initialize CSoundManager", L"Error", MB_OK);
		return 0;
	}


	return 1;
}

CSound* LoadSound(TCHAR filename[])
{
	CSound* wave;
	dsound->Create(&wave, (LPTSTR)filename);
	return wave;
}

void PlaySound(CSound* wave)
{
	wave->Play(0);
}

void LoopSound(CSound* wave)
{
	wave->Play(0, DSBPLAY_LOOPING);
}

void StopSound(CSound* wave)
{
	wave->Stop();
}