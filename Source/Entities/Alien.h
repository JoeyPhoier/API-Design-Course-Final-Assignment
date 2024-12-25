#pragma once
#include "raylib.h"
#include "MyTexture2D.h"
#include "Projectile.h"
#include "PlayerShip.h"
#include <vector>

class Alien final : public BaseEntity
{
private:
	friend class AlienArmy;
	static bool shouldMoveDownThisFrame;

	static float speed;
public:
	static constexpr float radius = 30;

	explicit Alien(Vector2 spawnPoint) : BaseEntity(spawnPoint) {};
	void Update() noexcept;
};

class AlienArmy final
{
private:
	static constexpr float maxLaserCooldown = 2;
	float currLaserCooldown = maxLaserCooldown;

	static constexpr Vector2 formationSize = { 8,5 };
	static constexpr float alienSpacing = 80;
public:
	Vector2 position = { 0, 0 };

	std::vector<Alien> alienSpan;
	std::vector<Projectile> alienLasers;

	AlienArmy() = default;
	explicit AlienArmy(Vector2 spawnPoint) noexcept : position(spawnPoint)
	{};

	void ResetArmy() noexcept;

	void Update() noexcept;

	void Render(const Texture2D& alienTexture, const Texture2D& projectileTexture) const noexcept;
};
