#pragma once 
#include <string>
#include <vector>
#include <iostream>
#include "raylib.h"
#include <format>
#include <sstream>
#include <array>
#include "VariableSaveSystem.h"

struct PlayerData;	//Defined below Leaderboad

class Leaderboard final
{
public:
	using ScoreType = unsigned int;

private:
	std::string leaderboardFileName = "Leaderboard.sig";

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

	void RenderTextBox() const;
	void RenderLeaderboardData() const;
public:
	void PrepareLeaderboard(ScoreType score);
	bool ShouldExitLeaderboard() const noexcept
	{
		return canExitLeaderboard;
	}

	void Update(ScoreType score);
	void Render() const;
};

struct PlayerData
{
	std::string name;
	Leaderboard::ScoreType score = 0;

	void Serialize(MyVariableSaver& outFile) const
	{
		outFile.Save(name);
		outFile.Save(score);
	}
#pragma warning(disable : 5817)
	//This function modifies the name and score members, and is therefore not const.
	//Even so, it generates a warning saying we should mark this function as const.
	void Unserialize(MyVariableLoader& inFile)
	{
		inFile.Load(name);
		inFile.Load(score);
	}
#pragma warning(default : 5817)
};