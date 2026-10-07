#include "Utils.h"

#include "Debug.h"

#include <cmath>

#include <iostream>

namespace Utils
{
	bool Normalize(sf::Vector2f& vector)
	{
		float magnitude = std::sqrt(vector.x * vector.x + vector.y * vector.y);

		if (magnitude != 0)
		{
			vector.x /= magnitude;
			vector.y /= magnitude;

			return true;
		}

		return false;
	}

	sf::Vector2f GetNormalized(sf::Vector2f vector)
	{
		float magnitude = GetDistance(vector);

		vector.x /= magnitude;
		vector.y /= magnitude;

		return vector;
	}

	float Dot(sf::Vector2f v1, sf::Vector2f v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	float GetDistance(sf::Vector2f p1, sf::Vector2f p2)
	{
		sf::Vector2f translation = GetTranslation(p1, p2);

		return GetDistance(translation);
	}

	float GetSqrDistance(sf::Vector2f p1, sf::Vector2f p2)
	{
		sf::Vector2f translation = GetTranslation(p1, p2);

		return translation.x * translation.x + translation.y * translation.y;
	}

	float GetDistance(sf::Vector2f translation)
	{
		return std::sqrt(translation.x * translation.x + translation.y * translation.y);
	}

	sf::Vector2f GetTranslation(sf::Vector2f p1, sf::Vector2f p2)
	{
		return { p2.x - p1.x, p2.y - p1.y };
	}

	float GetAngleDegree(sf::Vector2f v1, sf::Vector2f v2)
	{
		float dot = v1.x * v2.x + v1.y * v2.y;
		float det = v1.x * v2.y - v1.y * v2.x;

		return std::atan2(det, dot) * 180 / 3.14159265;
	}

	RaycastInfo Raycast(Segment segment, Circle circle)
	{
		sf::Vector2f v1 = segment.p2 - segment.p1;
		sf::Vector2f v2 = circle.center - segment.p1;

		float ratio = Dot(v2, v1) / Dot(v1, v1);

		if (ratio < 0)
			return { false, {0.f,0.f} };

		sf::Vector2f p = segment.p1 + (ratio * v1);

		sf::Vector2f max = p;
		if (ratio > 1.f)
			max = segment.p2;

		float distance = GetDistance(circle.center, max);
		if (distance > circle.radius)
			return { false, {0.f,0.f} };

		float sqrDist = GetSqrDistance(circle.center, p);
		float sqrRadius = circle.radius * circle.radius;
		float distToIntersection = std::sqrt(sqrRadius - sqrDist);

		sf::Vector2f dir = -GetNormalized(v1);
		sf::Vector2f intersection = p + dir * distToIntersection;

		return { true, intersection };
	}
}