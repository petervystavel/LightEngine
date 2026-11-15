#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"

void SampleScene::OnInitialize()
{
    mpEntity1 = CreateEntity<DummyEntity>(40.f, Color::Cyan);
    mpEntity1->SetPosition({ 300.f, 360.f });
}

void SampleScene::OnUpdate()
{
    if (InputManager::Get()->IsMouseDown(SDL_BUTTON_LEFT))
    {
        Vector2f mousePos = InputManager::Get()->GetMousePosition();

        //Do something
    }
}
