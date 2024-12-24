#include "Projectile.h"
#include "RayUtils.h"

Projectile::Projectile(Vector2 spawnPoint, bool wasFiredByPlayer) : position(spawnPoint), playerProjectile(wasFiredByPlayer) noexcept {}

void Projectile::Update()
{
	position.y -= speed;

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