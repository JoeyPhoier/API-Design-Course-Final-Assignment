#include "Alien.h"
#include "RayUtils.h"
#include <algorithm>
#include <functional>

float Alien::speed = 100;
bool Alien::shouldMoveDownThisFrame = false;

void Alien::Update() noexcept
{
	position.x += speed * GetFrameTime();

	if	(!shouldMoveDownThisFrame)
	{
		const auto screenWidth = static_cast<float>(GetScreenWidth());
		if ((speed > 0 && position.x >= screenWidth - radius) ||
			(speed < 0 && position.x <= radius))
		{
			shouldMoveDownThisFrame = true;
		}
	}
}

//ALIEN ARMY
void AlienArmy::ResetArmy() noexcept
{
	alienSpan.clear();

	const Vector2 armyPixelSize = { (formationSize.x - 1) * alienSpacing * .5f,
							   (formationSize.y - 1) * alienSpacing * .5f };

	const auto screenWidth = static_cast<float>(GetScreenWidth());
	const Vector2 armyPosition{ screenWidth * .5f,
								armyPixelSize.y + alienSpacing };
	const Vector2 firstPos = armyPosition - armyPixelSize;

	for (int collumn = 0; collumn < formationSize.x; ++collumn)
	{
		const float posX = firstPos.x + (collumn * alienSpacing);
		for (int row = 0; row < formationSize.y; ++row)
		{
			alienSpan.emplace_back(Vector2{ posX, firstPos.y + (row * alienSpacing) });
		}
	}
}

[[nodiscard]] Vector2 GetLowestAlienPositionFromRandomCollum(std::vector<Alien> alienVector)
{
	std::vector<Vector2> lowestAlienPosition;
	for (auto& alien : alienVector)
	{
		if (lowestAlienPosition.empty() ||
			alien.position.x != lowestAlienPosition.back().x)
		{
			//Next Collum
			lowestAlienPosition.emplace_back(alien.position);
		}
		else if (alien.position.y > lowestAlienPosition.back().y)
		{
			lowestAlienPosition.back() = alien.position;
		}
	}
	//TODO: Use std sample
	return lowestAlienPosition[rand() % lowestAlienPosition.size()];
}

void AlienArmy::UpdateAlienShooting() noexcept
{ 
	currLaserCooldown -= GetFrameTime();
	if (alienSpan.empty())
	{
		return;
	}
	if (const bool CanFireLaser = currLaserCooldown <= 0)
	{
		currLaserCooldown = maxLaserCooldown;
		alienLasers.emplace_back(GetLowestAlienPositionFromRandomCollum(alienSpan), false);
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
		Alien::speed *= -1;
	}

	UpdateAlienShooting();
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
