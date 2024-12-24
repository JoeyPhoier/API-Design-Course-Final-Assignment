#include "Projectile.h"
#include "RayUtils.h"

void Projectile::Update()
{
	position.y += (playerProjectile) ? -speed : +speed;

	const auto BottomEdge = static_cast<float>(GetScreenHeight());
	if (bool isOutOfBounds = position.y < 0 || position.y > BottomEdge)
	{
		Destroy();
	}
}

void Projectile::Render(const Texture2D& texture) const noexcept
{
	DrawTextureQuick(texture, position, .3f);
}