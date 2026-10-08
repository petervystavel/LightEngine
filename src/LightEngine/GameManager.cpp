#include "GameManager.h"

#include "Entity.h"
#include "Debug.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include <iostream>

GameManager::GameManager()
{
	mpWindow = nullptr;
	mDeltaTime = 0.0f;
	mFixedDeltaTime = DEFAULT_FIXED_DT;
	mpScene = nullptr;
	mWindowWidth = -1;
	mWindowHeight = -1;
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpWindow;
	delete mpScene;

	for (Entity* entity : mEntities)
	{
		delete entity;
	}
}

void GameManager::CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit, sf::Color clearColor)
{
	_ASSERT(mpWindow == nullptr);

	mpWindow = new sf::RenderWindow(sf::VideoMode(width, height), title);
	mpWindow->setFramerateLimit(fpsLimit);

	mWindowWidth = width;
	mWindowHeight = height;

	mClearColor = clearColor;
}

void GameManager::Run()
{
	if (mpWindow == nullptr)
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateWindow(1280, 720, "Default window");
	}

	//#TODO : Load somewhere else
	bool fontLoaded = mFont.loadFromFile("../../../res/Hack-Regular.ttf");
	_ASSERT(fontLoaded);

	_ASSERT(mpScene != nullptr);

	sf::Clock clock;
	while (mpWindow->isOpen())
	{
		SetDeltaTime(clock.restart().asSeconds());

		HandleInput();

		Update();

		Draw();
	}
}

void GameManager::HandleInput()
{
	sf::Event event;
	while (mpWindow->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			mpWindow->close();
		}

		mpScene->OnEvent(event);
	}
}

void GameManager::Update()
{
	mpScene->OnUpdate();

	//Update
	for (auto it = mEntities.begin(); it != mEntities.end(); )
	{
		Entity* entity = *it;

		entity->Update();

		if (entity->ToDestroy() == false)
		{
			++it;
			continue;
		}

		mEntitiesToDestroy.push_back(entity);
		it = mEntities.erase(it);
	}

	//Fixed Update
	mAccumulatedDeltaTime += mDeltaTime;
	while (mAccumulatedDeltaTime >= mFixedDeltaTime)
	{
#if CCD
		FixedUpdateCC();
#else
		FixedUpdateDC();
#endif
		mAccumulatedDeltaTime -= mFixedDeltaTime;
	}

	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it)
	{
		delete* it;
	}

	mEntitiesToDestroy.clear();

	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();
}

void GameManager::FixedUpdateDC()
{
	//Fixed Update
	for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		Entity* entity = *it;
		entity->PhysicUpdate();
		entity->PhysicMove(mFixedDeltaTime);
	}

	//Collision
	for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
	{
		Entity* e1 = *it1;

		auto it2 = it1;
		++it2;
		for (; it2 != mEntities.end(); ++it2)
		{
			Entity* e2 = *it2;

			if (e1->IsColliding(e2))
			{
				e1->CollisionReaction(e2);

				e1->OnCollision(e2);
				e2->OnCollision(e1);
			}
		}

		e1->TryBounceOnEdges();
	}

	for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		Entity* entity = *it;
		entity->mVelocity = entity->mNewVelocity;
	}
}

void GameManager::FixedUpdateCC()
{
	float dt = mFixedDeltaTime;
	float toiMin = mFixedDeltaTime;

	for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
	{
		Entity* entity = *it;
		entity->PhysicUpdate();
	}

	while (true)
	{
		Entity* e1Collision = nullptr;
		Entity* e2Collision = nullptr;
		Edge* edge = nullptr;

		for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
		{
			Entity* e1 = *it1;

			sf::Vector2f pos = e1->GetPosition();

			auto it2 = it1;
			++it2;
			for (; it2 != mEntities.end(); ++it2)
			{
				Entity* e2 = *it2;

				sf::Vector2f relativeTrans = (e1->mVelocity - e2->mVelocity) * dt;
				if (Utils::IsZero(relativeTrans))
					continue;

				IntersectionInfo info = e1->CircleCast(e2, relativeTrans);
				if (info.hit == false)
					continue;

				sf::Vector2f transToImpact = info.point - pos;
				float ratio = Utils::GetRatio(transToImpact, relativeTrans);

				float toi = dt * ratio;
				if (toi <= toiMin) 
				{
					toiMin = toi;
					e1Collision = e1;
					e2Collision = e2;
					edge = nullptr;
				}
			}

			if (Utils::IsZero(e1->mVelocity))
				continue;

			sf::Vector2f trans = e1->mVelocity * dt;
			for (int i = 0; i < mEdges.size(); ++i) 
			{
				IntersectionInfo info = e1->EdgeCast(mEdges[i], trans);
				if (info.hit == false)
					continue;

				sf::Vector2f transToImpact = info.point - pos;
				float ratio = Utils::GetRatio(transToImpact, trans);
				if (ratio <= 0)
					continue;

				float toi = dt * ratio;
				if (toi <= toiMin)
				{
					toiMin = toi;
					e1Collision = e1;
					edge = &mEdges[i];
				}
			}
		}

		for (auto it = mEntities.begin(); it != mEntities.end(); ++it)
		{
			Entity* entity = *it;
			entity->PhysicMove(toiMin);
		}

		if (e1Collision != nullptr)
		{
			if (edge != nullptr) 
			{
				e1Collision->Bounce(-edge->normal, 1.f);
				e1Collision->mVelocity = e1Collision->mNewVelocity;
			}
			else 
			{
				e1Collision->CollisionReaction(e2Collision);
				e1Collision->mVelocity = e1Collision->mNewVelocity;
				e2Collision->mVelocity = e2Collision->mNewVelocity;
			}
		}
		
		dt -= toiMin;
		if (dt <= 0)
			break;

		toiMin = dt;
	}
}

void GameManager::Draw()
{
	mpWindow->clear(mClearColor);

	for (Entity* entity : mEntities)
	{
		mpWindow->draw(*entity->GetShape());
	}

	for (int i = 0; i < mEdges.size(); ++i) 
	{
		sf::Vector2f p1 = mEdges[i].s.p1;
		sf::Vector2f p2 = mEdges[i].s.p2;

		Debug::DrawLine(p1, p2, sf::Color::White);
	}
	
	Debug::Get()->Draw(mpWindow);

	mpWindow->display();
}
