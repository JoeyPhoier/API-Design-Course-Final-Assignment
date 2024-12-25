#pragma once
#include "raylib.h"
#include "BaseEntity.h"
#include "Projectile.h"
#include <vector>

class PlayerShip final : public BaseEntity
{
private: 
	static constexpr int maxHealth = 3;

	static constexpr float speed = 7;
public:
	float radius = 50;

	int currHealth = maxHealth;

	//TODO: Use a proper spritesheet and handle this privately
	int activeTexture = 0;
	float timer = 0;

	PlayerShip() noexcept
	{
		const auto HalfWidth = static_cast<float>(GetScreenWidth()) * .5f;
		const auto PlayerHeight = static_cast<float>(GetScreenHeight()) - (radius * 2);
		position = { HalfWidth,PlayerHeight };
	}

	void Damage() noexcept override
	{
		--currHealth;
		if (currHealth <= 0)
		{
			isAlive = false;
		}
	}

	void Update() noexcept override
	{
		// TODO: Move these to an input processing member maybe.

		//Movement
		float direction = 0;
		if (IsKeyDown(KEY_LEFT))
		{
			direction--;
		}
		if (IsKeyDown(KEY_RIGHT))
		{
			direction++;
		}

		position.x += speed * direction;
		const float RightEdge = static_cast<float>(GetScreenWidth()) - radius;
		if (position.x < 0 + radius)
		{
			position.x = radius;
		}
		else if (position.x > RightEdge)
		{
			position.x = RightEdge;
		}

		//Determine frame for animation
		timer += GetFrameTime();

		// TODO: Weird check doing a timer comparison twice. 
		if (timer > 0.4 && activeTexture == 2)
		{
			activeTexture = 0;
			timer = 0;
		}
		else if (timer > 0.4)
		{
			activeTexture++;
			timer = 0;
		}

	}

	void CheckForLaserInput(std::vector<Projectile>& playerLasers)
	{
		if (IsKeyDown(KEY_SPACE))
		{
			//newProjectile.position.y = window_height - 130;
			playerLasers.emplace_back(position, true);
		}
	}
};
