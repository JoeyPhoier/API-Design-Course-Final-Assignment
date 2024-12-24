#pragma once
#include "raylib.h"
#include <string_view>
#include <stdexcept>
#include <format>
#include <utility>

class MyTexture2D
{
	Texture2D texture;
	
public:
	MyTexture2D() = delete;
	explicit MyTexture2D(std::string path) : texture(LoadTexture(path.c_str()))
	{
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
	MyTexture2D operator=(const MyTexture2D& other) = delete;

	//Class must be move insertable in order to be emplace constructable 
	MyTexture2D(MyTexture2D&& other) noexcept
	{
		std::swap(texture, other.texture);
	}
	MyTexture2D operator=(MyTexture2D&& other) noexcept
	{
		std::swap(texture, other.texture);
	}


	[[nodiscard]] const Texture2D& get() const noexcept
	{
		return texture;
	}

	operator Texture2D() const noexcept
	{
		return texture;
	}
};