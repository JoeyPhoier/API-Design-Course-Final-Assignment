#include "Alien.h"
#include "RayUtils.h"
#include <random>
#include "game.h"

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
[[nodiscard]] static constexpr std::vector<Alien> ResetAlienSpan(Vector2Int formationSize, float alienSpacing) noexcept
{
	std::vector<Alien> alienSpanTemp(formationSize.x * formationSize.y);

	const Vector2 armyPixelSize = { (formationSize.x - 1) * alienSpacing * .5f,
							   (formationSize.y - 1) * alienSpacing * .5f };
	const float screenWidth = Game::resolution.x;
	const Vector2 armyPosition{ screenWidth * .5f,
								armyPixelSize.y + alienSpacing };
	Vector2 alienPos = armyPosition - armyPixelSize;

	int collumCounter = 0;
	for (auto& alien : alienSpanTemp)
	{
		alien.position = alienPos;
		alienPos.x += alienSpacing;
		++collumCounter;
		if (collumCounter >= formationSize.x)
		{
			collumCounter = 0;
			alienPos.x = armyPosition.x - armyPixelSize.x;
			alienPos.y += alienSpacing;
		}
	}
	return alienSpanTemp;
}

void AlienArmy::Reset() noexcept
{
	alienLasers.clear();
	alienSpan = ResetAlienSpan(formationSize, alienSpacing);
	static_assert(ResetAlienSpan(formationSize, alienSpacing).size() == formationSize.x * formationSize.y);
}

[[nodiscard]] static Vector2 GetLowestAlienPositionFromRandomCollum(const std::vector<Alien>& alienVector) noexcept
{
	std::vector<Vector2> lowestAlienPosition;
	for (const auto& alien : alienVector)
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
	std::minstd_rand randomInt;
	return lowestAlienPosition[randomInt() % lowestAlienPosition.size()];
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
		Vector2 FiringAlienPosition = GetLowestAlienPositionFromRandomCollum(alienSpan);
		alienLasers.emplace_back(FiringAlienPosition, false);
	}
}

void AlienArmy::Update() noexcept
{
	//Spawn new aliens if aliens run out
	if (alienSpan.size() < 1)
	{
		Reset();
		return;
	}

	for (auto& alien : alienSpan)
	{
		alien.Update();
	}
	for (auto& laser : alienLasers)
	{
		laser.Update();
	}

	if (Alien::shouldMoveDownThisFrame)
	{
		for (auto& alien : alienSpan)
		{
			alien.position.y += 50;
		}
		Alien::shouldMoveDownThisFrame = false;
		Alien::speed *= -1;
	}

	UpdateAlienShooting();
}
