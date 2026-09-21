#include "Debug.h"

#include "GameManager.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <string>

Debug* Debug::Get()
{
	static Debug mInstance;

	return &mInstance;
}

void Debug::Draw(sf::RenderWindow* pRenderWindow)
{
	for (Line& line : mLines)
	{
		pRenderWindow->draw(&line.start, 2, sf::Lines);
	}

	mLines.clear();

	for (sf::Text& text : mTexts)
	{
		pRenderWindow->draw(text);
	}

	mTexts.clear();

	for (sf::CircleShape& circle : mCircles)
	{
		pRenderWindow->draw(circle);
	}

	mCircles.clear();
}

void Debug::DrawLine(sf::Vector2f p1, sf::Vector2f p2, const sf::Color& color)
{
	Line line;

	line.start = sf::Vertex(p1);
	line.start.color = color;

	line.end = sf::Vertex(p2);
	line.end.color = color;

	Debug::Get()->mLines.push_back(line);
}

void Debug::DrawRectangle(sf::Vector2f position, float width, float height, const sf::Color& color)
{
	sf::Vector2f topLeft = position;
	sf::Vector2f topRight = { position.x + width, position.y };
	sf::Vector2f bottomRight = { position.x + width, position.y + height };
	sf::Vector2f bottomLeft = { position.x, position.y + height };

	DrawLine(topLeft, topRight, color);
	DrawLine(topRight, bottomRight, color);
	DrawLine(bottomRight, bottomLeft, color);
	DrawLine(bottomLeft, topLeft, color);
}

void Debug::DrawCircle(sf::Vector2f center, float radius, const sf::Color& color)
{
	sf::CircleShape circle;

	circle.setRadius(radius);
	circle.setFillColor(color);
	circle.setPosition(center.x - radius, center.y - radius);

	Debug::Get()->mCircles.push_back(circle);
}

void Debug::DrawText(sf::Vector2f position, const std::string& text, const sf::Color& color)
{
	DrawText(position, text, 0.f, 0.f, color);
}

void Debug::DrawText(sf::Vector2f position, const std::string& text, float ratioX, float ratioY, const sf::Color& color)
{
	_ASSERT(ratioX >= 0.f && ratioX <= 1.f);
	_ASSERT(ratioY >= 0.f && ratioY <= 1.f);

	sf::Text sfText;

	sfText.setFont(GameManager::Get()->GetFont());
	sfText.setString(text);
	sfText.setCharacterSize(20);
	sfText.setFillColor(color);
	sfText.setPosition(position.x, position.y);

	const sf::FloatRect& bounds = sfText.getLocalBounds();
	sfText.setOrigin(bounds.width * ratioX, bounds.height * ratioY);

	Debug::Get()->mTexts.push_back(sfText);
}
