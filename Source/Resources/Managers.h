#pragma once
#include "raylib.h"

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