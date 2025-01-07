#pragma once
#include "raylib.h"
#include "RayUtils.h"
#include <vector>
#include <concepts>
#include <type_traits>
#include <utility>
#include "VariableSaveSystem.h"
#include "TextureLibrary.h"

class BaseEntity
{
protected:
	bool isAlive = true;
	static constexpr float renderScale = .3f;

	virtual void RenderSimpleTexture(const Texture2D& texture) const noexcept
	{
		DrawTextureQuick(texture, position, renderScale);
	}
public:
	Vector2 position = { 0, 0 };

	BaseEntity() = default;
	explicit BaseEntity(Vector2 spawnPoint) noexcept : position(spawnPoint)
	{};
	virtual constexpr ~BaseEntity() noexcept = default;

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
	virtual void Render(const TextureLibrary& textureLib) const = 0;

	virtual void Serialize(MyVariableSaver& outFile) const
	{
		outFile.Save(position);
	}
	virtual void Unserialize(MyVariableLoader& inFile)
	{
		inFile.Load(position);
	}
};

template <typename V>
concept is_VectorOfEntityClasses = is_Container<V> && std::is_base_of_v<BaseEntity, typename V::value_type>;

void RenderEntityVector(const is_VectorOfEntityClasses auto& entityVector, const TextureLibrary& textureLib)
{
	for (const auto& entity : entityVector)
	{
		entity.Render(textureLib);
	}
}
