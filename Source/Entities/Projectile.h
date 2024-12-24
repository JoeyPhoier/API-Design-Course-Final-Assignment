#pragma once
#include "raylib.h"
#include "MyTexture2D.h"

class Projectile final
{
private:
	bool isAlive = true;
	static constexpr int speed = 15;
public:
	Vector2 position = { 0,0 };
	bool playerProjectile = true;

	Projectile() = default;
	Projectile(Vector2 spawnPoint, bool wasFiredByPlayer) noexcept;

	[[nodiscard]] bool IsAlive() const noexcept
	{
		return isAlive;
	}
	void Destroy() noexcept
	{
		isAlive = false;
	}

	void Update();
	void Render(const Texture2D& texture) const noexcept;
};

