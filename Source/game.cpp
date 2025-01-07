#include "game.h"
#include <iostream>
#include <chrono>
#include "RayUtils.h"
#include <algorithm>
#include <functional>
#include <format>
#include "raymath.h"
#include "VariableSaveSystem.h"
#include <cassert>

//Utilizes predefined screen resolution instead of GetScreenWidth()/GetScreenHeight() in order to precompute the barrier vector.
[[nodiscard]] static constexpr std::vector<Barrier> ResetBarriers(int barrierCount, Vector2 screenSize) noexcept
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
	if (FileExists(levelFileName.c_str()))
	{
		LoadLevelFromFile();
	}
	else
	{
		player.Reset();
		alienArmy.Reset();
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

	MyVariableLoader inFile(levelFileName);
	inFile.Load(player);
	inFile.Load(alienArmy);
	inFile.Load(barriers);
	inFile.Load(background);
}

void Game::SaveLevelToFile() const
{
	MyVariableSaver outFile(levelFileName);
	outFile.Save(player);
	outFile.Save(alienArmy);
	outFile.Save(barriers);
	outFile.Save(background);
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
		for (auto& laser : playerLasers)
		{
			laser.Update();
		}
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

void Game::Render() 
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
		break;
	case State::GAMEPLAY:
		background.Render(textures);
		RenderEntityVector(playerLasers, textures);
		RenderEntityVector(barriers, textures);
		player.Render(textures);
		alienArmy.Render(textures);

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
