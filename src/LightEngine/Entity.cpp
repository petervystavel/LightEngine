#include "Entity.h"

#include "GameManager.h"
#include "Debug.h"

#include <cassert>
#include <SDL3/SDL_render.h>

void Entity::Initialize(float radius, const Color& color)
{
    assert(radius > 0.f);

    mCircle.position = { 0.f, 0.f };
    mCircle.radius = radius;
    mCircle.color = color;
    mCircle.filled = true;

    OnInitialize();
}

void Entity::Destroy()
{
    if (mToDestroy)
        return;

    mToDestroy = true;

    OnDestroy();
}

void Entity::SetPosition(Vector2f position, float ratioX, float ratioY)
{
    float size = mCircle.radius * 2.f;

    position.x -= size * ratioX;
    position.y -= size * ratioY;

    mCircle.position = position;
}

Vector2f Entity::GetPosition(float ratioX, float ratioY) const
{
    float size = mCircle.radius * 2.f;
    Vector2f position = mCircle.position;

    position.x += size * ratioX;
    position.y += size * ratioY;

    return position;
}

void Entity::Update()
{
    float dt = GetDeltaTime();

    //#TODO

    OnUpdate();
}

void Entity::Draw(SDL_Renderer* renderer) const
{
    mCircle.Draw(renderer);
}

Scene* Entity::GetScene() const
{
    return GameManager::Get()->GetScene();
}

float Entity::GetDeltaTime() const
{
    return GameManager::Get()->GetDeltaTime();
}
