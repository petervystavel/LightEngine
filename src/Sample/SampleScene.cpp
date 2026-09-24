#include "SampleScene.h"

#include "Debug.h"
#include "Utils.h"

#include <SFML/System/Vector2.hpp>

void SampleScene::OnInitialize()
{
	//pEntity1 = CreateEntity<Entity>(100, sf::Color::Red);
	//pEntity1->SetPosition({ 100.f, 100.f });

	pEntity2 = CreateEntity<Entity>(50, sf::Color::Green);
	pEntity2->SetPosition({ 500.f, 500.f });

	pEntitySelected = nullptr;
}

void SampleScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	sf::Vector2i mousePos = GetMousePosition();

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		if (pEntitySelected != nullptr ) 
		{
			pEntitySelected->AddImpulse(mVelocity);
			pEntitySelected = nullptr;
		}
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		TrySetSelectedEntity(pEntity1, mousePos.x, mousePos.y);
		TrySetSelectedEntity(pEntity2, mousePos.x, mousePos.y);
	}
}

void SampleScene::TrySetSelectedEntity(Entity* pEntity, int x, int y)
{
	if (pEntity == nullptr)
		return;

	if (pEntity->IsInside(x, y) == false)
		return;

	pEntitySelected = pEntity;
}

void SampleScene::TryDrawLine()
{
	if (pEntitySelected == nullptr)
		return;

	sf::Vector2f mousePos = (sf::Vector2f)GetMousePosition();
	sf::Vector2f center = pEntitySelected->GetPosition(0.5f, 0.5f);

	sf::Vector2f translation = Utils::GetTranslation(center, mousePos);

	float distance = Utils::GetDistance(translation);
	float radius = pEntitySelected->GetRadius();

	distance -= radius;
	if (distance <= 0)
		return;

	if (distance > mLineLengthMax)
		distance = mLineLengthMax;

	Utils::Normalize(translation);

	sf::Vector2f p1 = center + translation * radius;
	sf::Vector2f p2 = p1 + translation * distance;

	Debug::DrawLine(p1, p2, sf::Color::Green);

	float ratio = distance / mLineLengthMax;
	float strength = ratio * mStrengthMax;

	mVelocity = translation * strength;
}

void SampleScene::OnUpdate()
{
	if (pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position, 10, sf::Color::Blue);

		TryDrawLine();
	}
}