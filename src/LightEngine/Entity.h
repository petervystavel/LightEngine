#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include <list>

namespace sf 
{
	class Shape;
    class Color;
}

class Scene;

struct Force
{
    sf::Vector2f direction;
    float force;
    bool useMass;
};

class Entity
{
protected:
    sf::CircleShape mShape;
    sf::Vector2f mVelocity;
    std::list<Force> mForces;

    bool mToDestroy = false;
    int mTag = -1;

public:
    void AddForce(sf::Vector2f direction, float strength);
    void AddImpulse(sf::Vector2f direction, float impulse);
    void AddImpulse(sf::Vector2f velocity);
    void TryBounce(sf::Vector2f normal, float restitution = 1.f);
    void Bounce(sf::Vector2f normal, float restitution = 1.f);

    void SetPosition(sf::Vector2f position, float ratioX = 0.5f, float ratioY = 0.5f);
	void SetTag(int tag) { mTag = tag; }
	float GetRadius() const { return mShape.getRadius(); }

    sf::Vector2f GetPosition(float ratioX = 0.5f, float ratioY = 0.5f) const;
    float GetX(float ratioX = 0.5f) const;
    float GetY(float ratioY = 0.5f) const;
	sf::Shape* GetShape() { return &mShape; }

	bool IsTag(int tag) const { return mTag == tag; }
    bool IsColliding(Entity* other) const;
	bool IsInside(float x, float y) const;

    void Destroy();
	bool ToDestroy() const { return mToDestroy; }
	
	template<typename T>
	T* GetScene() const;

    Scene* GetScene() const;
	float GetDeltaTime() const;
    float GetFixedDeltaTime() const;
    int GetWindowWidth() const;
    int GetWindowHeight() const;

    template<typename T>
    T* CreateEntity(float radius, const sf::Color& color);

protected:
    Entity() = default;
    ~Entity() = default;

    virtual void OnUpdate() {};
    virtual void OnCollision(Entity* collidedWith) {};
	virtual void OnInitialize() {};
	virtual void OnDestroy() {};
	
private:
    void FixedUpdate();
    void Update();
	void Initialize(float radius, const sf::Color& color);

    friend class GameManager;
    friend Scene;
};

#include "Entity.inl"