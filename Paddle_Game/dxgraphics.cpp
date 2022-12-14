// dxgraphics.cpp - Direct3D framework source code file
#include <d3d9.h>
#include <d3dx9.h>
#include "dxgraphics.h"

//variable declarations
LPDIRECT3D9 d3d = NULL;
LPDIRECT3DDEVICE9 d3ddev = NULL;
LPDIRECT3DSURFACE9 backbuffer = NULL;


int Init_Direct3D(HWND hwnd, int width, int height, int fullscreen)
{
	//initialize Direct3D
	d3d = Direct3DCreate9(D3D_SDK_VERSION);
	if (d3d == NULL)
	{
		MessageBox(hwnd, L"Error initializing Direct3D", L"Error", MB_OK);
		return 0;
	}

	//set Direct3D presentation parameters
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.Windowed = (!fullscreen);
	d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.hDeviceWindow = hwnd;

	// create Direct3D device
	d3d->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hwnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,
		&d3dpp,
		&d3ddev);

	if (d3ddev == NULL)
	{
		MessageBox(hwnd, L"Error creating Direct3D device", L"Error", MB_OK);
		return 0;
	}

	// clear the backbuffer to black
	d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//create pointer to the backbuffer
	d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);

	return 1;
}

LPDIRECT3DSURFACE9 LoadSurface(LPCWSTR filename, D3DCOLOR transcolor)
{
	LPDIRECT3DSURFACE9 image = NULL;
	D3DXIMAGE_INFO info;
	HRESULT result;

	//get width and height from bitmap file
	result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK)
		return NULL;

	//create surface;
	result = d3ddev->CreateOffscreenPlainSurface(
		info.Width,            //width of surface
		info.Height,           //height of surface
		D3DFMT_X8R8G8B8,       //surface format
		D3DPOOL_DEFAULT,       //memory pool to use
		&image,                //pointer to the surface
		NULL);                 //reserved (always NULL)

	if (result != D3D_OK)
		return NULL;

	//load surface from file into newly created surface
	result = D3DXLoadSurfaceFromFile(
		image,         //destination surface
		NULL,          //destination palette
		NULL,          //destination rectangle
		filename,      //source file
		NULL,          //source rectangle
		D3DX_DEFAULT,  //controls how image is filtered
		transcolor,    //for transparency (0 for none)
		NULL);         //source image info (usually NULL)

	//make sure file was loaded ok
	if (result != D3D_OK)
		return NULL;

	return image;

}

LPDIRECT3DTEXTURE9 LoadTexture(LPCWSTR filename, D3DCOLOR transcolor)
{
	//texture pointer
	LPDIRECT3DTEXTURE9 texture = NULL;

	//read info from image
	D3DXIMAGE_INFO info;

	HRESULT result;

	//get info of image
	result = D3DXGetImageInfoFromFile(filename, &info);
	if (result != D3D_OK)
		return NULL;

	//create texture from file image
	result = D3DXCreateTextureFromFileEx(
		d3ddev,         //Direct3D device
		filename,       //source 
		info.Width,
		info.Height,
		1,              //connect level(default is 1)
		D3DPOOL_DEFAULT,//type of surface
		D3DFMT_UNKNOWN, //define type surface
		D3DPOOL_DEFAULT,//memory class for texture
		D3DX_DEFAULT,	//filter
		D3DX_DEFAULT,   //mip filter
		transcolor,     //
		&info,          //info of file
		NULL,
		&texture);

	//make sure texture loaded successfully
	if (result != D3D_OK)
		return NULL;

	return texture;


}