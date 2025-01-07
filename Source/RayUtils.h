#pragma once
#include "raylib.h"

inline static void DrawTextureQuick_WithSource(const Texture2D& texture, Rectangle source, Vector2 position, float scale) noexcept
{
	DrawTexturePro(texture, source,
		{
			position.x,
			position.y,
			source.width * scale,
			source.height * scale,
		},
		{
			source.width * scale * 0.5f,
			source.height * scale * 0.5f
		},
		0,
		WHITE);
}

inline static void DrawTextureQuick(const Texture2D& texture, Vector2 position, float scale) noexcept
{
	const Rectangle source{ 0, 0, static_cast<float>(texture.width), static_cast<float>(texture.height), };
	DrawTextureQuick_WithSource(texture, source, position, scale);
}

//Raylib has a couple of these operations builtin, such as Vector2Subtract, but these take the vectors in by value.
//Taking them in by reference is lighter, so I opted to make my own.
constexpr Vector2 operator-(const Vector2& first, const Vector2& second) noexcept
{
	return {
		first.x - second.x,
		first.y - second.y
	};
};
constexpr Vector2 operator+(const Vector2& first, const Vector2& second) noexcept
{
	return {
		first.x + second.x,
		first.y + second.y
	};
};
constexpr Vector2 operator+=(Vector2& first, const Vector2& second) noexcept
{
	first.x += second.x;
	first.y += second.y;
	return first;
};
constexpr Vector2 operator*(const Vector2& first, const Vector2& second) noexcept
{
	return {
	first.x * second.x,
	first.y * second.y
	};
};
constexpr Vector2 operator*(const Vector2& first, float second) noexcept
{
	return {
	first.x * second,
	first.y * second
	};
};

struct Vector2Int
{
	int x;
	int y;
};