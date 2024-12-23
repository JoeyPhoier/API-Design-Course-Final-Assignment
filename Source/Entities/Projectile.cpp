#include "Projectile.h"
#include "RayUtils.h"

void Projectile::Update()
{
	position.y -= speed;

	lineStart.y = position.y - 15;
	lineEnd.y = position.y + 15;

	lineStart.x = position.x;
	lineEnd.x = position.x;

	if (position.y < 0 || position.y > 1500)
	{
		active = false;
	}
}

void Projectile::Render(const Texture2D& texture)
{
	DrawTextureQuick(texture, position, .3f);
}