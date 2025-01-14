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
#include "Managers.h"
#include "TextureLibrary.h"

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
		const float halfScreenWidth = static_cast<float>(GetScreenWidth()) * .5f;
		position.x = halfScreenWidth - ((playerPosition.x - halfScreenWidth) / 15);
	}
	void Render(const TextureLibrary& textureLib) const noexcept
	{
		DrawTextureQuick(textureLib.backgroundTexture, position, renderScale);
	}
};

class Game final
{
public:
	static constexpr Vector2 resolution = { 1920,1080 };
private:
	std::string windowName = "SPACE INVADERS";
	WindowManager window = WindowManager(resolution, windowName);
	AudioManager audio;
	TextureLibrary textures;

	std::string levelFileName = "Level.sig";

	enum class State
	{
		STARTSCREEN,
		GAMEPLAY,
		ENDSCREEN
	};
	State gameState = State::STARTSCREEN;

	Leaderboard leaderboard;
	Leaderboard::ScoreType score = 0;

	//Entities
	PlayerShip player;
	AlienArmy alienArmy;
	std::vector<Projectile> playerLasers;
	std::vector<Barrier> barriers;
	static constexpr int defaultBarrierCount = 5;
	Background background;

	void StartGameplay();
	void EndGameplay();

	void LoadLevelFromFile();
	void SaveLevelToFile() const;

	void Update();
	void CollisionChecks() noexcept;
	void CleanUpDeadEntities();

	void Render();
public:
	void Loop();
};

static bool CollisionCheck_ProjectileVSEntityVector(Projectile& projectile, is_VectorOfEntityClasses auto& entityVector) noexcept
{
	for (auto& entity : entityVector)
	{
		if (MyCheckCollision_AABBCircle(projectile.position, Projectile::size, entity.position, entity.radius))
		{
			projectile.Damage();
			entity.Damage();
			return true;
		}
	}
	return false;
};