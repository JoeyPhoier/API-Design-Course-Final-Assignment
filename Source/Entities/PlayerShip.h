#pragma once
#include "raylib.h"
#include "BaseEntity.h"
#include "Projectile.h"
#include <vector>
#include "MyAnimation2D.h"

class PlayerShip final : public BaseEntity
{
private: 
	static constexpr unsigned int maxHealth = 3;
	static constexpr float speed = 7;

	static constexpr float maxLaserCooldown = 0.4f;
	float currLaserCooldown = 0;

	MyAnimator2D animator;
public:
	static constexpr float radius = 50;
	unsigned int currHealth = maxHealth;

	PlayerShip() noexcept { Reset(); };
	~PlayerShip() override = default;
	PlayerShip(const PlayerShip& other) = default;
	PlayerShip& operator=(const PlayerShip& other) = default;
	PlayerShip(PlayerShip&& other) = default;
	PlayerShip& operator=(PlayerShip&& other) = default;

	void Reset() noexcept;

	void Damage() noexcept override
	{
		if (--currHealth <= 0)
		{
			isAlive = false;
		}
	}

	void Update() noexcept override;
	void CheckForLaserInput(std::vector<Projectile>& playerLasers) noexcept;

	void Render(const TextureLibrary& textureLib) const override;

	void Serialize(MyVariableSaver& outFile) const override
	{
		BaseEntity::Serialize(outFile);
		outFile.Save(currHealth);

	}
	void Unserialize(MyVariableLoader& inFile) override
	{
		BaseEntity::Unserialize(inFile);
		currLaserCooldown = 0;
		inFile.Load(currHealth);
	}
};
