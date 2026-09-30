#include "SampleScene.h"

#include "Debug.h"
#include "Utils.h"

#include <SFML/System/Vector2.hpp>

void SampleScene::OnInitialize()
{
	pEntitySelected = nullptr;
}

void SampleScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	sf::Vector2f mousePos = GetMousePosition();

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		Entity* pEntity = CreateEntity<Entity>(50, sf::Color::Green);
		pEntity->SetPosition({ mousePos.x, mousePos.y });
		m_pEntities.push_back(pEntity);
	}

	if (event.mouseButton.button == sf::Mouse::Button::Left)
	{
		if (pEntitySelected == nullptr)
		{
			for (int i = 0; i < m_pEntities.size(); ++i)
			{
				TrySetSelectedEntity(m_pEntities[i], mousePos.x, mousePos.y);
			}
		}
		else 
		{
			pEntitySelected->AddImpulse(mVelocity);
			pEntitySelected = nullptr;
		}
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