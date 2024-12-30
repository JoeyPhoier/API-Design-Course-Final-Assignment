#include "PlayerShip.h"


void PlayerShip::Reset() noexcept
{
	const auto HalfWidth = static_cast<float>(GetScreenWidth()) * .5f;
	const auto PlayerHeight = static_cast<float>(GetScreenHeight()) - (radius * 2);
	position = { HalfWidth,PlayerHeight };

	currHealth = maxHealth;
	currLaserCooldown = 0;
}

void PlayerShip::Update() noexcept 
{
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

	currSpriteTimer += GetFrameTime();
	if (currSpriteTimer >= maxSpriteTimer)
	{
		currSpriteTimer = 0;
		activeTexture = (activeTexture + 1) % 3;
	}
}

void PlayerShip::CheckForLaserInput(std::vector<Projectile>& playerLasers) noexcept
{
	if (currLaserCooldown > 0)
	{
		currLaserCooldown -= std::min(currLaserCooldown, GetFrameTime());
		return;
	}

	const bool CanFire = currLaserCooldown == 0;
	if (IsKeyDown(KEY_SPACE) && CanFire)
	{
		currLaserCooldown = maxLaserCooldown;
		playerLasers.emplace_back(position, true);
	}
}

void PlayerShip::Render(const Texture2D& texture) const noexcept
{
	const auto spriteSquareSide = static_cast<float>(texture.height);
	const Rectangle source = { activeTexture * spriteSquareSide, 0, spriteSquareSide, spriteSquareSide };
	const Rectangle dest = {
		position.x,
		position.y,
		spriteSquareSide * renderScale,
		spriteSquareSide * renderScale,
	};
	const Vector2 origin = {
		spriteSquareSide * 0.5f * renderScale,
		spriteSquareSide * 0.5f * renderScale
	};
	DrawTexturePro(texture, source, dest, origin, 0, WHITE);
}