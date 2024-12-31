#pragma once
#include "raylib.h"
#include "RayUtils.h"
#include <vector>
#include <concepts>
#include <type_traits>
#include <utility>

class BaseEntity
{
protected:
	bool isAlive = true;
	static constexpr float renderScale = .3f;
public:
	Vector2 position = { 0, 0 };

	BaseEntity() = default;
	explicit BaseEntity(Vector2 spawnPoint) noexcept : position(spawnPoint)
	{};
	virtual ~BaseEntity() = default;

	BaseEntity(const BaseEntity& other) = default;
	BaseEntity& operator=(const BaseEntity& other) = default;
	BaseEntity(BaseEntity&& other) = default;
	BaseEntity& operator=(BaseEntity&& other) = default;

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

template <typename V>
concept VectorOfDerivedFromEntity = std::is_same_v<V, std::vector<typename std::remove_cvref_t<V>::value_type>>&&
									std::is_base_of_v<BaseEntity, typename V::value_type>;

void RenderEntityVector(const VectorOfDerivedFromEntity auto& entityVector, const Texture2D& texture) noexcept
{
	for (const auto& entity : entityVector)
	{
		entity.Render(texture);
	}
}
