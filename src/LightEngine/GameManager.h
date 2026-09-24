#pragma once

#include <list>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Text.hpp>

class Entity;
class Scene;
class Debug;

#define DEFAULT_FIXED_DT 1.f / 60.f

namespace sf 
{
	class RenderWindow;
	class Event;
}

class GameManager
{
	std::list<Entity*> mEntities;
	std::list<Entity*> mEntitiesToDestroy;
	std::list<Entity*> mEntitiesToAdd;

	sf::RenderWindow* mpWindow;
	sf::Font mFont;

	Scene* mpScene;

	float mDeltaTime;
	float mFixedDeltaTime;
	float mAccumulatedDeltaTime;

	int mWindowWidth;
	int mWindowHeight;

	sf::Color mClearColor;

private:
	GameManager();

	void Run();
	
	void HandleInput();
	void Update();
	void FixedUpdate();
	void Draw();

	void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }
	void SetFixedDeltaTime(float fixedDeltaTime) { mFixedDeltaTime = fixedDeltaTime; }

	sf::RenderWindow* GetWindow() const { return mpWindow; }

public:
	~GameManager();
	static GameManager* Get();

	void CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit = 60, sf::Color clearColor = sf::Color::Black);

	template<typename T>
	void LaunchScene();

	float GetDeltaTime() const { return mDeltaTime; }
	float GetFixedDeltaTime() const { return mFixedDeltaTime; }
	Scene* GetScene() const { return mpScene; }
	sf::Font& GetFont() { return mFont; };

	friend Debug;
	friend Scene;
};

#include "GameManager.inl"