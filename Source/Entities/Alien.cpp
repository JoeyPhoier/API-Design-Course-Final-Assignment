#include "Alien.h"
#include "RayUtils.h"
#include <algorithm>
#include "Collision.h"'
#include <functional>

float Alien::speed = 100;
bool Alien::shouldMoveDownThisFrame = false;

void Alien::Update() noexcept
{
	position.x += speed * GetFrameTime();

	if	(!shouldMoveDownThisFrame)
	{
		const auto screenWidth = static_cast<float>(GetScreenWidth());
		if (position.x >= screenWidth - radius)
		{
			shouldMoveDownThisFrame = true;
			speed = -abs(speed);
		}
		else if (position.x <= radius)
		{
			shouldMoveDownThisFrame = true;
			speed = abs(speed);
		}
	}
}

//ALIEN ARMY
void AlienArmy::ResetArmy() noexcept
{
	alienSpan.clear();

	const Vector2 armySize = { (formationSize.x - 1) * alienSpacing * .5f,
							   (formationSize.y - 1) * alienSpacing * .5f };

	const auto screenWidth = static_cast<float>(GetScreenWidth());
	position = { screenWidth * .5f,
				armySize.y + alienSpacing };

	const Vector2 firstPos = position - armySize;

	for (int row = 0; row < formationSize.y; ++row)
	{
		for (int collumn = 0; collumn < formationSize.x; ++collumn)
		{
			alienSpan.emplace_back(Vector2{
				firstPos.x + (collumn * alienSpacing),
				firstPos.y + (row * alienSpacing)
								   });
		}
	}
}

void AlienArmy::Update() noexcept
{
	//Spawn new aliens if aliens run out
	if (alienSpan.size() < 1)
	{
		ResetArmy();
		return;
	}

	std::ranges::for_each(alienSpan, std::mem_fn(&Alien::Update));
	std::ranges::for_each(alienLasers, std::mem_fn(&Projectile::Update));

	if (Alien::shouldMoveDownThisFrame)
	{
		std::ranges::for_each(alienSpan, [&](Alien& alien) noexcept
							  {
								  alien.position.y += 50;
							  });
		Alien::shouldMoveDownThisFrame = false;
	}

	currLaserCooldown -= GetFrameTime();
	if (const bool CanFireLaser = currLaserCooldown <= 0)
	{
		currLaserCooldown = maxLaserCooldown;

		int randomAlienIndex = 0;
		//TODO: Make only the lower aliens shoot
		if (alienSpan.size() > 1)
		{
			randomAlienIndex = rand() % alienSpan.size();
		}
		alienLasers.emplace_back(alienSpan[randomAlienIndex].position, false);
	}
}

bool AlienArmy::HasAlienReachedPlayer(const Vector2& playerPosition, const float playerRadius) const noexcept
{
	return std::ranges::any_of(alienSpan, [&](const Alien& alien)
							   {
								   return (alien.position.y + alien.radius > playerPosition.y - playerRadius);
							   });
}

void AlienArmy::Render(const Texture2D& alienTexture, const Texture2D& projectileTexture) const noexcept
{
	std::ranges::for_each(alienSpan, [&](const Alien& alien)
						  {
							  alien.Render(alienTexture);
						  });
	std::ranges::for_each(alienLasers, [&](const Projectile& laser)
						  {
							  laser.Render(projectileTexture);
						  });
}
