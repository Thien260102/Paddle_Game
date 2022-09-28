//Anim_Sprite program source code file

#include "game.h"

//background image
LPDIRECT3DSURFACE9 back;

//sprite handler
LPD3DXSPRITE sprite_handler;

//ball, paddle sprites
LPDIRECT3DTEXTURE9 paddle_ball;
SPRITE ball;
SPRITE paddle;
RECT srcBall;
RECT srcPaddle;

// the wave sound
// CSound *sound_bounce;
CSound *sound_hit;

//misc
HRESULT result;
long start = GetTickCount();


//initializes the game
int Game_Init(HWND hwnd)
{
	//set random number seed
	srand(time(NULL));

	//initialize DirectInput
	if (!Init_DirectInput(hwnd))
	{
		MessageBox(hwnd, L"Error initializing the DirectInput", L"Error", MB_OK);
		return 0;
	}

	//initialize mouse
	if (!Init_Mouse(hwnd))
	{
		MessageBox(hwnd, L"Error initializing the mouse", L"Error", MB_OK);
		return 0;
	}

	//initialize keyboard
	if (!Init_Keyboard(hwnd))
	{
		MessageBox(hwnd, L"Error initializing the keyboard", L"Error", MB_OK);
		return 0;
	}

	//initialize DirectSound
	if (!Init_DirectSound(hwnd))
	{
		MessageBox(hwnd, L"Error initialize DirectSound", L"Error", MB_OK);

	}

	//create sprite handler
	result = D3DXCreateSprite(d3ddev, &sprite_handler);
	if (result != D3D_OK)
		return 0;

	//load background image
	back = LoadSurface(L"background.jpg", NULL);
	if (!back)
		return 0;

	//load the ball and paddle sprite
	paddle_ball = LoadTexture(L"paddle_ball.jpg", D3DCOLOR_XRGB(255, 0, 255));
	if (!paddle_ball)
		return 0;

	//set the ball's properties
	ball.x = 400;
	ball.y = 200;
	ball.width = 23;
	ball.height = 23;
	ball.movex = 8;
	ball.movey = -8;

	srcBall.left = 126;
	srcBall.top = 30;
	srcBall.right = srcBall.left + ball.width;
	srcBall.bottom = srcBall.top + ball.height;

	//set the paddle's properties
	paddle.x = 300;
	paddle.y = SCREEN_HEIGHT - 50;
	paddle.width = 209;
	paddle.height = 17;

	srcPaddle.left = 33;
	srcPaddle.top = 62;
	srcPaddle.right = srcPaddle.left + paddle.width;
	srcPaddle.bottom = srcPaddle.top + paddle.height;

	//load bounce wave file
	TCHAR temp[] = _T("hit.wav");
	sound_hit = LoadSound(temp);
	if(!sound_hit)
	return 0;


	//return ok
	return 1;
}

int Collision(SPRITE sprite1, SPRITE sprite2)
{
	RECT rect1;
	rect1.left = sprite1.x + 1;
	rect1.top = sprite1.y + 1;
	rect1.right = sprite1.x + sprite1.width - 1;
	rect1.bottom = sprite1.y + sprite1.height - 1;

	RECT rect2;
	rect2.left = sprite2.x + 1;
	rect2.top = sprite2.y + 1;
	rect2.right = sprite2.x + sprite2.width - 1;
	rect2.bottom = sprite2.y + sprite2.height - 1;

	RECT dest;
	return IntersectRect(&dest, &rect1, &rect2);
}

void Game_Run(HWND hwnd)
{
	//ball position vector
	D3DXVECTOR3 position(0, 0, 0);

	//make sure the direct3d device is valid
	if (d3ddev == NULL)
		return;

	//update mouse and keyboard
	Poll_Mouse();
	Poll_Keyboard();

	//after short delay, ready for next frame
	//this keep the game running at a steady frame rate
	if (GetTickCount() - start >= 30)
	{
		//reset timing
		start = GetTickCount();

		//move the ball sprite
		ball.x += ball.movex;
		ball.y += ball.movey;

		//bounce the ball at screen edges
		if (ball.x > SCREEN_WIDTH - ball.width)
		{
			ball.x -= ball.width;
			ball.movex *= -1;
			//PlaySound(sound_bounce);
		}
		else if (ball.x < 0)
		{
			ball.x += ball.width;
			ball.movex *= -1;
			//PlaySound(sound_bounce);
		}

		if (ball.y > SCREEN_HEIGHT - ball.height)
		{
			ball.y -= ball.height;
			ball.movey *= -1;
			//PlaySound(sound_bounce);
		}
		else if (ball.y < 0)
		{
			ball.y += ball.width;
			ball.movey *= -1;
			//PlaySound(sound_bounce);
		}

		//move the paddle
		paddle.x += Mouse_X();
		if (paddle.x > SCREEN_WIDTH - paddle.width)
			paddle.x = SCREEN_WIDTH - paddle.width;
		else if (paddle.x < 0)
			paddle.x = 0;

		//check for left arrow
		if (Key_Down(DIK_LEFT))
			paddle.x -= 5;
		//check for right arrow
		if (Key_Down(DIK_RIGHT))
			paddle.x += 5;

		//see if the ball hit the paddle
		if (Collision(paddle, ball))
		{
			ball.y -= ball.movey;
			ball.movey *= -1;
			PlaySound(sound_hit);
		}
	}

	// start rendering
	if (d3ddev->BeginScene())
	{
		//erase the entire background
		d3ddev->StretchRect(back, NULL, backbuffer, NULL, D3DTEXF_NONE);

		//start sprite handler
		sprite_handler->Begin(D3DXSPRITE_ALPHABLEND);

		//draw the ball
		position.x = (float)ball.x;
		position.y = (float)ball.y;
		sprite_handler->Draw(
			paddle_ball,
			&srcBall,
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255));

		

		//draw the paddle
		position.x = (float)paddle.x;
		position.y = (float)paddle.y;
		sprite_handler->Draw(
			paddle_ball,
			&srcPaddle,
			NULL,
			&position,
			D3DCOLOR_XRGB(255, 255, 255));

		//stop drawing
		sprite_handler->End();

		//stop rendering
		d3ddev->EndScene();
	}

	//display the backbuffer on the screen
	d3ddev->Present(NULL, NULL, NULL, NULL);

	//check for mouse button (to exit program)
	if (Mouse_Button(0))
		PostMessage(hwnd, WM_DESTROY, 0, 0);

	//check for escape key (to exit program)
	if (Key_Down(DIK_ESCAPE))
		PostMessage(hwnd, WM_DESTROY, 0, 0);

}

//frees memory and cleans up before the game ends
void Game_End(HWND hwnd)
{
	if (paddle_ball != NULL)
		paddle_ball->Release();

	if (back != NULL)
		back->Release();

	if (sprite_handler != NULL)
		sprite_handler->Release();

	//if (sound_bounce != NULL)
	// delete sound_bounce;
	if (sound_hit != NULL)
		delete sound_hit;

}