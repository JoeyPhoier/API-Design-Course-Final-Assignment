#pragma once
#include "raylib.h"
#include "MyTexture2D.h"

class Alien final
{
private:
	bool isAlive = true;
	static float speed;

	static constexpr float renderScale = .3f;
public:
	Vector2 position = { 0, 0 };

	static constexpr float radius = 30;
	static bool shouldMoveDownThisFrame;

	[[nodiscard]] bool IsAlive() const noexcept
	{
		return isAlive;
	}
	void Kill() noexcept
	{
		isAlive = false;
	}

	void Update() noexcept;
	void Render(const Texture2D& texture) const noexcept;
};
