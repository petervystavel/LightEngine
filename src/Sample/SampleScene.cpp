#include "SampleScene.h"

#include "Debug.h"
#include "Utils.h"

#include <SFML/System/Vector2.hpp>

void SampleScene::OnInitialize()
{
	pEntitySelected = nullptr;

	float top = 1;
	float left = 1;
	float bottom = GetWindowHeight() - 1;
	float right = GetWindowWidth() - 1;

	AddEdge({ left, top }, { right, top }, { 0, 1 });
	AddEdge({ right, top }, { right, bottom }, { -1, 0 });
	AddEdge({ right, bottom }, { left, bottom }, { 0, -1 });
	AddEdge({ left, bottom }, { left, top }, { 1, 0 });
}

void SampleScene::OnEvent(const sf::Event& event)
{
	if (event.type != sf::Event::EventType::MouseButtonPressed)
		return;

	sf::Vector2f mousePos = GetMousePosition();

	if (event.mouseButton.button == sf::Mouse::Button::Right)
	{
		sf::Vector2f circlePos = mousePos;
		if (mCtrl)
			circlePos = mCtrlPosition;

		sf::Color color = sf::Color::Green;
		float radius = 10;
		float mass = 1;
		if (mAlt)
		{
			color = sf::Color::Red;
			radius = 100;
			mass = 1000;
		}

		Entity* pEntity = CreateEntity<Entity>(radius, color);
		pEntity->SetPosition({ circlePos.x, circlePos.y });
		pEntity->SetMass(mass);
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

	//if (distance > mLineLengthMax)
	//	distance = mLineLengthMax;

	Utils::Normalize(translation);

	sf::Vector2f p1 = center + translation * radius;
	sf::Vector2f p2 = p1 + translation * distance;

	Debug::DrawLine(p1, p2, sf::Color::Green);

	float ratio = distance / mLineLengthMax;
	float strength = ratio * mStrengthMax;

	mVelocity = translation * strength;
}


#include <iostream>

void SampleScene::OnUpdate()
{
	if (pEntitySelected != nullptr)
	{
		sf::Vector2f position = pEntitySelected->GetPosition();
		Debug::DrawCircle(position, 10, sf::Color::Blue);

		TryDrawLine();

		sf::Vector2f mousePos = GetMousePosition();
		sf::Vector2f center = pEntitySelected->GetPosition();

		sf::Vector2f translation = mousePos - center;
		
		//for (int i = 0; i < m_pEntities.size(); ++i)
		//{
		//	if (m_pEntities[i] == pEntitySelected)
		//		continue;

		//	sf::Vector2f translation = mousePos - center;
		//	RaycastInfo info = pEntitySelected->Circlecast(m_pEntities[i], translation);

		//	if (info.hit == false)
		//		continue;

		//	Debug::DrawCircle(info.point, pEntitySelected->GetRadius(), sf::Color::Blue);
		//}

		for (int i = 0; i < GameManager::Get()->mEdges.size(); ++i) 
		{
			IntersectionInfo info = pEntitySelected->EdgeCast(GameManager::Get()->mEdges[i], translation);
			if (info.hit == false)
				continue;

			Debug::DrawCircle(info.point, pEntitySelected->GetRadius(), sf::Color::Blue);
		}
	}

	mCtrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl);
	mAlt = sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt);

	if (mCtrl)
	{
		sf::Vector2f mousePos = GetMousePosition();
		float x = (int)(mousePos.x / 100) * 100 + 50;
		float y = (int)(mousePos.y / 100) * 100 + 50;

		mCtrlPosition = { x, y };

		Debug::DrawCircle({ x, y }, 10, sf::Color::Blue);
	}
}