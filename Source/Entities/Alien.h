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

	explicit Alien(Vector2 spawnPoint) noexcept : BaseEntity(spawnPoint) {};
	void Update() noexcept override;
};

class AlienArmy final
{
private:
	static constexpr float maxLaserCooldown = 2;
	float currLaserCooldown = maxLaserCooldown;

	Vector2 formationSize = { 8,5 };
	static constexpr float alienSpacing = 100;
public:

	std::vector<Alien> alienSpan;
	std::vector<Alien*> bottomAlienCollection;
	std::vector<Projectile> alienLasers;

	AlienArmy() = default;
	explicit AlienArmy(Vector2 formation) noexcept :formationSize(formation){};

	void Clear() noexcept
	{
		alienSpan.clear();
		alienLasers.clear();
	}
	void ResetArmy() noexcept;

	void UpdateAlienShooting() noexcept;
	void Update() noexcept;
	[[nodiscard]] bool HasAlienReachedPlayer(const Vector2& playerPosition, const float playerRadius) const noexcept;

	void Render(const Texture2D& alienTexture, const Texture2D& projectileTexture) const noexcept;
};
