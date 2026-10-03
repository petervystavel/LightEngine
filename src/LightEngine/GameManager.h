#pragma once

#include <list>

//#include <SFML/Graphics/Color.hpp>
//#include <SFML/Graphics/Text.hpp>

class Entity;
class Scene;
class Debug;

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
	//sf::Font mFont;

	Scene* mpScene;

	float mDeltaTime;

	int mWindowWidth;
	int mWindowHeight;

	//Color mClearColor;

private:
	GameManager();

	void Run();
	
	void HandleInput();
	void Update();
	void Draw();

	void SetDeltaTime(float deltaTime) { mDeltaTime = deltaTime; }

	sf::RenderWindow* GetWindow() const { return mpWindow; }

public:
	~GameManager();
	static GameManager* Get();

	//void CreateWindow(unsigned int width, unsigned int height, const char* title, int fpsLimit = 60, sf::Color clearColor = sf::Color::Black);

	template<typename T>
	void LaunchScene();

	float GetDeltaTime() const { return mDeltaTime; }
	Scene* GetScene() const { return mpScene; }
	//sf::Font& GetFont() { return mFont; };

	friend Debug;
	friend Scene;
};

#include "GameManager.inl"