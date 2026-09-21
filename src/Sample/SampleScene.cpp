#include "SampleScene.h"

#include "Debug.h"

#include <SFML/System/Vector2.hpp>

void SampleScene::OnInitialize()
{
	pEntity1 = CreateEntity<Entity>(100, sf::Color::Red);
	pEntity1->SetPosition({ 100.f, 100.f });
	pEntity1->SetRigidBody(true);

	pEntity2 = CreateEntity<Entity>(50, sf::Color::Green);
	pEntity2->SetPosition({ 500.f, 500.f });
	pEntity2->SetRigidBody(true);

	pEntitySelected = nullptr;
}

void SampleScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	sf::Vector2f mousePos = { (float) event.mouseButton.x, (float)event.mouseButton.y };

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		TrySetSelectedEntity(pEntity1, mousePos.x, mousePos.y);
		TrySetSelectedEntity(pEntity2, mousePos.x, mousePos.y);
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (pEntitySelected != nullptr) 
		{
			pEntitySelected->GoTo(mousePos, 100.f);
		}
	}
}

void SampleScene::TrySetSelectedEntity(Entity* pEntity, int x, int y)
{
	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

void SampleScene::OnUpdate()
{
	if(pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position, 10, sf::Color::Blue);
	}
}