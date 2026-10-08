#pragma once

#include <SFML/System/Vector2.hpp>

struct IntersectionInfo 
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

struct Edge 
{
	Segment s;
	sf::Vector2f normal;
};

namespace Utils
{
	bool IsZero(const sf::Vector2f& vector);
	bool Normalize(sf::Vector2f& vector);
	float Dot(sf::Vector2f v1, sf::Vector2f v2);
	float GetDistance(sf::Vector2f p1, sf::Vector2f p2);
	float GetDistance(sf::Vector2f translation);
	sf::Vector2f GetTranslation(sf::Vector2f p1, sf::Vector2f p2);
	float GetAngleDegree(sf::Vector2f v1, sf::Vector2f v2);
	IntersectionInfo RayCast(Segment segment, Circle circle);
	float Cross(sf::Vector2f a, sf::Vector2f b);
	IntersectionInfo Intersect(Segment s1, Segment s2);
	float GetRatio(sf::Vector2f v1, sf::Vector2f v2);
}