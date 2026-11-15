#include "SampleScene.h"

#include "DummyEntity.h"
#include "InputManager.h"
#include "Debug.h"

void SampleScene::OnInitialize()
{
    mpEntity1 = CreateEntity<DummyEntity>(40.f, Color::Cyan);
    mpEntity1->SetPosition({ 300.f, 360.f });

    mpEntity2 = CreateEntity<DummyEntity>(60.f, Color::Orange);
    mpEntity2->SetPosition({ 700.f, 360.f });
}

void SampleScene::OnUpdate()
{
   
}
