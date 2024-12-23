#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "Resources.h"
#include "Projectile.h"
#include "PlayerShip.h"
#include "Barrier.h"
#include "Alien.h"

//TODO: Convert all enums to enum classes.
enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

//TODO: Should be moved to a highscore specific header.
struct PlayerData
{
	std::string name;
	int score;
};

struct Game
{
	// Gamestate
	State gameState = {};

	// Score
	int score;

	// for later, make a file where you can adjust the number of walls (config file) 
	int wallCount = 5;

	//TODO: Should be be a static value part of alien.
	//Aliens shooting
	float shootTimer = 0;

	//Aliens stuff? (idk cause liv wrote this)
	Rectangle rec = { 0, 0 ,0 ,0 }; 

	//TODO: width and height should be in a vector, probably.
	int formationWidth = 8;
	int formationHeight = 5;
	int alienSpacing = 80;
	//TODO: What is this.
	int formationX = 100;
	int formationY = 50;

	//TODO: Is this necessary?
	bool newHighScore = false;
	

	void Start();
	void End();

	void Continue();
	void Launch();

	void Update();
	void Render();

	void SpawnAliens();

	bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineTop, Vector2 lineBottom);

	bool CheckNewHighScore();

	void InsertNewHighScore(std::string name);

	void LoadLeaderboard();
	void SaveLeaderboard();


	//TODO: Textures should go into their own managers
	// Entity Storage and Resources
	Resources resources;

	PlayerShip player;

	std::vector<Projectile> Projectiles;

	std::vector<Barrier> Walls;

	std::vector<Alien> Aliens;

	std::vector<PlayerData> Leaderboard = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };

	//TODO: All of these should probably be stored in their respective classes
	Vector2 playerPos;
	Vector2 alienPos; 
	Vector2 cornerPos;
	Vector2 backgroundPos;
	float offset;

	//TODO: Theres probably a better way to do this
	//TEXTBOX ENTER
	char name[9 + 1] = "\0";      //One extra space required for null terminator char '\0'
	int letterCount = 0;

	Rectangle textBox = { 600, 500, 225, 50 };
	bool mouseOnText = false;

	//TODO: What is this being used for? Doesnt seem necessary.
	int framesCounter = 0;

};