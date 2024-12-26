#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "MyTexture2D.h"
#include "Projectile.h"
#include "PlayerShip.h"
#include "Barrier.h"
#include "Alien.h"
#include "Leaderboard.h"

//TODO: Convert all enums to enum classes.
enum class State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

struct Game
{
	State gameState = State::STARTSCREEN;

	int score = 0;

	//Entities
	PlayerShip player;
	AlienArmy alienArmy;
	std::vector<Projectile> playerLasers;
	std::vector<Barrier> barriers;
	static constexpr int wallCount = 5;
	Vector2 backgroundPos;

	//Textures
	MyTexture2D playerTexture = MyTexture2D("./Assets/PlayerShip.png");
	MyTexture2D alienTexture = MyTexture2D("./Assets/Alien.png");
	MyTexture2D projectileTexture = MyTexture2D("./Assets/Laser.png");
	MyTexture2D barrierTexture = MyTexture2D("./Assets/Barrier.png");
	MyTexture2D backgroundTexture = MyTexture2D("./Assets/Space Background.png");

	Leaderboard leaderboard;

	void Start();
	void End();

	void Continue();

	void Update();
	void CollisionChecks() noexcept;
	void CleanUpDeadEntities() noexcept;

	void Render();
};

template <typename T>
concept DerivedFromEntity = std::is_base_of_v<BaseEntity, T>;

template <DerivedFromEntity T>
bool CollisionCheck_ProjectileVSEntityVector(Projectile& projectile, std::vector<T>& entityVector) noexcept
{
	for (auto& entity : entityVector)
	{
		if (MyCheckCollision_AABBCircle(projectile.position, projectile.size, entity.position, entity.radius))
		{
			projectile.Damage();
			entity.Damage();
			return true;
		}
	}
	return false;
};