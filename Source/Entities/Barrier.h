#pragma once
#include "raylib.h"
#include "BaseEntity.h"
#include <format>

class Barrier final : public BaseEntity
{
private:
	static constexpr int maxHealth = 50;

	static constexpr int textSize = 40;
	static constexpr Vector2 textOffset = { -21, 10 };
public:
	static constexpr float radius = 60;
	int currHealth = maxHealth;

	Barrier() noexcept = default;
	explicit Barrier(Vector2 spawnPoint) noexcept : BaseEntity(spawnPoint) {};

	void Damage() noexcept override
	{
		--currHealth;
		if (currHealth <= 0)
		{
			isAlive = false;
		}
	}

	void Update() noexcept override { /*Does not require any Updating operations.*/ };
	void Render(const Texture2D& texture) const noexcept override
	{
		BaseEntity::Render(texture);
		DrawText(std::format("{}", currHealth).c_str(),
				 static_cast<int>(position.x + textOffset.x),
				 static_cast<int>(position.y + textOffset.y),
				 40, RED);
	}
};
