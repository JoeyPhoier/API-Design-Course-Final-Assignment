#include "PlayerShip.h"
#include "RayUtils.h"
#include <iostream>

PlayerShip::PlayerShip()
{
	//TODO: Get rid of magic 70 value;
	position = { static_cast<float>(GetScreenWidth() * .5f),
				 static_cast<float>(GetScreenHeight() - 70) };
}

void PlayerShip::Update()
{
	// TODO: Move these to an input processing member maybe.

	//Movement
	float direction = 0;
	if (IsKeyDown(KEY_LEFT))
	{
		direction--;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		direction++;
	}

	position.x += speed * direction;

	const float RightEdge = static_cast<float>(GetScreenWidth()) - radius;
	if (position.x < 0 + radius)
	{
		position.x = radius;
	}
	else if (position.x > RightEdge)
	{
		position.x = RightEdge;
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

void PlayerShip::Render(const Texture2D& texture) const noexcept
{
	DrawTextureQuick(texture, position, renderScale);
}