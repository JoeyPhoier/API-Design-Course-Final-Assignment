#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include "raylib.h"
#include <format>
#include <sstream>
#include <array>

struct PlayerData;	//Defined below Leaderboad

class Leaderboard final
{
public:
	using ScoreType = unsigned int;

private:
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
	void UpdateNameTextBox(ScoreType score);

	void InsertNewHighScore(const PlayerData& data);

	void LoadLeaderboard();
	void SaveLeaderboard() const;

	void RenderTextBox() const noexcept;
	void RenderLeaderboardData() const noexcept;
public:
	void PrepareLeaderboard(ScoreType score);
	bool ShouldExitLeaderboard() const noexcept
	{
		return canExitLeaderboard;
	}

	void Update(ScoreType score);
	void Render() const noexcept;
};

struct PlayerData
{
	std::string name;
	Leaderboard::ScoreType score = 0;
};