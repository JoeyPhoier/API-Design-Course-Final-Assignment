#pragma once
#include "raylib.h"
#include <vector>
#include <string_view>
#include "MyTexture2D.h"
#include <format>
#include <stdexcept>
#include <string>
#include <fstream>
#include <sstream>

class AnimationException : public std::runtime_error
{
public:
	static constexpr std::string_view message = "ANIMATION EXCEPTION : {}.";
	explicit AnimationException(std::string_view info) : runtime_error(std::format(message, info)) {}
};

struct AnimationFrame
{
	Rectangle source;
	float frameHoldTime;
};

class MyAnimation2D
{
	friend class MyAnimator2D;
	MyTexture2D spriteSheet;
	std::vector<AnimationFrame> frames;

	[[nodiscard]] const AnimationFrame& GetFrame(size_t index) const
	{
		if (index >= frames.size())
		{
			throw AnimationException("Attempted to access frame out of bounds of the the animation storage.");
		}
		return frames[index];
	}

	[[nodiscard]] inline float GetFrameHoldTime(size_t index) const
	{
		return GetFrame(index).frameHoldTime;
	}

	[[nodiscard]] inline Rectangle GetFrameSource(size_t index) const
	{
		return GetFrame(index).source;
	}

	[[nodiscard]] inline size_t GetNextFrameIndex(size_t index) const noexcept
	{
		return (index + 1) % frames.size();
	}

public:
	explicit MyAnimation2D(std::string_view spriteSheetPath, std::string_view animationAtlasPath) : spriteSheet(spriteSheetPath)
	{
		std::ifstream inFile(animationAtlasPath.data());
		std::string buffer;
		while (std::getline(inFile, buffer) && buffer.size() > 0)
		{
			if (buffer.front() == '#')
			{
				continue;
			}
			std::istringstream stringStream(buffer);
			Rectangle rec;
			float holdTime = 0;
			stringStream >> rec.x >> rec.y >> rec.width >> rec.height >> holdTime;
			frames.emplace_back(rec, holdTime);
		}
	}

	[[nodiscard]] inline const Texture& GetTexture() const noexcept
	{
		return spriteSheet.get();
	}
};

class MyAnimator2D
{
	mutable size_t currentFrameIndex = 0;
	mutable float currentFrameTime = 0;

public:
	void Update() noexcept
	{
		currentFrameTime += GetFrameTime();
	}

	void UpdateFrameData(const MyAnimation2D& animation) const 
	{
		float currentFrameHoldTime = animation.GetFrameHoldTime(currentFrameIndex);
		while (currentFrameTime >= currentFrameHoldTime)
		{
			currentFrameTime -= currentFrameHoldTime;
			currentFrameIndex = animation.GetNextFrameIndex(currentFrameIndex);
			currentFrameHoldTime = animation.GetFrameHoldTime(currentFrameIndex);
		}
	}

	[[nodiscard]] Rectangle GetSourceRectangle(const MyAnimation2D& animation) const 
	{
		return animation.GetFrameSource(currentFrameIndex);
	}
};