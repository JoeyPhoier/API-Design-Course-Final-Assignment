#pragma once
#include "raylib.h"
#include "MyTexture2D.h"
#include "Projectile.h"
#include "PlayerShip.h"
#include <vector>

class Alien final
{
private:
	friend class AlienArmy;
	bool isAlive = true;
	static float speed;

	static constexpr float renderScale = .3f;
public:
	Vector2 position = { 0, 0 };

	static constexpr float radius = 30;
	static bool shouldMoveDownThisFrame;

	Alien() = default;
	explicit Alien(Vector2 spawnPoint) noexcept : position(spawnPoint)
	{};

	[[nodiscard]] bool IsAlive() const noexcept
	{
		return isAlive;
	}
	void Kill() noexcept
	{
		isAlive = false;
	}

	void Update() noexcept;
	void Render(const Texture2D& texture) const noexcept;
};

class AlienArmy final
{
private:
	static constexpr float maxLaserCooldown = 2;
	float currLaserCooldown = maxLaserCooldown;

	Vector2 formationSize = { 8,5 };
	int alienSpacing = 80;

public:
	Vector2 position = { 0, 0 };

	std::vector<Alien> alienSpan;
	std::vector<Projectile> alienLasers;

	AlienArmy() = default;
	explicit AlienArmy(Vector2 spawnPoint) noexcept : position(spawnPoint)
	{};

	void ResetArmy() noexcept;

	void Update() noexcept;
	void EraseDeadEntities() noexcept;

	void Render(const Texture2D& alienTexture, const Texture2D& projectileTexture) const noexcept;
};
