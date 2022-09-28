#pragma once

//Anim_Sprite program header file

#ifndef _GAME_H
#define _GAME_H 1

//windows/directx headers
#include <d3d9.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <d3dx9math.h>

//framework-specific headers
#include "dxgraphics.h"
#include "dxinput.h"
#include "dxaudio.h"

//application title
#define APPTITILE L"Paddle_Game"

//screen setup
#define FULLSCREEN 0         //1 = fullscreen, 0 = windowed
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

//macros to read the keyboard asynchronously
//#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
//#define KEY_UP(vk_code)  ((GetAsyncKeyState(vk_code) * 0x8000) ? 1 : 0)

//function prototypes
int Game_Init(HWND);
void Game_Run(HWND);
void Game_End(HWND);

//sprite structure
typedef struct
{
	int x, y;
	int width, height;
	int movex, movey;
	int curframe, lastframe;
	int animdelay, animcount;
	int scalex, scaley;
	int rotation, rotaterate;
} SPRITE;

#endif 
