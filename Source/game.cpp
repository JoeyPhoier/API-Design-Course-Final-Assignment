#include "game.h"
#include <iostream>
#include <chrono>
#include "RayUtils.h"
#include <algorithm>
#include <functional>
#include <format>
#include "raymath.h"

void ResetBarriers(std::vector<Barrier>& barrierVector, int barrierCount) noexcept
{
	const auto barrierCountf = static_cast<float>(barrierCount + 1);
	const float wallSpacing = static_cast<float>(GetScreenWidth()) / barrierCountf;
	const float wallHeight = static_cast<float>(GetScreenHeight()) - 250;
	for (int i = 1; i <= barrierCount; i++)
	{
		barrierVector.emplace_back(Vector2{ wallSpacing * i , wallHeight });
	}
}

void Game::StartGameplay() noexcept
{
	score = 0;

	player.Reset();
	alienArmy.ResetArmy();
	ResetBarriers(barriers, barrierCount);
	background.Reset();
	gameState = State::GAMEPLAY;
}

void Game::EndGameplay() noexcept
{
	alienArmy.Clear();
	playerLasers.clear();
	barriers.clear();

	leaderboard.PrepareLeaderboard(score);
	gameState = State::ENDSCREEN;
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
		if (alienArmy.HasAlienReachedPlayer(player.position, player.radius))
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
		if (MyCheckCollision_AABBCircle(alienLaser.position, alienLaser.size, player.position, player.radius))
		{
			alienLaser.Damage();
			player.Damage();
		}
	}
}

void Game::CleanUpDeadEntities() noexcept
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
		std::ranges::for_each(playerLasers, [&](const Projectile& laser)
							  {
								  laser.Render(textures.projectileTexture);
							  });
		std::ranges::for_each(barriers, [&](const Barrier& barrier)
							  {
								  barrier.Render(textures.barrierTexture);
							  });
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

void Game::Loop() noexcept
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
