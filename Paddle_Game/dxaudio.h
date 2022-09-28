#pragma once
//dxaudio.h - directsound framework header file
#ifndef _DXAUDIO_H
#define _DXAUDIO_H

#include <dsound.h>
#include "dsutil.h"
#include <atlstr.h>

//primary directsound object
extern CSoundManager* dsound;

//fuction prototypes
int Init_DirectSound(HWND);
CSound* LoadSound(TCHAR[]);
void PlaySound(CSound*);
void LoopSound(CSound*);
void StopSound(CSound*);
#endif