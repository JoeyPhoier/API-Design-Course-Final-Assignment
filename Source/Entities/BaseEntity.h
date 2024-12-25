#pragma once
#include "raylib.h"
#include "RayUtils.h"

class BaseEntity
{
protected:
	bool isAlive = true;
	static constexpr float renderScale = .3f;
public:
	Vector2 position = { 0, 0 };

	static constexpr float radius = 30;

	BaseEntity() = default;
	explicit BaseEntity(Vector2 spawnPoint) noexcept : position(spawnPoint)
	{};
	virtual ~BaseEntity() = default;

	[[nodiscard]] bool IsAlive() const noexcept
	{
		return isAlive;
	}
	virtual void Damage() noexcept
	{
		isAlive = false;
	}

	virtual void Update() noexcept = 0;
	virtual void Render(const Texture2D& texture) const noexcept
	{
		DrawTextureQuick(texture, position, renderScale);
	}
};
