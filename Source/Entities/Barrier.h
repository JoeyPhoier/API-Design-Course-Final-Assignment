#pragma once
#include "raylib.h"
#include "MyTexture2D.h"

class Barrier final
{
private:
	//TODO: Do all entities need an active flag? Consider getting rid of them.
	bool isAlive = true;
	static constexpr int maxHealth = 50;

	static constexpr float renderScale = .3f;
	static constexpr int textSize = 40;
	static constexpr Vector2 textOffset = { -21, 10 };
public:
	Vector2 position;
	int radius = 60;
	int currHealth = maxHealth;

	[[nodiscard]] bool IsAlive() const noexcept
	{
		return isAlive;
	}
	void Damage() noexcept
	{
		--currHealth;
		if (currHealth <= 0)
		{
			isAlive = false;
		}
	}

	void Render(const Texture2D& texture) const noexcept;
};
