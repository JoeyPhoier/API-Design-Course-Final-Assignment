#pragma once
#include "raylib.h"
#include "MyTexture2D.h"
#include <vector>

class PlayerShip final
{
private: 
	bool isAlive;
	static constexpr int maxHealth = 3;

	static constexpr float speed = 7;
	static constexpr float renderScale = .3f;
public:
	Vector2 position = { 0,0 };
	float radius = 50;

	int currHealth = maxHealth;

	//TODO: Use a proper spritesheet and handle this privately
	int activeTexture = 0;
	float timer = 0;

	PlayerShip();

	void Damage() noexcept
	{
		--currHealth;
		if (currHealth <= 0)
		{
			isAlive = false;
		}
	}

	void Render(const Texture2D& texture) const noexcept;
	void Update();
};
