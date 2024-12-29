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
	dataTable.clear();

	std::ifstream input("Leaderboard.txt");
	if (input.fail())
	{
		return;
	}
	std::vector<std::string> entries;
	std::string line;
	while (!input.eof())
	{
		std::getline(input, line);
		if (!line.empty())
		{
			entries.push_back(line);
		}
	}

	for (const auto& entry : entries)
	{
		dataTable.emplace_back(entry);
	}
}

void Leaderboard::SaveLeaderboard()
{
	std::string leaderboardData;
	auto saveTableEntry = [&](const PlayerData& data)
		{
			leaderboardData << data;
		};
	std::ranges::for_each(dataTable, saveTableEntry);
	SaveFileText("Leaderboard.txt", leaderboardData.data());
}

void Leaderboard::PrepareLeaderboard(int score) noexcept
{
	LoadLeaderboard();
	const bool isThereSpaceInLeaderboard = dataTable.size() < 5;
	const bool isScoreGreaterThanLowestEntry = !dataTable.empty() && score > dataTable.back().score;

	isInInputNameScreen = isThereSpaceInLeaderboard || isScoreGreaterThanLowestEntry;
}

void Leaderboard::Update(int score) noexcept
{
	if (isInInputNameScreen)
	{
		UpdateNameTextBox(score);
	}
	else if (IsKeyPressed(KEY_ENTER))
	{
		canExitLeaderboard = true;
	}
}

void Leaderboard::UpdateTextBoxSelection() noexcept
{
	if (!IsMouseButtonPressed(0))
	{
		return;
	}

	textBoxSelected = FixedCheckCollisionPointRec(GetMousePosition(), textboxRectangle);
	SetMouseCursor(textBoxSelected ? MOUSE_CURSOR_IBEAM : MOUSE_CURSOR_DEFAULT);
}

void Leaderboard::TextboxWritingInput() noexcept
{
	int key = GetCharPressed();
	auto ShouldPerformKeyCheck = [&]()
		{
			const bool WasAKeyPressed = key > 0;
			const bool IsThereSpace = playerName.size() < maxCharactersOnName;
			return WasAKeyPressed && IsThereSpace;
		};

	while (ShouldPerformKeyCheck())
	{
		if (const bool IsKeyAValidCharacter = (key >= 32) && (key <= 125))
		{
			playerName += static_cast<char>(key);
		}
		key = GetCharPressed();  // Check next character in the queue
	}
}

void Leaderboard::UpdateNameTextBox(int score) noexcept
{
	UpdateTextBoxSelection();
	if (!textBoxSelected)
	{
		return;
	}

	textBoxRenderTimer += GetFrameTime();
	if (textBoxRenderTimer > 2)
	{
		textBoxRenderTimer = 0;
	}

	TextboxWritingInput();

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
	DrawRectangleRec(textboxRectangle, LIGHTGRAY);
	DrawRectangleLinesEx(textboxRectangle, textBoxThickness, textBoxSelected ? RED : DARKGRAY);

	const auto textX = static_cast<int>(textboxRectangle.x);
	const auto textY = static_cast<int>(textboxRectangle.y);
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
}

void Leaderboard::Render() const noexcept
{
	if (isInInputNameScreen)
	{
		RenderTextBox();
		return;
	}
	else
	{
		RenderLeaderboardData();
	}
}
