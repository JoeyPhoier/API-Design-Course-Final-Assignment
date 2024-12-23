#include "PlayerShip.h"
#include "RayUtils.h"
#include <iostream>

void PlayerShip::Initialize()
{
	// TODO: Get rid of two step init.

	float window_width = (float)GetScreenWidth();
	x_pos = window_width / 2;
	std::cout << "Find Player -X:" << GetScreenWidth() / 2 << "Find Player -Y" << GetScreenHeight() - player_base_height << std::endl;
}

void PlayerShip::Update()
{
	// TODO: Move these to an input processing member maybe.

	//Movement
	direction = 0;
	if (IsKeyDown(KEY_LEFT))
	{
		direction--;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		direction++;
	}

	x_pos += speed * direction;

	if (x_pos < 0 + radius)
	{
		x_pos = 0 + radius;
	}
	// TODO: Cast things properly.
	else if (x_pos > GetScreenWidth() - radius)
	{
		x_pos = GetScreenWidth() - radius;
	}

	//Determine frame for animation
	timer += GetFrameTime();

	// TODO: Weird check doing a timer comparison twice. 
	if (timer > 0.4 && activeTexture == 2)
	{
		activeTexture = 0;
		timer = 0;
	}
	else if (timer > 0.4)
	{
		activeTexture++;
		timer = 0;
	}
}

void PlayerShip::Render(Texture2D texture)
{
	DrawTextureQuick(texture, { x_pos, GetScreenHeight() - player_base_height }, .3f);
}