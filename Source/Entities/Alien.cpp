#include "Alien.h"
#include "RayUtils.h"
#include <algorithm>
#include "Collision.h"

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

void Alien::Render(const Texture2D& texture) const noexcept
{
	DrawTextureQuick(texture, position, renderScale);
}

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
	std::ranges::for_each(alienSpan, [&](Alien& alien) noexcept
						  {
							  alien.Update();
						  });
	std::ranges::for_each(alienLasers, [&](Projectile& laser) noexcept
						  {
							  laser.Update();
						  });

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
		alienLasers.push_back(Projectile(alienSpan[randomAlienIndex].position, false));
	}
}

void AlienArmy::EraseDeadEntities() noexcept
{
	auto IsDeadA = [&](const Alien& alien)
		{
			return !alien.IsAlive();
		};
	auto lastValidItA = std::remove_if(alienSpan.begin(), alienSpan.end(), IsDeadA);
	alienSpan.erase(lastValidItA, alienSpan.end());

	auto IsDeadL = [&](const Projectile& laser)
		{
			return !laser.IsAlive();
		};
	auto lastValidItL = std::remove_if(alienLasers.begin(), alienLasers.end(), IsDeadL);
	alienLasers.erase(lastValidItL, alienLasers.end());
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
