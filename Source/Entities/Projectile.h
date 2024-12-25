#pragma once
#include "raylib.h"
#include "MyTexture2D.h"
#include "RayUtils.h"

class Projectile final
{
private:
	bool isAlive = true;
	static constexpr int speed = 15;
public:
	Vector2 position = { 0,0 };
	static constexpr Vector2 size = { 10,10 };
	bool playerProjectile = true;

	Projectile() = default;
	Projectile(Vector2 spawnPoint, bool wasFiredByPlayer) noexcept : position(spawnPoint), playerProjectile(wasFiredByPlayer)
	{};

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

