#pragma once
#include "raylib.h"
#include <vector>
#include "Resources.h"
#include <string>

//TODO: Convert all enums to enum classes.
enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

//TODO: Consider if using entity enums is the right way to god
enum struct EntityType
{
	PLAYER,
	ENEMY,
	PLAYER_PROJECTILE,
	ENEMY_PROJECTILE
};

//TODO: Should be moved to a highscore specific header.
struct PlayerData
{
	std::string name;
	int score;
};

//TODO: Should be moved to a Player specific header.
struct Player
{
public:

	float x_pos = 0;
	float speed = 7;
	float player_base_height = 70.0f;  
	float radius = 50;
	int lives = 3;
	int direction = 0;
	int activeTexture = 0;
	float timer = 0;

	EntityType type = EntityType::PLAYER;

	void Initialize();
	void Render(Texture2D texture);
	void Update();
	
};


struct Projectile
{
public: 
	// INITIALIZE PROJECTILE WHILE DEFINING IF ITS PLAYER OR ENEMY 
	Vector2 position = {0,0};
	int speed = 15; 
	bool active = true;  
	//TODO: Should not be null
	EntityType type = {};

	//TODO: This is probably uncessary. Just get starting pos, and add velocity upwards
	// Consider making projectiles AABBs, instead of lines
	// LINE WILL UPDATE WITH POSITION FOR CALCULATIONS
	Vector2 lineStart = { 0, 0 };
	Vector2 lineEnd = { 0, 0 };

	void Update();

	void Render(Texture2D texture);
};

//TODO: Change name. Should be barrier or something.
struct Wall
{
public: 
	//TODO: Does not need both a position and a rectangle. 
	Vector2 position;
	Rectangle rec; 
	//TODO: Do all entities need an active flag? Consider getting rid of them.
	bool active;
	//TODO: Do all walls need a color member? If so, should probably be static.
	Color color;
	//TODO: I would like to see the max health be a static and constexpr value.
	int health = 50;
	int radius = 60;


	void Render(Texture2D texture); 
	void Update(); 
};

struct Alien
{
public:
	
	//TODO: Is this necessary? If all textures are rendered with base color, get rid of this.
	Color color = WHITE;
	Vector2 position = {0, 0};
	//TODO: Why do they need a position and an x and y?
	int x = 0;
	int y = 0; 
	//TODO: Should be constexpr and static
	float radius = 30;
	bool active = true;  
	bool moveRight = true; 
	
	EntityType type = EntityType::ENEMY; 

	int speed = 2; 
		 
	void Update(); 
	void Render(Texture2D texture); 
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

	Player player;

	std::vector<Projectile> Projectiles;

	std::vector<Wall> Walls;

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