#include "game.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include "RayUtils.h"
#include "Collision.h"
#include <algorithm>
#include <functional>
#include <format>

// MATH FUNCTIONS
float lineLength(Vector2 A, Vector2 B) //Uses pythagoras to calculate the length of a line
{
	float length = sqrtf(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));

	return length;
}

bool pointInCircle(Vector2 circlePos, float radius, Vector2 point) // Uses pythagoras to calculate if a point is within a circle or not
{
	float distanceToCentre = lineLength(circlePos, point);

	if (distanceToCentre < radius)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Game::Start()
{
	// creating walls 
	float window_width = (float)GetScreenWidth(); 
	float window_height = (float)GetScreenHeight(); 
	float wall_distance = window_width / (wallCount + 1); 
	for (int i = 0; i < wallCount; i++)
	{
		barriers.emplace_back(Vector2{ wall_distance * (i + 1) ,
									   window_height - 250 });
	}

	//creating player
	PlayerShip newPlayer;
	player = newPlayer;

	//creating aliens
	alienArmy.ResetArmy();

	//reset score
	score = 0;

	backgroundPos.y = window_height * .5f;

	gameState = State::GAMEPLAY;
}

void Game::End()
{
	playerLasers.clear();
	barriers.clear();
	alienArmy.Clear();

	leaderboard.PrepareLeaderboard(score);
	gameState = State::ENDSCREEN;
}

void Game::Continue()
{
	//TODO: Reset
	gameState = State::STARTSCREEN;
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		//Code 
		//TODO: replace all usages of keyReleased with keydown.
		if (IsKeyPressed(KEY_SPACE))
		{
			Start();
		}

		break;
	case State::GAMEPLAY:
		//Code
		if (IsKeyPressed(KEY_Q))
		{
			End();
		}

		player.Update();
		player.CheckForLaserInput(playerLasers);
		backgroundPos.x = (static_cast<float>(GetScreenWidth()) * .5f) - (player.position.x / 15);
		std::ranges::for_each(playerLasers, std::mem_fn(&Projectile::Update));
		alienArmy.Update();
		if (alienArmy.HasAlienReachedPlayer(player.position, player.radius))
		{
			End();
		}
		
		CollisionChecks();
		CleanUpDeadEntities();
	break;
	case State::ENDSCREEN:
		leaderboard.Update(score);
		if (leaderboard.CanExitLeaderboard())
		{
			//TODO: Make a proper restart game func
			Continue();
		}
		break;
	default:
		break;
	}
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
	//End game if player dies
	if (!player.IsAlive())
	{
		End();
		return;
	}

	// REMOVE INACTIVE/DEAD ENITITIES
	auto IsEntityDead = [&](const BaseEntity& entity) noexcept
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
		DrawTextureQuick(backgroundTexture.get(), backgroundPos, 1.1f);
		std::ranges::for_each(playerLasers, [&](const Projectile& laser) { laser.Render(projectileTexture); });
		std::ranges::for_each(barriers, [&](const Barrier& barrier) { barrier.Render(barrierTexture); });
		player.Render(playerTexture);
		alienArmy.Render(alienTexture, projectileTexture);

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
