#pragma once
#include "raylib.h"
#include "MyTexture2D.h"

class Barrier
{
public:
	//TODO: Does not need both a position and a rectangle. 
	Vector2 position;
	Rectangle rec;
	//TODO: Do all entities need an active flag? Consider getting rid of them.
	bool active;
	//TODO: Do all walls need a color member? If so, should probably be static.
	Color color;
	//TODO: I would like to see the max health be a static and constexpr value.
	int health = 50;
	int radius = 60;

	void Render(const Texture2D& texture);
	void Update();
};
