#include "Alien.h"
#include "RayUtils.h"

void Alien::Update()
{
	int window_width = GetScreenWidth();

	//TODO: This can formated in a cleaner way
	if (moveRight)
	{
		position.x += speed;

		if (position.x >= GetScreenWidth())
		{
			moveRight = false;
			position.y += 50;
		}
	}
	else
	{
		position.x -= speed;

		if (position.x <= 0)
		{
			moveRight = true;
			position.y += 50;
		}
	}
}

void Alien::Render(const Texture2D& texture) const noexcept
{
	DrawTextureQuick(texture, position, .3f);
}