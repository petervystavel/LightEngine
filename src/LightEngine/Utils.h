#pragma once

#include <SFML/System/Vector2.hpp>

struct RaycastInfo 
{
	bool hit;
	sf::Vector2f point;
};

struct Circle 
{
	sf::Vector2f center;
	float radius;
};

struct Segment 
{
	sf::Vector2f p1;
	sf::Vector2f p2;
};

namespace Utils
{
	bool Normalize(sf::Vector2f& vector);
	float Dot(sf::Vector2f v1, sf::Vector2f v2);
	float GetDistance(sf::Vector2f p1, sf::Vector2f p2);
	float GetDistance(sf::Vector2f translation);
	sf::Vector2f GetTranslation(sf::Vector2f p1, sf::Vector2f p2);
	float GetAngleDegree(sf::Vector2f v1, sf::Vector2f v2);
	RaycastInfo Raycast(Segment segment, Circle circle);
}