#include "Barrier.h"
#include "RayUtils.h"

void Barrier::Render(const Texture2D& texture)
{
	DrawTextureQuick(texture, position, .3f);
	DrawText(TextFormat("%i", health), position.x - 21, position.y + 10, 40, RED);
}

void Barrier::Update()
{
	// set walls as inactive when out of health
	if (health < 1)
	{
		active = false;
	}
}