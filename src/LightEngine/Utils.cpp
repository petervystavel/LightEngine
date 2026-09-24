#include "Utils.h"

#include <cmath>

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

	float Dot(sf::Vector2f v1, sf::Vector2f v2)
	{
		return v1.x * v2.x + v1.y + v2.y;
	}

	float GetDistance(sf::Vector2f p1, sf::Vector2f p2)
	{
		sf::Vector2f translation = GetTranslation(p1, p2);

		return GetDistance(translation);
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
}