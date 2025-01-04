#pragma once
#include "raylib.h"
#include <string_view>
#include <stdexcept>
#include <format>
#include <utility>

class TextureLoadingException : public std::runtime_error
{
public:
	static constexpr std::string_view message = "TEXTURE LOADING EXCEPTION : Failed to load Texture at {}.";
	explicit TextureLoadingException(std::string_view path) : runtime_error(std::format(message, path)) {}
};

class MyTexture2D
{
	Texture2D texture;
	
public:
	MyTexture2D() = delete;
	explicit MyTexture2D(std::string_view path) : texture(LoadTexture(path.data()))
	{
		if (texture.id == 0)
		{
			throw TextureLoadingException(path);
		}
	}
	~MyTexture2D() noexcept
	{
		UnloadTexture(texture);
	}

	MyTexture2D(const MyTexture2D& other) = delete;
	MyTexture2D operator=(const MyTexture2D& other) = delete;
	MyTexture2D(MyTexture2D&& other) = delete;
	MyTexture2D operator=(MyTexture2D&& other) = delete;

	[[nodiscard]] const Texture2D& get() const noexcept
	{
		return texture;
	}
	//Normally, implicit conversions like these are evil. However, I find that
	//wrappers and RAII classes can be treated as an exception to this rule.
	[[nodiscard]] operator Texture2D() const noexcept
	{
		return get();
	}
};