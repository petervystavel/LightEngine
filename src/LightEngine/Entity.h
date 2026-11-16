#pragma once

#include "Rendering.h"
#include "Vector.h"
#include "Color.h"

struct SDL_Renderer;

class Scene;

class Entity
{
protected:
    Circle mCircle;
    int mTag = -1;
    bool mToDestroy = false;

public:
    void SetPosition(Vector2f position, float ratioX = 0.5f, float ratioY = 0.5f);
    void SetTag(int tag) { mTag = tag; }
    float GetRadius() const { return mCircle.radius; }

    Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;

    bool IsTag(int tag) const { return mTag == tag; }

    void Destroy();
    bool ToDestroy() const { return mToDestroy; }

    template<typename T>
    T* GetScene() const;

    Scene* GetScene() const;
    float GetDeltaTime() const;

    template<typename T>
    T* CreateEntity(float radius, const Color& color);

protected:
    Entity() = default;
    virtual ~Entity() = default;

    virtual void OnUpdate() {}
    virtual void OnCollision(Entity* collidedWith) {}
    virtual void OnInitialize() {}
    virtual void OnDestroy() {}

private:
    void Update();
    void Draw(SDL_Renderer* renderer) const;
    void Initialize(float radius, const Color& color);

    friend class GameManager;
    friend class Scene;
};

#include "Entity.inl"
