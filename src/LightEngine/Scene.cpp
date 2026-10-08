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

sf::Vector2f Scene::GetMousePosition() const
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(*mpGameManager->mpWindow);

	return {(float)mousePos.x, (float)mousePos.y};
}

void Scene::AddEdge(sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f normal)
{
	Edge edge = { {p1, p2}, normal };
	mpGameManager->mEdges.push_back(edge);
}