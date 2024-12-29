#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include "raylib.h"
#include <format>
#include <sstream>

struct PlayerData
{
	std::string name;
	int score;

	PlayerData(std::string_view namei, int scorei) : name(namei), score(scorei) {};
	explicit PlayerData(std::string_view textEntry)
	{
		std::stringstream ss(textEntry.data());
		std::getline(ss, name,':');
		std::string scoreString; 
		std::getline(ss, scoreString);
		score = std::stoi(scoreString);
	}

	friend std::string& operator<<(std::string& output, const PlayerData& data)
	{
		return output.append(std::format("{}: {}\n", data.name, data.score));
	};
};

class Leaderboard
{
	std::vector<PlayerData> dataTable;

	std::string playerName = "";
	Rectangle textboxRectangle = { 600, 500, 225, 50 };
	static constexpr int maxCharactersOnName = 8;
	bool textBoxSelected = false;
	float textBoxRenderTimer = 0;

	bool isInInputNameScreen = true;
	bool canExitLeaderboard = false;

	void UpdateTextBoxSelection() noexcept;
	void TextboxWritingInput() noexcept;
	void UpdateNameTextBox(int score) noexcept;

	void InsertNewHighScore(PlayerData data) noexcept;

	void LoadLeaderboard();
	void SaveLeaderboard();

	void RenderTextBox() const noexcept;
	void RenderLeaderboardData() const noexcept;
public:
	void PrepareLeaderboard(int score) noexcept;
	bool ShouldExitLeaderboard() const noexcept
	{
		return canExitLeaderboard;
	}

	void Update(int score) noexcept;
	void Render() const noexcept;
};