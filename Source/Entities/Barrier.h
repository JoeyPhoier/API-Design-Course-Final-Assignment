#pragma once
#include "raylib.h"
#include "BaseEntity.h"
#include <format>

class Barrier final : public BaseEntity
{
private:
	static constexpr unsigned int maxHealth = 50;

	static constexpr unsigned int textSize = 40;
	static constexpr Vector2Int textOffset = { -21, 10 };
public:
	static constexpr float radius = 60;
	unsigned int currHealth = maxHealth;

	Barrier() noexcept = default;
	explicit Barrier(Vector2 spawnPoint) noexcept : BaseEntity(spawnPoint) {};
	constexpr ~Barrier() noexcept override = default;

	void Damage() noexcept override
	{
		--currHealth;
		if (currHealth <= 0)
		{
			isAlive = false;
		}
	}
	inline void ResetHealth() noexcept
	{
		currHealth = maxHealth;
	}

	void Update() noexcept override { /*Does not require any Updating operations.*/ };
	void Render(const TextureLibrary& textureLib) const override
	{
		RenderSimpleTexture(textureLib.barrierTexture);
		DrawText(std::format("{}", currHealth).c_str(),
				 static_cast<int>(position.x + textOffset.x),
				 static_cast<int>(position.y + textOffset.y),
				 40, RED);
	}

	void Serialize(MyVariableSaver& outFile) const override
	{
		BaseEntity::Serialize(outFile);
		outFile.Save(currHealth);

	}
	void Unserialize(MyVariableLoader& inFile) override
	{
		BaseEntity::Unserialize(inFile);
		inFile.Load(currHealth);
	}
};
