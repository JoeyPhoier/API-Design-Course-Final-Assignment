#pragma once
#include "raylib.h"
#include "MyTexture2D.h"

//Textures
struct TextureManager
{
	MyTexture2D playerTexture = MyTexture2D("./Assets/PlayerShip.png");
	MyTexture2D alienTexture = MyTexture2D("./Assets/Alien.png");
	MyTexture2D projectileTexture = MyTexture2D("./Assets/Laser.png");
	MyTexture2D barrierTexture = MyTexture2D("./Assets/Barrier.png");
	MyTexture2D backgroundTexture = MyTexture2D("./Assets/Space Background.png");
};

//Window
class WindowManager
{
public:
	WindowManager() = delete;
	WindowManager(Vector2 windowDimensions, std::string_view appName) noexcept
	{
		InitWindow(static_cast<int>(windowDimensions.x), static_cast<int>(windowDimensions.y), appName.data());
	}
	~WindowManager() noexcept
	{
		CloseWindow();
	};
	WindowManager(const WindowManager& other) = delete;
	WindowManager operator=(const WindowManager& other) = delete;
	WindowManager(WindowManager&& other) = delete;
	WindowManager operator=(WindowManager&& other) = delete;
};

//Audio
class AudioManager
{
public:
	AudioManager() noexcept
	{
		InitAudioDevice();
	};
	~AudioManager() noexcept
	{
		CloseAudioDevice();
	};
	AudioManager(const AudioManager& other) = delete;
	AudioManager operator=(const AudioManager& other) = delete;
	AudioManager(AudioManager&& other) = delete;
	AudioManager operator=(AudioManager&& other) = delete;
};