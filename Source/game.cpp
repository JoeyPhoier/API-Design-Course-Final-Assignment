#include "game.h"
#include <iostream>
#include <chrono>
#include "RayUtils.h"
#include <algorithm>
#include <functional>
#include <format>
#include "raymath.h"
#include <fstream>
#include <cassert>

//Utilizes predefined screen resolution instead of GetScreenWidth()/GetScreenHeight() in order to precompute the barrier vector.
static constexpr std::vector<Barrier> ResetBarriers(int barrierCount, Vector2 screenSize) noexcept
{
	std::vector<Barrier> barrierVector(barrierCount);
	const auto barrierCountf = static_cast<float>(barrierCount);
	const float wallSpacing = screenSize.x / (barrierCountf + 1.f);
	const float wallHeight = screenSize.y - 250;
	int i = 1;
	for (auto& barrier : barrierVector)
	{
		barrier.position = Vector2{ wallSpacing * i , wallHeight };
		++i;
	}
	return barrierVector;
}

void Game::StartGameplay()
{
	score = 0;
	if (FileExists("Level.sig"))
	{
		LoadLevelFromFile();
	}
	else
	{
		player.Reset();
		alienArmy.ResetArmy();
		barriers = ResetBarriers(defaultBarrierCount, resolution);
		static_assert(ResetBarriers(defaultBarrierCount, resolution).size() == defaultBarrierCount);
		background.Reset();
		SaveLevelToFile();
	}
	gameState = State::GAMEPLAY;
}

void Game::EndGameplay()
{
	alienArmy.Clear();
	playerLasers.clear();
	barriers.clear();

	leaderboard.PrepareLeaderboard(score);
	gameState = State::ENDSCREEN;
}

void Game::LoadLevelFromFile()
{
	alienArmy.Clear();
	barriers.clear();

	std::ifstream inFile("Level.sig", std::ios::binary);
	if (inFile.fail())
	{
		return;
	}
	inFile.read(std::bit_cast<char*>(&player), sizeof(PlayerShip));
	size_t alienCount = 0;
	inFile.read(std::bit_cast<char*>(&alienCount), sizeof(size_t));
	alienArmy.alienSpan.resize(alienCount);
	for (auto& alien : alienArmy.alienSpan)
	{
		inFile.read(std::bit_cast<char*>(&alien.position), sizeof(Vector2));
	}
	size_t barrierCount = 0;
	inFile.read(std::bit_cast<char*>(&barrierCount), sizeof(size_t));
	barriers.resize(barrierCount);
	inFile.read(std::bit_cast<char*>(&barriers.front()), sizeof(Barrier) * barrierCount);
	inFile.read(std::bit_cast<char*>(&background), sizeof(Background));
}

void Game::SaveLevelToFile() const
{
	std::ofstream outFile("Level.sig", std::ios::binary);
	outFile.write(std::bit_cast<const char*>(&player), sizeof(player));
	const size_t alienCount = alienArmy.alienSpan.size();
	outFile.write(std::bit_cast<const char*>(&alienCount), sizeof(size_t));
	for (auto& alien : alienArmy.alienSpan)
	{
		outFile.write(std::bit_cast<const char*>(&alien.position), sizeof(Vector2));
	}
	const size_t barrierCount = barriers.size();
	outFile.write(std::bit_cast<const char*>(&barrierCount), sizeof(size_t));
	outFile.write(std::bit_cast<const char*>(&barriers.front()), sizeof(Barrier) * barrierCount);
	outFile.write(std::bit_cast<const char*>(&background), sizeof(background));
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		if (IsKeyPressed(KEY_SPACE))
		{
			StartGameplay();
		}
		break;
	case State::GAMEPLAY:
		if (IsKeyPressed(KEY_Q))
		{
			EndGameplay();
		}

		player.Update();
		player.CheckForLaserInput(playerLasers);
		std::ranges::for_each(playerLasers, std::mem_fn(&Projectile::Update));
		alienArmy.Update();
		if (alienArmy.HasAlienReachedPlayer(player.position, PlayerShip::radius))
		{
			EndGameplay();
		}
		background.Update(player.position);

		CollisionChecks();
		CleanUpDeadEntities();
		break;
	case State::ENDSCREEN:
		leaderboard.Update(score);
		if (leaderboard.ShouldExitLeaderboard())
		{
			gameState = State::STARTSCREEN;
		}
		break;
	default:
		break;
	}
}

static bool MyCheckCollision_AABBCircle(const Vector2& boxPosition, const Vector2& boxSize, const Vector2& circlePosition, const float& circleRadius) noexcept
{
	const Vector2 topLeftCorner = boxPosition - (boxSize * .5f);
	const Vector2 bottomRightCorner = boxPosition + (boxSize * .5f);
	const Vector2 closestPoint = Vector2Clamp(circlePosition, topLeftCorner, bottomRightCorner);

	const bool IsClosestPointInCircle = circleRadius >= Vector2Distance(circlePosition, closestPoint);
	return IsClosestPointInCircle;
}

void Game::CollisionChecks() noexcept
{
	for (auto& projectile : playerLasers)
	{
		CollisionCheck_ProjectileVSEntityVector(projectile, barriers);
		if (CollisionCheck_ProjectileVSEntityVector(projectile, alienArmy.alienSpan))
		{
			score += 100;
		}
	}
	for (auto& alienLaser : alienArmy.alienLasers)
	{
		CollisionCheck_ProjectileVSEntityVector(alienLaser, barriers);
		if (MyCheckCollision_AABBCircle(alienLaser.position, Projectile::size, player.position, PlayerShip::radius))
		{
			alienLaser.Damage();
			player.Damage();
		}
	}
}

void Game::CleanUpDeadEntities()
{
	if (!player.IsAlive())
	{
		EndGameplay();
		return;
	}

	auto IsEntityDead = [](const BaseEntity& entity) noexcept
		{
			return !entity.IsAlive();
		};
	std::erase_if(playerLasers, IsEntityDead);
	std::erase_if(barriers, IsEntityDead);
	std::erase_if(alienArmy.alienSpan, IsEntityDead);
	std::erase_if(alienArmy.alienLasers, IsEntityDead);
}

void Game::Render() const noexcept
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
		break;
	case State::GAMEPLAY:
		background.Render(textures.backgroundTexture);
		RenderEntityVector(playerLasers, textures.projectileTexture);
		RenderEntityVector(barriers, textures.barrierTexture);
		player.Render(textures.playerTexture);
		alienArmy.Render(textures.alienTexture, textures.projectileTexture);

		DrawText(std::format("Score: {}", score).c_str(), 50, 20, 40, YELLOW);
		DrawText(std::format("Lives: {}", player.currHealth).c_str(), 50, 70, 40, YELLOW);
		break;
	case State::ENDSCREEN:
		leaderboard.Render();
		break;
	default:
		break;
	}
}

void Game::Loop()
{
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		Update();

		BeginDrawing();
		ClearBackground(BLACK);
		Render();
		EndDrawing();
	}
}
