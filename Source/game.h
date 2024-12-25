#pragma once
#include "raylib.h"
#include <vector>
#include <string>
#include "MyTexture2D.h"
#include "Projectile.h"
#include "PlayerShip.h"
#include "Barrier.h"
#include "Alien.h"

//TODO: Convert all enums to enum classes.
enum class State
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
	State gameState = State::STARTSCREEN;

	int score = 0;

	//Aliens stuff? (idk cause liv wrote this)
	Rectangle rec = { 0, 0 ,0 ,0 }; 


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

	bool CheckCollision(Vector2 circlePos, float circleRadius, Vector2 lineTop, Vector2 lineBottom);

	bool CheckNewHighScore();

	void InsertNewHighScore(std::string name);

	void LoadLeaderboard();
	void SaveLeaderboard();

	//Entities
	PlayerShip player;
	AlienArmy alienArmy;
	std::vector<Projectile> Projectiles;
	std::vector<Barrier> Barriers;
	int wallCount = 5;

	//Textures
	std::vector<MyTexture2D> playerTextures;
	MyTexture2D AlienTexture = MyTexture2D("./Assets/Alien.png");
	MyTexture2D ProjectileTexture = MyTexture2D("./Assets/Laser.png");
	MyTexture2D BarrierTexture = MyTexture2D("./Assets/Barrier.png");
	MyTexture2D backgroundTexture = MyTexture2D("./Assets/Space Background.png");

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