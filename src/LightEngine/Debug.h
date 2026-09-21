#pragma once

#include <vector>
#include <string>

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

class GameManager;

namespace sf
{
	class Color;
	class RenderWindow;
}

struct Line 
{
	sf::Vertex start;
	sf::Vertex end;
};

class Debug
{
	std::vector<Line> mLines;
	std::vector<sf::Text> mTexts;
	std::vector<sf::CircleShape> mCircles;

	void Draw(sf::RenderWindow* pRenderWindow);

public:
	static Debug* Get();

	static void DrawLine(sf::Vector2f p1, sf::Vector2f p2, const sf::Color& color);
	static void DrawRectangle(sf::Vector2f position, float width, float height, const sf::Color& color);
	static void DrawCircle(sf::Vector2f center, float radius, const sf::Color& color);
	static void DrawText(sf::Vector2f position, const std::string& text, const sf::Color& color);
	static void DrawText(sf::Vector2f position, const std::string& text, float ratioX, float ratioY, const sf::Color& color);

	friend GameManager;
};

