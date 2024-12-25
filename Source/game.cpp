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

void Game::End()
{
	//SAVE SCORE AND UPDATE SCOREBOARD
	playerLasers.clear();
	Barriers.clear();
	alienArmy.alienSpan.clear();
	alienArmy.alienLasers.clear();
	newHighScore = CheckNewHighScore();
	gameState = State::ENDSCREEN;
}

void Game::Continue()
{
	SaveLeaderboard();
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
		//Code
	
		//Exit endscreen
		if (IsKeyPressed(KEY_ENTER) && !newHighScore)
		{
			//TODO: Add an early exit
			Continue();
		}
	
		//TODO: Make the check for newHighScore. Rn its done elsewhere, which it shouldnt be.
		if (newHighScore)
		{
			//TODO: Throw cursor changing into its own function.

			if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
			else mouseOnText = false;

			if (mouseOnText)
			{
				// Set the window's cursor to the I-Beam
				SetMouseCursor(MOUSE_CURSOR_IBEAM);

				// Get char pressed on the queue
				int key = GetCharPressed();

				// Check if more characters have been pressed on the same frame
				while (key > 0)
				{
					// NOTE: Only allow keys in range [32..125]
					if ((key >= 32) && (key <= 125) && (letterCount < 9))
					{
						name[letterCount] = (char)key;
						name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
						letterCount++;
					}

					key = GetCharPressed();  // Check next character in the queue
				}

				//Remove chars 
				if (IsKeyPressed(KEY_BACKSPACE))
				{
					letterCount--;
					if (letterCount < 0) letterCount = 0;
					name[letterCount] = '\0';
				}
			}
			else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

			if (mouseOnText)
			{
				framesCounter++;
			}
			else
			{
				framesCounter = 0;
			}

			// If the name is right legth and enter is pressed, exit screen by setting highscore to false and add 
			// name + score to scoreboard
			if (letterCount > 0 && letterCount < 9 && IsKeyPressed(KEY_ENTER))
			{
				std::string nameEntry(name);

				InsertNewHighScore(nameEntry);

				newHighScore = false;
			}
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


void Game::Render()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		//TODO: Create a shorthand for this
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
		break;
	case State::GAMEPLAY:
		DrawTextureQuick(backgroundTexture.get(), backgroundPos, 1.1f);
		std::ranges::for_each(playerLasers, [&](const Projectile& laser)
							  {
								  laser.Render(ProjectileTexture);
							  });
		std::ranges::for_each(Barriers, [&](const Barrier& barrier)
							  {
								  barrier.Render(BarrierTexture);
							  });
		player.Render(playerTexture);
		alienArmy.Render(AlienTexture, ProjectileTexture);

		DrawText(std::format("Score: {}", score).c_str(), 50, 20, 40, YELLOW);
		DrawText(std::format("Lives: {}", player.currHealth).c_str(), 50, 70, 40, YELLOW);
		break;
	case State::ENDSCREEN:
		//Code
		//DrawText("END", 50, 50, 40, YELLOW);

		if (newHighScore)
		{
			DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);

			// BELOW CODE IS FOR NAME INPUT RENDER
			DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

			DrawRectangleRec(textBox, LIGHTGRAY);
			if (mouseOnText)
			{
				// HOVER CONFIRMIATION
				DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
			}
			else
			{
				DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
			}

			//Draw the name being typed out
			DrawText(name, (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

			//Draw the text explaining how many characters are used
			DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

			//TODO: Another mouseontext check? They should be handled in the same thing.
			if (mouseOnText)
			{
				if (letterCount < 9)
				{
					// Draw blinking underscore char
					if (((framesCounter / 20) % 2) == 0)
					{
						DrawText("_", (int)textBox.x + 8 + MeasureText(name, 40), (int)textBox.y + 12, 40, MAROON);
					}
				}
				else
				{
					//Name needs to be shorter
					DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
				}
			}

			// Explain how to continue when name is input
			if (letterCount > 0 && letterCount < 9)
			{
				DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
			}
		}
		else {
			// If no highscore or name is entered, show scoreboard and call it a day
			DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

			DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

			for (int i = 0; i < Leaderboard.size(); i++)
			{
				//TODO: Get rid of draw pointer interpretation, or throw it in a wrapper func.
				char* tempNameDisplay = Leaderboard[i].name.data();
				DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
				DrawText(TextFormat("%i", Leaderboard[i].score), 350, 140 + (i * 40), 40, YELLOW);
			}
		}
		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
}

bool Game::CheckNewHighScore()
{
	if (score > Leaderboard[4].score)
	{
		return true;
	}
	return false;
}

void Game::InsertNewHighScore(std::string name)
{
	PlayerData newData;
	newData.name = name;
	newData.score = score;
	
	//TODO: Maybe replace this with some sort of algo for sorting? Then again, we know its sorted, we just need to insert one new element.
	// Having an actual sort makes sure that it will always be sorted - if for some reason it gets unsorted, this would never resort it without the sort.
	for (int i = 0; i < Leaderboard.size(); i++)
	{
		if (newData.score > Leaderboard[i].score)
		{

			Leaderboard.insert(Leaderboard.begin() + i, newData);

			Leaderboard.pop_back();

			i = Leaderboard.size();

		}
	}
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


bool Game::CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineStart, Vector2 lineEnd)
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
