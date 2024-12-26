#include "game.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
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
		Barriers.emplace_back(Vector2{ wall_distance * (i + 1) ,
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

bool Game::CanScoreGoOnLeaderboard() const noexcept
{
	return (Leaderboard.size() < 5 ||
			score > Leaderboard.back().score);
}

void Game::End()
{
	playerLasers.clear();
	Barriers.clear();
	alienArmy.Clear();

	if (CanScoreGoOnLeaderboard())
	{
		gameState = State::INPUTNAME;
	}
	else
	{
		gameState = State::LEADERBOARD;
	}
}

void Game::Continue()
{
	SaveLeaderboard();
	gameState = State::STARTSCREEN;
}

bool FixedCheckCollisionPointRec(const Vector2& point,Rectangle rectangle) noexcept
{
	rectangle.y -= rectangle.height * .5f;
	return CheckCollisionPointRec(point, rectangle);
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
	case State::INPUTNAME:
	{
		//This is required due to a raylib bug in the CheckCollisionPointRec
		
		if (const bool IsMouseOnTextBox = FixedCheckCollisionPointRec(GetMousePosition(), textBox))
		{
			SetMouseCursor(MOUSE_CURSOR_IBEAM);
			if (IsMouseButtonPressed(0))
			{
				textBoxSelected = true;
			}
		}
		else
		{
			SetMouseCursor(MOUSE_CURSOR_DEFAULT);
			if (IsMouseButtonPressed(0))
			{
				textBoxSelected = false;
				textBoxRenderTimer = 0;
			}
		}
		UpdateNameTextBox();
	}
		break;
	case State::LEADERBOARD:
		if (IsKeyPressed(KEY_ENTER))
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
		CollisionCheck_ProjectileVSEntityVector(projectile, Barriers);
		if (CollisionCheck_ProjectileVSEntityVector(projectile, alienArmy.alienSpan))
		{
			score += 100;
		}
	}
	for (auto& alienLaser : alienArmy.alienLasers)
	{
		CollisionCheck_ProjectileVSEntityVector(alienLaser, Barriers);
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
	std::erase_if(Barriers, IsEntityDead);
	std::erase_if(alienArmy.alienSpan, IsEntityDead);
	std::erase_if(alienArmy.alienLasers, IsEntityDead);
}

void Game::UpdateNameTextBox() noexcept
{
	if (!textBoxSelected)
	{
		return;
	}

	textBoxRenderTimer += GetFrameTime();
	if (textBoxRenderTimer > 2)
	{
		textBoxRenderTimer = 0;
	}

	int key = GetCharPressed();
	auto ShouldPerformKeyCheck = [&]()
		{
			const bool WasAKeyPressed = key > 0;
			const bool IsThereSpace = playerName.size() < maxCharactersOnName;
			return WasAKeyPressed && IsThereSpace;
		};

	while (ShouldPerformKeyCheck())
	{
		if (const bool IsKeyValidCharacter = (key >= 32) && (key <= 125))
		{
			playerName += static_cast<char>(key);
		}
		key = GetCharPressed();  // Check next character in the queue
	}
	if (IsKeyPressed(KEY_BACKSPACE) && playerName.size() > 0)
	{
		playerName.pop_back();
	}

	if (IsKeyPressed(KEY_ENTER) && playerName.size() > 0)
	{
		InsertNewHighScore(playerName);
	}
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
		std::ranges::for_each(playerLasers, [&](const Projectile& laser) { laser.Render(ProjectileTexture); });
		std::ranges::for_each(Barriers, [&](const Barrier& barrier) { barrier.Render(BarrierTexture); });
		player.Render(playerTexture);
		alienArmy.Render(AlienTexture, ProjectileTexture);

		DrawText(std::format("Score: {}", score).c_str(), 50, 20, 40, YELLOW);
		DrawText(std::format("Lives: {}", player.currHealth).c_str(), 50, 70, 40, YELLOW);
		break;
	case State::INPUTNAME:
	{
		RenderTextBox();
		DrawText((playerName.size() > 0) ? "PRESS ENTER TO CONTINUE" : "PLEASE INSERT A NAME",
				 600, 800, 40, YELLOW);
		DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
		DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
	}
		break;
	case State::LEADERBOARD:
		DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
		DrawText("LEADERBOARD", 50, 100, 40, YELLOW);
		RenderLeaderboardData();
		break;
	default:
		break;
	}
}

void Game::RenderTextBox() const noexcept
{
	const int textBoxThickness = 3;
	DrawRectangleRec(textBox, LIGHTGRAY);
	DrawRectangleLinesEx(textBox, textBoxThickness, textBoxSelected ? RED : DARKGRAY);

	const auto textX = static_cast<int>(textBox.x);
	const auto textY = static_cast<int>(textBox.y);
	DrawText(playerName.c_str(), textX + 5, textY + 8, 40, MAROON);
	DrawText(std::format("INPUT CHARS: {}/{}", playerName.size(), 8).c_str(), 600, 600, 20, YELLOW);
	if (textBoxSelected && playerName.size() < maxCharactersOnName && textBoxRenderTimer > 1.f)
	{
		DrawText("_", textX + maxCharactersOnName + MeasureText(playerName.c_str(), 40), textY + 12, 40, MAROON);
	}
}

void Game::RenderLeaderboardData() const noexcept
{
	int yOffset = 0;
	for (const auto& data : Leaderboard)
	{
		DrawText(data.name.c_str(), 50, 140 + yOffset, 40, YELLOW);
		DrawText(std::format("{}", data.score).c_str(), 350, 140 + yOffset, 40, YELLOW);
		yOffset += 40;
	}
}

void Game::InsertNewHighScore(std::string_view name) noexcept
{
	Leaderboard.emplace_back(name.data(), score);
	std::ranges::sort(Leaderboard, [&](const PlayerData& a, const PlayerData& b){ return a.score > b.score; });
	if (Leaderboard.size() > 5)
	{
		Leaderboard.pop_back();
	}
	gameState = State::LEADERBOARD;
}

void Game::LoadLeaderboard()
{
	//TODO: Maybe Implement this?
	
	// CLEAR LEADERBOARD

	// OPEN FILE

	// READ DATA

	// WRITE DATA ONTO LEADERBOARD

	//CLOSE FILE
}

void Game::SaveLeaderboard()
{
	// SAVE LEADERBOARD AS ARRAY

	// OPEN FILE
	std::fstream file;

	//TODO: Call close and throw this into a RAII class
	file.open("Leaderboard");

	if (!file)
	{
		std::cout << "file not found \n";

	}
	else
	{
		std::cout << "file found \n";
	}
	// CLEAR FILE

	// WRITE ARRAY DATA INTO FILE

	// CLOSE FILE
}


bool Game::CheckCollisions(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd)
{
	//TODO: This whole function should probably be rewritten. For our purposes, we only need to do compare two circles

	// our objective is to calculate the distance between the closest point on the line to the centre of the circle, 
	// and determine if it is shorter than the radius.

	// check if either edge of line is within circle
	if (pointInCircle(circlePos, circleRadius, lineStart) || pointInCircle(circlePos, circleRadius, lineEnd))
	{
		return true;
	}

	// simplify variables
	Vector2 A = lineStart;
	Vector2 B = lineEnd;
	Vector2 C = circlePos;

	// calculate the length of the line
	float length = lineLength(A, B);

	// calculate the dot product
	float dotP = (((C.x - A.x) * (B.x - A.x)) + ((C.y - A.y) * (B.y - A.y))) / pow(length, 2);

	// use dot product to find closest point
	float closestX = A.x + (dotP * (B.x - A.x));
	float closestY = A.y + (dotP * (B.y - A.y));

	//find out if coordinates are on the line.
	// we do this by comparing the distance of the dot to the edges, with two vectors
	// if the distance of the vectors combined is the same as the length the point is on the line

	//since we are using floating points, we will allow the distance to be slightly innaccurate to create a smoother collision
	float buffer = 0.1;

	float closeToStart = lineLength(A, { closestX, closestY }); //closestX + Y compared to line Start
	float closeToEnd = lineLength(B, { closestX, closestY });	//closestX + Y compared to line End

	float closestLength = closeToStart + closeToEnd;

	if (closestLength == length + buffer || closestLength == length - buffer)
	{
		//Point is on the line!

		//Compare length between closest point and circle centre with circle radius

		float closeToCentre = lineLength(A, { closestX, closestY }); //closestX + Y compared to circle centre

		if (closeToCentre < circleRadius)
		{
			//Line is colliding with circle!
			return true;
		}
		else
		{
			//Line is not colliding
			return false;
		}
	}
	else
	{
		// Point is not on the line, line is not colliding
		return false;
	}
}
