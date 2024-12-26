#pragma once 
#include <string>
#include <vector>
#include "raylib.h"

struct PlayerData
{
	std::string name;
	int score;
};

class Leaderboard
{
	std::vector<PlayerData> dataTable = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };

	std::string playerName = "";
	Rectangle textBox = { 600, 500, 225, 50 };
	static constexpr int maxCharactersOnName = 8;
	bool textBoxSelected = false;
	float textBoxRenderTimer = 0;

	bool isInInputNameScreen = true;
	bool canExitLeaderboard = false;

	void UpdateNameTextBox(int score) noexcept;

	void InsertNewHighScore(PlayerData data) noexcept;

	void LoadLeaderboard();
	void SaveLeaderboard();

	void RenderTextBox() const noexcept;
	void RenderLeaderboardData() const noexcept;

	bool CanScoreGoOnLeaderboard(int score) const noexcept
	{
		return (dataTable.size() < 5 ||
				score > dataTable.back().score);
	}
public:
	void PrepareLeaderboard(int score) noexcept;
	bool CanExitLeaderboard() const noexcept
	{
		return canExitLeaderboard;
	}

	void Update(int score) noexcept;
	void Render() noexcept;
};