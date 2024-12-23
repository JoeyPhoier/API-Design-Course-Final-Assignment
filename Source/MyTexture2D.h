#pragma once
#include "raylib.h"
#include <string_view>
#include <stdexcept>
#include <format>

class MyTexture2D
{
	Texture2D texture;
	
	MyTexture2D() = delete;
	explicit MyTexture2D(std::string_view path)
	{
		texture = LoadTexture(path.data());
		if (texture.id == 0)
		{
			throw(std::runtime_error(std::format("Failed to load Texture at {}.", path)));
		}
	}
	~MyTexture2D() noexcept
	{
		UnloadTexture(texture);
	}

	MyTexture2D(const MyTexture2D& other) = delete;
	MyTexture2D(const MyTexture2D&& other) = delete;
	MyTexture2D operator=(const MyTexture2D& other) = delete;
	MyTexture2D operator=(const MyTexture2D&& other) = delete;

	[[nodiscard]] const Texture2D& get() const noexcept
	{
		return texture;
	}
};