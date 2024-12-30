#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include "raylib.h"
#include <format>
#include <sstream>
#include <array>


struct PlayerData
{
	std::string name;
	unsigned int score;

	PlayerData() noexcept = default;
	PlayerData(std::string_view namei, int scorei) noexcept : name(namei), score(scorei) {};
};

class Leaderboard
{
	std::vector<PlayerData> dataTable;
	
	std::string playerName = "";
	Rectangle textboxRectangle = { 600, 500, 225, 50 };
	static constexpr int maxCharactersOnName = 3;
	bool textBoxSelected = false;
	float textBoxRenderTimer = 0;

	bool isInInputNameScreen = true;
	bool canExitLeaderboard = false;

	void UpdateTextBoxSelection() noexcept;
	void TextboxWritingInput() noexcept;
	void UpdateNameTextBox(int score) noexcept;

	void InsertNewHighScore(const PlayerData& data) noexcept;

	void LoadLeaderboard();
	void SaveLeaderboard();

	void RenderTextBox() const noexcept;
	void RenderLeaderboardData() const noexcept;
public:
	void PrepareLeaderboard(int score);
	bool ShouldExitLeaderboard() const noexcept
	{
		return canExitLeaderboard;
	}

	void Update(int score) noexcept;
	void Render() const noexcept;
};