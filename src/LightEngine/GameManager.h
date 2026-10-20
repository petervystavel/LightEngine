#pragma once

#include <list>
#include <type_traits>
#include <cassert>

#include "Utils.h"

class Entity;
class Scene;
class Debug;

struct SDL_Window;
struct SDL_Renderer;

class GameManager
{
	std::list<Entity*> mEntities;
	std::list<Entity*> mEntitiesToDestroy;
	std::list<Entity*> mEntitiesToAdd;

	SDL_Window* mpWindow;
	SDL_Renderer* mpRenderer;
	int mFpsLimit;
	Uint64 mFrameDurationNS;
	//sf::Font mFont;

	Scene* mpScene;

	float mDeltaTime;

	int mWindowWidth;
	int mWindowHeight;

	Color mClearColor;

private:
	GameManager();

	void Run();
	
	bool Event();
	void Update();
	void Draw();

	void SetDeltaTime(Uint64 deltaTimeNS) { mDeltaTime = static_cast<float>(deltaTimeNS) / 1000000000.f; }

	SDL_Window* GetWindow() const { return mpWindow; }

public:
	~GameManager();
	static GameManager* Get();

	void CreateWindow(int width, int height, const char* title, int fpsLimit = 60, Color clearColor = { 0 });
	SDL_Renderer* GetRenderer() const { return mpRenderer; }

	template<typename T>
	void LaunchScene();

	float GetDeltaTime() const { return mDeltaTime; }
	Scene* GetScene() const { return mpScene; }
	//sf::Font& GetFont() { return mFont; };

	friend Debug;
	friend Scene;
};

#include "GameManager.inl"
