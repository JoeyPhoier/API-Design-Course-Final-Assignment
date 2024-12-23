#include "game.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include "RayUtils.h"

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
		Wall newWalls;
		newWalls.position.y = window_height - 250; 
		newWalls.position.x = wall_distance * (i + 1); 

		Walls.push_back(newWalls); 
	}

	//creating player
	Player newPlayer;
	player = newPlayer;
	player.Initialize();

	//creating aliens
	SpawnAliens();

	//reset score
	score = 0;

	gameState = State::GAMEPLAY;
}

void Game::End()
{
	//SAVE SCORE AND UPDATE SCOREBOARD
	Projectiles.clear();
	Walls.clear();
	Aliens.clear();
	newHighScore = CheckNewHighScore();
	gameState = State::ENDSCREEN;
}

void Game::Continue()
{
	SaveLeaderboard();
	gameState = State::STARTSCREEN;
}

void Game::Launch()
{
	//TODO: Get rid of this two step init.

	//LOAD SOME RESOURCES HERE
	resources.Load();
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		//Code 
		//TODO: replace all usages of keyReleased with keydown.
		if (IsKeyReleased(KEY_SPACE))
		{
			Start();


		}

		break;
	case State::GAMEPLAY:
		//Code
		if (IsKeyReleased(KEY_Q))
		{
			//TODO: Add an early return or something.
			End();
		}

		//Update Player
		player.Update();
		
		//TODO: Replace with a for range loop or an algo.
		//Update Aliens and Check if they are past player
		for (int i = 0; i < Aliens.size(); i++)
		{
			Aliens[i].Update(); 

			if (Aliens[i].position.y > GetScreenHeight() - player.player_base_height)
			{
				End();
			}
		}

		//TODO: This should probably be processed when the player takes damage, not every frame.
		//End game if player dies
		if (player.lives < 1)
		{
			End();
		}

		//TODO: This should probably be processed when an alien is destroyed, not every frame.
		//Spawn new aliens if aliens run out
		if (Aliens.size() < 1)
		{
			SpawnAliens();
		}

		//TODO: What is the point of this
		// Update background with offset
		playerPos = { player.x_pos, (float)player.player_base_height };
		cornerPos = { 0, (float)player.player_base_height };
		offset = lineLength(playerPos, cornerPos) * -1;
		backgroundPos.x = -player.x_pos / 15;

		//TODO: Replace these with for loops or algos
		//UPDATE PROJECTILE
		for (int i = 0; i < Projectiles.size(); i++)
		{
			Projectiles[i].Update();
		}
		//UPDATE PROJECTILE
		for (int i = 0; i < Walls.size(); i++)
		{
			Walls[i].Update();
		}

		//TODO: Collision checks should be handled in a separate function, or by member methods.
		//CHECK ALL COLLISONS HERE
		for (int i = 0; i < Projectiles.size(); i++)
		{
			if (Projectiles[i].playerProjectile)
			{
				for (int a = 0; a < Aliens.size(); a++)
				{
					if (CheckCollision(Aliens[a].position, Aliens[a].radius, Projectiles[i].lineStart, Projectiles[i].lineEnd))
					{
						// Kill!
						std::cout << "Hit! \n";
						// Set them as inactive, will be killed later
						Projectiles[i].active = false;
						Aliens[a].active = false;
						score += 100;
					}
				}
			}

			//ENEMY PROJECTILES HERE
			for (int i = 0; i < Projectiles.size(); i++)
			{
				if (!Projectiles[i].playerProjectile)
				{
					if (CheckCollision({player.x_pos, GetScreenHeight() - player.player_base_height }, player.radius, Projectiles[i].lineStart, Projectiles[i].lineEnd))
					{
						std::cout << "dead!\n"; 
						Projectiles[i].active = false; 
						player.lives -= 1; 
					}
				}
			}


			for (int b = 0; b < Walls.size(); b++)
			{
				if (CheckCollision(Walls[b].position, Walls[b].radius, Projectiles[i].lineStart, Projectiles[i].lineEnd))
				{
					// Kill!
					std::cout << "Hit! \n";
					// Set them as inactive, will be killed later
					Projectiles[i].active = false;
					Walls[b].health -= 1;
				}
			}
		}

		//TODO: Player shooting should be handled in player update.
		//MAKE PROJECTILE
		if (IsKeyPressed(KEY_SPACE))
		{
			float window_height = (float)GetScreenHeight();
			Projectile newProjectile;
			newProjectile.position.x = player.x_pos;
			newProjectile.position.y = window_height - 130;
			newProjectile.playerProjectile = true;
			Projectiles.push_back(newProjectile);
		}

		//TODO: Should probably be handled by the alien update.
		//Aliens Shooting
		shootTimer += 1;
		if (shootTimer > 59) //once per second
		{
			int randomAlienIndex = 0;

			if (Aliens.size() > 1)
			{
				randomAlienIndex = rand() % Aliens.size();
			}

			Projectile newProjectile;
			newProjectile.position = Aliens[randomAlienIndex].position;
			newProjectile.position.y += 40;
			newProjectile.speed = -15;
			newProjectile.playerProjectile = false;
			Projectiles.push_back(newProjectile);
			shootTimer = 0;
		}

		//TODO: Replace with a for range or algo
		// REMOVE INACTIVE/DEAD ENITITIES
		for (int i = 0; i < Projectiles.size(); i++)
		{
			if (Projectiles[i].active == false)
			{
				Projectiles.erase(Projectiles.begin() + i);
				// Prevent the loop from skipping an instance because of index changes, since all insances after
				// the killed objects are moved down in index. This is the same for all loops with similar function
				i--;
			}
		}
		for (int i = 0; i < Aliens.size(); i++)
		{
			if (Aliens[i].active == false)
			{
				//TODO: Use a friend member predicate for this

				Aliens.erase(Aliens.begin() + i);
				i--;
			}
		}
		for (int i = 0; i < Walls.size(); i++)
		{
			if (Walls[i].active == false)
			{
				Walls.erase(Walls.begin() + i);
				i--;
			}
		}

			
		

	break;
	case State::ENDSCREEN:
		//Code
	
		//Exit endscreen
		if (IsKeyReleased(KEY_ENTER) && !newHighScore)
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
			if (letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER))
			{
				std::string nameEntry(name);

				InsertNewHighScore(nameEntry);

				newHighScore = false;
			}


		}
		


		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
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
		DrawTextureQuick(resources.backgroundTexture, backgroundPos, 1.1f);

		//TODO: Use std::format
		DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
		DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);

		//player rendering 
		//TODO: Player Render shouldnt require anything
		player.Render(resources.shipTextures[player.activeTexture]);

		//TODO: All of these should be in range loops or algos
		//projectile rendering
		for (int i = 0; i < Projectiles.size(); i++)
		{
			Projectiles[i].Render(resources.laserTexture);
		}

		// wall rendering 
		for (int i = 0; i < Walls.size(); i++)
		{
			Walls[i].Render(resources.barrierTexture); 
		}

		//alien rendering  
		for (int i = 0; i < Aliens.size(); i++)
		{
			Aliens[i].Render(resources.alienTexture);
		}






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

void Game::SpawnAliens()
{
	for (int row = 0; row < formationHeight; row++) {
		for (int col = 0; col < formationWidth; col++) {
			Alien newAlien = Alien();
			//TODO: Why is the alien getting instantated with wrongful data? These should be in the constructor.
			newAlien.active = true;
			newAlien.position.x = formationX + 450 + (col * alienSpacing);
			newAlien.position.y = formationY + (row * alienSpacing);
			//TODO: Replace with Emplace back
			Aliens.push_back(newAlien);
			std::cout << "Find Alien -X:" << newAlien.position.x << std::endl;
			std::cout << "Find Alien -Y:" << newAlien.position.y << std::endl;
		}
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

void Player::Initialize() 
{
	// TODO: Get rid of two step init.
	
	float window_width = (float)GetScreenWidth();
	x_pos = window_width / 2;
	std::cout<< "Find Player -X:" << GetScreenWidth() / 2 << "Find Player -Y" << GetScreenHeight() - player_base_height << std::endl;

}

void Player::Update() 
{
	// TODO: Move these to an input processing member maybe.

	//Movement
	direction = 0;
	if (IsKeyDown(KEY_LEFT))
	{
		direction--;
	}
	if (IsKeyDown(KEY_RIGHT))
	{
		direction++;
	}

	x_pos += speed * direction;

	if (x_pos < 0 + radius)
	{
		x_pos = 0 + radius;
	}
	// TODO: Cast things properly.
	else if (x_pos > GetScreenWidth() - radius)
	{
		x_pos = GetScreenWidth() - radius;
	}


	//Determine frame for animation
	timer += GetFrameTime();

	// TODO: Weird check doing a timer comparison twice. 
	if (timer > 0.4 && activeTexture == 2)
	{
		activeTexture = 0;
		timer = 0;
	}
	else if (timer > 0.4)
	{
		activeTexture++;
		timer = 0;
	}
}

void Player::Render(Texture2D texture) 
{
	float window_height = GetScreenHeight(); 

	DrawTextureQuick(texture, { x_pos, window_height - player_base_height }, .3f);
}

void Wall::Render(Texture2D texture)
{
	DrawTextureQuick(texture, position, .3f);
	DrawText(TextFormat("%i", health), position.x-21, position.y+10, 40, RED);
}

void Wall::Update() 
{

	// set walls as inactive when out of health
	if (health < 1)
	{
		active = false;
	}


}

void Alien::Update() 
{
	int window_width = GetScreenWidth(); 

	 //TODO: This can formated in a cleaner way
	if (moveRight)
	{
		position.x += speed; 

		if (position.x >= GetScreenWidth())
		{
			moveRight = false; 
			position.y += 50; 
		}
	}
	else 
	{
		position.x -= speed; 

		if (position.x <= 0)
		{
			moveRight = true; 
			position.y += 50; 
		}
	}
}

void Alien::Render(Texture2D texture) 
{
	DrawTextureQuick(texture, position, .3f);
}
