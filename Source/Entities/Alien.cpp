#include "Alien.h"
#include "RayUtils.h"

float Alien::speed = 2;
bool Alien::shouldMoveDownThisFrame = false;

void Alien::Update() noexcept
{
	position.x += speed;

	if	(!shouldMoveDownThisFrame)
	{
		const auto screenWidth = static_cast<float>(GetScreenWidth());
		if (position.x >= screenWidth || position.x <= 0)
		{
			shouldMoveDownThisFrame = true;
			speed *= -1;
		}
	}
}

void Alien::Render(const Texture2D& texture) const noexcept
{
	DrawTextureQuick(texture, position, renderScale);
}
