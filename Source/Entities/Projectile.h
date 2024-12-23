#pragma once
#include "raylib.h"

class Projectile
{
public:
	// INITIALIZE PROJECTILE WHILE DEFINING IF ITS PLAYER OR ENEMY 
	Vector2 position = { 0,0 };
	int speed = 15;
	bool active = true;
	bool playerProjectile = true;
	//TODO: Should not be null

	//TODO: This is probably uncessary. Just get starting pos, and add velocity upwards
	// Consider making projectiles AABBs, instead of lines
	// LINE WILL UPDATE WITH POSITION FOR CALCULATIONS
	Vector2 lineStart = { 0, 0 };
	Vector2 lineEnd = { 0, 0 };

	void Update();
	void Render(const Texture2D& texture);
};

