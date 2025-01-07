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

	animator.Update();
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

void PlayerShip::Render(const TextureLibrary& textureLib) const
{
	animator.UpdateFrameData(textureLib.playerAnimation);
	Rectangle source = animator.GetSourceRectangle(textureLib.playerAnimation);
	DrawTextureQuick_WithSource(textureLib.playerAnimation.GetTexture(), source, position, renderScale);
}