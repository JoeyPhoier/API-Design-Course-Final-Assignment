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
#include "Resources.h"

class Background final
{
	Vector2 position = { 0,0 };
	static constexpr float renderScale = 1.1f;
public:
	void Reset() noexcept
	{
		position.y = static_cast<float>(GetScreenHeight()) * .5f;
	}
	void Update(Vector2 playerPosition) noexcept
	{
		position.x = (static_cast<float>(GetScreenWidth()) * .5f) - (playerPosition.x / 15);
	}
	void Render(const Texture2D& texture) const noexcept
	{
		DrawTextureQuick(texture, position, renderScale);
	}
};

class Game final
{
	Vector2 resolution = { 1920,1080 };
	std::string windowName = "SPACE INVADERS";
	WindowManager window = WindowManager(resolution, windowName);
	AudioManager audio;

	enum class State
	{
		STARTSCREEN,
		GAMEPLAY,
		ENDSCREEN
	};
	State gameState = State::STARTSCREEN;

	Leaderboard leaderboard;
	int score = 0;

	//Entities
	PlayerShip player;
	AlienArmy alienArmy;
	std::vector<Projectile> playerLasers;
	std::vector<Barrier> barriers;
	static constexpr int barrierCount = 5;
	Background background;

	TextureLibrary textures;


	void StartGameplay();
	void EndGameplay() noexcept;

	void Update();
	void CollisionChecks() noexcept;
	void CleanUpDeadEntities() noexcept;

	void Render();
public:
	void Loop();
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