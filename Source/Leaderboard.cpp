#include "Leaderboard.h"
#include <format>
#include <algorithm>
#include <iostream>
#include <fstream>

//This is required due to a raylib bug in the CheckCollisionPointRec
bool FixedCheckCollisionPointRec(const Vector2& point, Rectangle rectangle) noexcept
{
	rectangle.y -= rectangle.height * .5f;
	return CheckCollisionPointRec(point, rectangle);
}

void Leaderboard::LoadLeaderboard()
{
	//TODO: Maybe Implement this?

	// CLEAR LEADERBOARD

	// OPEN FILE

	// READ DATA

	// WRITE DATA ONTO LEADERBOARD

	//CLOSE FILE
}

void Leaderboard::SaveLeaderboard()
{
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

void Leaderboard::PrepareLeaderboard(int score) noexcept
{
	canExitLeaderboard = false;
	if (CanScoreGoOnLeaderboard(score))
	{
		isInInputNameScreen = true;
	}
	else
	{
		isInInputNameScreen = false;
	}
}

void Leaderboard::Update(int score) noexcept
{
	if (!isInInputNameScreen)
	{
		if (IsKeyPressed(KEY_ENTER))
		{
			canExitLeaderboard = true;
		}
		return;
	}

	UpdateNameTextBox(score);
}

//TODO: Shorten to simplify
void Leaderboard::UpdateNameTextBox(int score) noexcept
{
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
		InsertNewHighScore({ playerName, score});
		isInInputNameScreen = false;
	}
}

void Leaderboard::RenderTextBox() const noexcept
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

	DrawText((playerName.size() > 0) ? "PRESS ENTER TO CONTINUE" : "PLEASE INSERT A NAME",
			 600, 800, 40, YELLOW);
	DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);
	DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);
}

void Leaderboard::RenderLeaderboardData() const noexcept
{
	int yOffset = 0;
	for (const auto& data : dataTable)
	{
		DrawText(data.name.c_str(), 50, 140 + yOffset, 40, YELLOW);
		DrawText(std::format("{}", data.score).c_str(), 350, 140 + yOffset, 40, YELLOW);
		yOffset += 40;
	}

	DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);
	DrawText("LEADERBOARD", 50, 100, 40, YELLOW);
}

void Leaderboard::InsertNewHighScore(PlayerData data) noexcept
{
	dataTable.emplace_back(data);
	std::ranges::sort(dataTable, [&](const PlayerData& a, const PlayerData& b)
					  {
						  return a.score > b.score;
					  });
	if (dataTable.size() > 5)
	{
		dataTable.pop_back();
	}
	SaveLeaderboard();
	//TODO Move to game
	//gameState = State::LEADERBOARD;
}

void Leaderboard::Render() noexcept
{
	if (isInInputNameScreen)
	{
		RenderTextBox();
	}
	else
	{
		RenderLeaderboardData();
	}
}
