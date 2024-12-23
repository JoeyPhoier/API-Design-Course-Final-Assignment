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

static void DrawTextureQuick(const Texture2D& texture, const Vector2& position) noexcept
{
	DrawTextureQuick(texture, position, 1.f);
}


