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
#include <type_traits>
#include <utility>

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
	TextureManager textures;

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
	static constexpr int defaultBarrierCount = 5;
	Background background;

	void StartGameplay();
	void EndGameplay();

	void LoadLevelFromFile();
	void SaveLevelToFile() const;

	void Update();
	void CollisionChecks() noexcept;
	void CleanUpDeadEntities();

	void Render() const noexcept;
public:
	void Loop();
};

template <typename V>
concept VectorOfDerivedFromEntity = std::is_same_v<V, std::vector<typename std::remove_cvref_t<V>::value_type>> &&
									std::is_base_of_v<BaseEntity, typename V::value_type>;

bool CollisionCheck_ProjectileVSEntityVector(Projectile& projectile, VectorOfDerivedFromEntity auto& entityVector) noexcept
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