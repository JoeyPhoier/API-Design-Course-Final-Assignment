#include "Barrier.h"
#include "RayUtils.h"
#include <format>

void Barrier::Render(const Texture2D& texture)  const noexcept
{
	DrawTextureQuick(texture, position, renderScale);
	DrawText(std::format("{}", currHealth).c_str(),
			 static_cast<int>(position.x + textOffset.x) ,
			 static_cast<int>(position.y + textOffset.y) ,
			 textSize, RED);
}
