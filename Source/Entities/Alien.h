#pragma once
#include "raylib.h"
#include "MyTexture2D.h"

struct Alien
{
public:
	//TODO: Is this necessary? If all textures are rendered with base color, get rid of this.
	Color color = WHITE;
	Vector2 position = { 0, 0 };
	//TODO: Why do they need a position and an x and y?
	int x = 0;
	int y = 0;
	//TODO: Should be constexpr and static
	float radius = 30;
	bool active = true;
	bool moveRight = true;


	int speed = 2;

	void Update();
	void Render(const Texture2D& texture) const noexcept;
};
