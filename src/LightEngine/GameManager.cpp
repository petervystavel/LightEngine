#include "GameManager.h"

#include "Entity.h"
#include "Debug.h"

#include <iostream>

#include "SDL3/SDL.h"

GameManager::GameManager()
{
	mpWindow = nullptr;
	mpRenderer = nullptr;
	mDeltaTime = 0.0f;
	mpScene = nullptr;
	mWindowWidth = -1;
	mWindowHeight = -1;
	mClearColor = { 0 };
}

GameManager* GameManager::Get()
{
	static GameManager mInstance;

	return &mInstance;
}

GameManager::~GameManager()
{
	delete mpScene;

	for (Entity* entity : mEntities)
	{
		delete entity;
	}

	SDL_DestroyRenderer(mpRenderer);
	SDL_DestroyWindow(mpWindow);
	SDL_Quit();
}

void GameManager::CreateWindow(int width, int height, const char* title, int fpsLimit, Color clearColor)
{
	assert(mpWindow == nullptr);
	if (mpWindow != nullptr)
		return;

	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		SDL_Log("SDL initialization failed: %s", SDL_GetError());
		return;
	}

	if (SDL_CreateWindowAndRenderer(title, width, height, 0, &mpWindow, &mpRenderer) == false)
	{
		SDL_Log("Window and renderer creation failed: %s", SDL_GetError());
		SDL_DestroyRenderer(mpRenderer);
		SDL_DestroyWindow(mpWindow);
		mpRenderer = nullptr;
		mpWindow = nullptr;
		return;
	}

	mWindowWidth = width;
	mWindowHeight = height;

	mFpsLimit = fpsLimit;
	mFrameDurationNS = 1000000000ULL / mFpsLimit;

	mClearColor = clearColor;
}

void GameManager::Run()
{
	if (mpWindow == nullptr)
	{
		std::cout << "Window not created, creating default window" << std::endl;
		CreateWindow(1280, 720, "Default window");
	}

	if (mpWindow == nullptr || mpRenderer == nullptr || mpScene == nullptr)
		return;

	Uint64 previousTime = SDL_GetTicksNS();
	while (true)
	{
		const Uint64 frameStart = SDL_GetTicksNS();
		SetDeltaTime(frameStart - previousTime);
		previousTime = frameStart;

		bool quit = Event();
		if (quit == false)
			break;

		Update();

		Draw();

		const Uint64 elapsed = SDL_GetTicksNS() - frameStart;
		if (elapsed < mFrameDurationNS)
			SDL_DelayNS(mFrameDurationNS - elapsed);
	}
}

bool GameManager::Event()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
			return false;
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

    //Collision
    for (auto it1 = mEntities.begin(); it1 != mEntities.end(); ++it1)
    {
        auto it2 = it1;
        ++it2;
        for (; it2 != mEntities.end(); ++it2)
        {
            Entity* entity = *it1;
            Entity* otherEntity = *it2;

            if (entity->IsColliding(otherEntity))
            {
				if (entity->IsRigidBody() && otherEntity->IsRigidBody())
					entity->Repulse(otherEntity);

                entity->OnCollision(otherEntity);
                otherEntity->OnCollision(entity);
            }
        }
    }

	for (auto it = mEntitiesToDestroy.begin(); it != mEntitiesToDestroy.end(); ++it) 
	{
		delete *it;
	}

    mEntitiesToDestroy.clear();

	for (auto it = mEntitiesToAdd.begin(); it != mEntitiesToAdd.end(); ++it)
	{
		mEntities.push_back(*it);
	}

	mEntitiesToAdd.clear();
}

void GameManager::Draw()
{
	// Color stores its channels as 0xAARRGGBB.
	const Uint32 color = static_cast<Uint32>(mClearColor.argb);
	SDL_SetRenderDrawColor(mpRenderer,
		static_cast<Uint8>((color >> 16) & 0xFF),
		static_cast<Uint8>((color >> 8) & 0xFF),
		static_cast<Uint8>(color & 0xFF),
		static_cast<Uint8>((color >> 24) & 0xFF));
	SDL_RenderClear(mpRenderer);

	// Draw entities and debug shapes here using mpRenderer.

	SDL_RenderPresent(mpRenderer);
}
