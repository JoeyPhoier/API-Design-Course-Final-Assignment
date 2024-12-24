#pragma once
#include "raylib.h"
#include "raymath.h"
#include "RayUtils.h"

static bool MyCheckCollision_AABBCircle(const Vector2& boxPosition, const Vector2& boxSize, const Vector2& circlePosition, const float& circleRadius) noexcept
{
	const Vector2 topLeftCorner = boxPosition - (boxSize * .5f);
	const Vector2 bottomRightCorner = boxPosition + (boxSize * .5f);
	const Vector2 closestPoint = Vector2Clamp(circlePosition, topLeftCorner, bottomRightCorner);

	const bool IsClosestPointInCircle = circleRadius >= Vector2Distance(circlePosition, closestPoint);
	return IsClosestPointInCircle;
}

static bool MyCheckCollision_AABBCircle(const MyRectangle& box, const Vector2& circlePosition, const float& circleRadius) noexcept
{
	const Vector2 topLeftCorner = box.position - (box.size * .5f);
	const Vector2 bottomRightCorner = box.position + (box.size * .5f);
	const Vector2 closestPoint = Vector2Clamp(circlePosition, topLeftCorner, bottomRightCorner);

	const bool IsClosestPointInCircle = circleRadius >= Vector2Distance(circlePosition, closestPoint);
	return IsClosestPointInCircle;
}
