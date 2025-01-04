#pragma once
#include "raylib.h"
#include "MyTexture2D.h"
#include "Projectile.h"
#include "PlayerShip.h"
#include <vector>
#include <algorithm>

class 
	Alien final : public BaseEntity
{
private:
	friend class AlienArmy;
	static bool shouldMoveDownThisFrame;

	static float speed;
public:
	static constexpr float radius = 30;

	Alien() noexcept = default;
	explicit Alien(Vector2 spawnPoint) noexcept : BaseEntity(spawnPoint) {};
	void Update() noexcept override;
};

class AlienArmy final
{
private:
	static constexpr float maxLaserCooldown = 2;
	float currLaserCooldown = maxLaserCooldown;

	static constexpr Vector2Int formationSize = { 8,5 };
	static constexpr float alienSpacing = 100;

public:
	std::vector<Alien> alienSpan;
	std::vector<Projectile> alienLasers;

	void Clear() noexcept
	{
		alienSpan.clear();
		alienLasers.clear();
	}
	void Reset() noexcept;

	void UpdateAlienShooting() noexcept;
	void Update() noexcept;
	[[nodiscard]] inline bool HasAlienReachedPlayer(const Vector2& playerPosition, const float playerRadius) const noexcept
	{
		return std::ranges::any_of(alienSpan, [&](const Alien& alien)
								   {
									   return (alien.position.y + Alien::radius > playerPosition.y - playerRadius);
								   });
	}

	inline void Render(const Texture2D& alienTexture, const Texture2D& projectileTexture) const noexcept
	{
		RenderEntityVector(alienSpan, alienTexture);
		RenderEntityVector(alienLasers, projectileTexture);
	}

	void Serialize(MyVariableSaver& outFile) const
	{
		outFile.Save(alienSpan);
		outFile.Save(Alien::speed);
	}
	void Unserialize(MyVariableLoader& inFile)
	{
		inFile.Load(alienSpan);
		currLaserCooldown = 0;
		inFile.Load(Alien::speed);
	}
};
