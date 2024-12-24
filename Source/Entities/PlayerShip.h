#pragma once
#include "raylib.h"
#include "MyTexture2D.h"
#include <vector>

struct PlayerShip
{
public:

	float x_pos = 0;
	float speed = 7;
	float player_base_height = 70.0f;
	float radius = 50;
	int lives = 3;
	int direction = 0;
	int activeTexture = 0;
	float timer = 0;

	void Initialize();
	void Render(const Texture2D& texture);
	void Update();
};
