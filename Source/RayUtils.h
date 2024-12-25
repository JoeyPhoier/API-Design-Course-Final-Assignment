#pragma once
#include "raylib.h"

static void DrawTextureQuick(const Texture2D& texture, Vector2 position, float scale) noexcept
{
	DrawTexturePro(texture,
		{
			0,
			0,
			static_cast<float>(texture.width),
			static_cast<float>(texture.height),
		},
		{
			position.x,
			position.y,
			texture.width * scale,
			texture.height * scale,
		},
		{
			texture.width * scale * 0.5f,
			texture.height * scale * 0.5f
		},
		0,
		WHITE);
}


//This Struct is very similar to the Rectangle class present in raylib.
//However, given a lot of the code utilizes vector2's, the usage of the rectangle class forces
//us to have to constantly create vector2's at the function callsites. Alongside this, I find that
//utilizing the position defining vector as the center of the box to be a lot easier to work with.
struct MyRectangle
{
	//Center of the rectangle.
	Vector2 position;
	//Width and height of the rectangle.
	Vector2 size{ 0,0 };
};

//Raylib has a couple of these operations builtin, such as Vector2Subtract, but these take the vectors in by value.
//Taking them in by reference is lighter, so I opted to make my own.
inline Vector2 operator-(const Vector2& first, const Vector2& second) noexcept
{
	return {
		first.x - second.x,
		first.y - second.y
	};
};
inline Vector2 operator+(const Vector2& first, const Vector2& second) noexcept
{
	return {
		first.x + second.x,
		first.y + second.y
	};
};
inline Vector2 operator+=(Vector2& first, const Vector2& second) noexcept
{
	first.x += second.x;
	first.y += second.y;
	return first;
};
inline Vector2 operator*(const Vector2& first, const Vector2& second) noexcept
{
	return {
	first.x * second.x,
	first.y * second.y
	};
};
inline Vector2 operator*(const Vector2& first, float second) noexcept
{
	return {
	first.x * second,
	first.y * second
	};
};


