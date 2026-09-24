#include "Scene.h"

#include "GameManager.h"

#include <SFML/Graphics/RenderWindow.hpp>

int Scene::GetWindowWidth() const
{
	return mpGameManager->mWindowWidth;
}

int Scene::GetWindowHeight() const
{
	return mpGameManager->mWindowHeight;
}

float Scene::GetDeltaTime() const
{
	return mpGameManager->mDeltaTime;
}

sf::Vector2i Scene::GetMousePosition() const
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*mpGameManager->mpWindow);

	int x = mousePos.x;
	int y = mousePos.y;

	return mousePos;
}