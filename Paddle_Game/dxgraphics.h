#pragma once
// Direct3D framework header file
#include <d3d9.h>

#ifndef _DXGRAPHICS_H
#define _DXGRAPHICS_H

//function prototypes
int Init_Direct3D(HWND, int, int, int);
LPDIRECT3DSURFACE9 LoadSurface(LPCWSTR, D3DCOLOR);
LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR, D3DCOLOR);

//variable declarations
extern LPDIRECT3D9 d3d;
extern LPDIRECT3DDEVICE9 d3ddev;
extern LPDIRECT3DSURFACE9 backbuffer;

#endif
