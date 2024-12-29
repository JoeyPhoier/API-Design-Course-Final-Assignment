#pragma once
#include "raylib.h"
#include "BaseEntity.h"

class Projectile final : public BaseEntity
{
private:
	static constexpr int speed = 15;
public:
	static constexpr Vector2 size = { 10,10 };
	bool playerProjectile = true;

	Projectile(Vector2 spawnPoint, bool wasFiredByPlayer) noexcept : BaseEntity(spawnPoint), playerProjectile(wasFiredByPlayer) {};

	void Update() noexcept override
	{
		position.y += playerProjectile ? -speed : +speed;

		const auto BottomEdge = static_cast<float>(GetScreenHeight());
		if (const bool isOutOfBounds = position.y < 0 || position.y > BottomEdge)
		{
			Damage();
		}
	}
};

